<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.callbacks');

/*
 * Get and format all inbound information
 * - Headers
 * - Body
 * - Method
 * - Post data
 * - Get data
 * - user agent
 * - Path arguments /api/v1/%/%/% etc.
 */
class RequestMeta {
  public $get;
  public $post;
  public $agent;
  public $session;
  public $basic;
  public $method;

  function __construct($args = false) {
    $this->headers();
    $this->get();
    $this->post();
    $this->agent();
    $this->method();
  }

  function add_data($key, $value) {
    $array = $this->args;
    $array[$key] = $value;
    $this->args = $array;
  }

  function headers() {
    /*
     * Currently we are only looking for basic auth and session/sid auth data.
     */
    if(!empty($_SERVER['HTTP_AUTHORIZATION'])) {
      $base64 = substr($_SERVER['HTTP_AUTHORIZATION'],6);
      $user_pass_string = base64_decode($base64);
      $user_pass = explode(':', $user_pass_string);
      $basic = new stdClass();
      $basic->user = $user_pass[0];
      $basic->pass = $user_pass[1];
      $this->basic = $basic;
    }
    if(!empty($_SERVER['HTTP_SID'])) {
      $session = new stdClass();
      $session->sid = $_SERVER['HTTP_SID'];
      $this->session = $session;
    }
    if(!empty($this->session)) {
      unset($this->basic);
    }
  }

  function get() {
    $get = $_GET;
    unset($get['q']);
    if(!empty($get)) {
      $this->get = $get;
    }
  }

  function post() {
    $post = $_POST;
    if(!empty($post)) {
      $this->post = $post;
    }
  }

  function agent() {
    if(!empty($_SERVER['HTTP_USER_AGENT'])) {
      $this->agent = $_SERVER['HTTP_USER_AGENT'];
    }
  }

  function method() {
    $this->method = $_SERVER['REQUEST_METHOD'];
  }
}

/*
 * No-auth
 * Basic Auth
 * Session Auth
 */
class Auth {
  public $sid        = false;
  public $type       = false;
  public $user_id    = false;
  public $user       = false;
  public $authorized = false;

  function __construct($data) {
    if(!empty($data->session)) {
      $this->set_session($data);
    } elseif(!empty($data->basic)) {
      $this->set_basic($data);
    }
  }

  private function set_session($data) {
    if(!empty($data->session)) {
      $this->type = 'session';
      $session = new SessionAuth($data->session->sid);
      if($session->access == true) {
        $user = user_load($session->user_id);
        $this->set_user($user);
        $this->sid = $session->sid;
        $this->authorized = true;
      }
      unset($this->basic);
    }
  }

  private function set_basic($data) {
    if(!empty($data->basic)) {
      $this->type = 'basic';
      $basic = new BasicAuth($data->basic->user, $data->basic->pass);
      $this->sid = $basic->sid;
      $this->set_user($basic->user);
    }
  }

  private function set_user($user) {
    if($user != false) {
      $this->user = $user;
      $this->user_id = (int) $this->user->uid;
      $this->authorized = true;
    }
  }

  public function non_auth() {
    $this->type = 'non_auth';
    $this->authorized = true;
  }
}


/*
 * The formatter class for getting data back to the requester.
 * Properties include
 * - status: required
 * - code: required
 * - message: optional.
 * - data: required.
 * - errors: optional.
 */
class Synt {
  /*
   * The only goal of this class is to dynamically set the properties based on the xClass passed into it.
   */
  public $code = 200;
  public $message = 'Ok.';
  public $data = false;
  public $errors;

  private $xClass;

  function __construct($callbac) {
    $this->xClass = $callbac;
    $this->data($callbac);
    $this->status($callbac);
    $this->errors($callbac);
    $this->finish();
  }

  function status($callbac) {
    if(!empty($callbac->code) && !empty($callbac->message)) {
      $this->code = (int) $callbac->code;
      $this->message = $callbac->message;
    }
  }

  function data($callbac) {
    if(!empty($callbac->data)) {
      $this->data = $callbac->data;
    }
  }

  function errors($callbac) {
    if(!empty($callbac->errors)) {
      $this->errors = $callbac->errors;
      // Update status;
      foreach($callbac->errors as $error) {
        $this->code = $error->code;
        $this->message = $error->message;
      }
    } else {
      unset($this->errors);
    }
  }

  function finish() {
    drupal_add_http_header('Status', $this->code . ' ' . $this->message);
  }

  public function debug() {
    $this->debug = $this->xClass;
  }

}


/*
 * AUTHORIZATION
 */
class BasicAuth {

  public $access = false;
  public $user_id = false;
  public $user = false;
  public $sid = false;

  function __construct($user_name, $pass) { // On registration $user->name is synced to $user->mail.
    $id = user_authenticate($user_name, $pass);
    $this->set_user_id($id);

    if ($this->user_id > 0) {
      $account = array('uid' => $this->user_id);
      user_login_submit(array(), $account);
      global $user;
      $this->user = user_load($user->uid);
      $sid = session_id();
      $hashed_session_id = drupal_hash_base64($sid);
      $this->set_sid($hashed_session_id);
      $this->set_access(TRUE);
    } else {
      $this->user = false;
    }
  }

  private function set_user_id($id) {
    $this->user_id = (int) $id;
  }

  private function set_sid($sid) {
    $this->sid = $sid;
  }

  private function set_access($new_access) {
    $this->access = $new_access;
  }
}

/*
 * SessionAuth is the method to authenticate a user if they have a $sid that is valid and current.
 */
class SessionAuth {
  public $access = false;
  public $user_id = 0;
  public $sid = null;

  function __construct($sid) {
    if(!empty($sid)) {
      $query = 'SELECT s.uid FROM {sessions} s WHERE s.sid = :sid';
      $args = array(
        ':sid' => $sid,
      );
      $result = db_query($query, $args);
      foreach ($result as $record) {
        $this->set_access(TRUE);
        $this->set_user_id($record->uid);
        $this->set_sid($sid);
      }
    }
  }

  private function set_user_id($id) {
    $this->user_id = (int) $id;
  }

  private function set_sid($sid) {
    $this->sid = $sid;
  }

  private function set_access($new_access) {
    $this->access = $new_access;
  }
}

class AnonAuth {
  public $access = true;
  public $user_id = 0;
  public $sid = null;

  function __construct($auth) {
    if(!empty($auth->user_id) ) {
      $this->user_id = $auth->user_id;
    }
    if(!empty($auth->sid)) {
      $this->sid = $auth->sid;
    }
  }
}

class NyTechError {
  public $code;
  public $message;
  public function __construct($code, $message) {
    $this->code = $code;
    $this->message = $message;
    watchdog('error', print_r($this, true));
  }
}
