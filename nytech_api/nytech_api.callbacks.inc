<?php

module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');


/*
 * All other classes in this file are to extend this class.
 */
class Callback {
  public $code = 200;
  public $message = 'OK';
  public $data;
  public $errors;

  public $authorized = false;
  public $account = false;

  public $request;
  public $auth;

  public $methods = 'GET';

  function __construct($array = false) {
    drupal_add_http_header('Access-Control-Allow-Origin', "*");
    $this->variables($array);
    $this->request();
    $this->auth();
  }

  private function variables($array) {
    if(is_array($array)) {
      foreach($array as $key => $value) {
        $this->add_property($key, $value);
      }
    }
  }

  function request() {
    $this->request = new RequestMeta();
  }

  function auth() {
    $auth = new Auth($this->request);
    $this->authorized = false;
    if($auth->authorized != true) {
      $this->add_error(403, 'Unauthorized');
    } elseif($auth->authorized == true) {
      $this->authorized = true;
    }
    if($this->authorized == true && !empty($auth->user)) {
      $this->account = new NyTechAccount($auth->user);
    }
    $this->auth = $auth;
  }

  function check_methods() {
    drupal_add_http_header('Access-Control-Allow-Methods', $this->methods);
    $array = explode(',', $this->methods);
    if(!in_array($this->request->method, $array)) {
      $this->add_error(405, 'Method Not Allowed');
    }
  }

  function add_property($key, $data) {
    $this->$key = $data;
  }

  function data($data) {
    if($this->authorized != false) {
      $this->data = $data;
    }
  }

  function status($code, $message) {
    $this->code = (int) $code;
    $this->message = $message;
  }

  function add_error($code, $ping) {
    $errors = [];
    if(!empty($this->errors)) {
      $errors = $this->errors;
    }
    $errors[] = new NyTechError($code, $ping);
    $this->errors = $errors;
  }

  function account_load($user_id) {
    $this->account = new NyTechAccount(user_load($user_id));
  }
}

/*
 * Classes that extend Callback.
 */

class NyTechTestBootstrap extends Callback {
  function __construct() {
    parent::__construct();
  }
}

class NyTechAPIAccount extends Callback {
  function __construct() {
    parent::__construct();
    $this->add_property('methods', 'GET,POST');
    $this->check_methods();
    if($this->request->method == 'POST') {
      // new NyTechAPIAccountUpdate($this->request);
      $this->account_load($this->auth->user_id);
      $this->status(201, 'Account updated.');
    }
    $this->data($this->account);
  }
}

