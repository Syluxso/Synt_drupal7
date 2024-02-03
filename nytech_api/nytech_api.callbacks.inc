<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.eclass');

/*
 * Classes that extend Callback.
 */

class NyTechTest extends Callback {
  function __construct() {
    parent::__construct();
    $this->data([
      'test' => 'data',
      'ping' => 'pong',
      'foo' => [
        'bar',
        'far',
        'rar',
      ]
    ]);
  }
}

class NyTechResourceAccount extends Callback {
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

class NyTechResourceNotifications extends Callback {
  function __construct() {
    parent::__construct();
    $method_ok = $this->check_methods();
    if($method_ok) {
      $notifications = new NyTechUserNotifications($this->auth->user);
      $notifications->query();
      $objects = [];
      foreach($notifications->objects as $entity) {
        $objects[] = new NyTechNotification($entity);
      }
      $this->data($objects);
    }
  }
}

class NyTechEpDeals extends Callback {
  function __construct($array = false) {
    $this->variables($array);
    parent::__construct();
    $method_ok = $this->check_methods();
    if($method_ok) {
      $queryClass = new NyTechAPIUsersDeals;
      $list = [];
      if(!empty($queryClass->objects)) {
        foreach($queryClass->objects as $object) {
          $list[] = new NyTechAPIDeal($object);
        }
      }
      $data = [
        'count' => $queryClass->count,
        'list'  => $list,
      ];


      $this->data($data);
    }
  }
}

class NyTechRecourseRegister extends Callback {
  function __construct() {
    $this->variables(['methods' => 'POST']);
    $parent_args = ['non_auth' => true];
    parent::__construct($parent_args);
    $method_ok = $this->check_methods();
    if($method_ok) {
      $register = new NyTechAPIRegister($this->request->post['user'], $this->request->post['phone'], $this->request->post['pass']);
      if(!empty($register->error)) {
        $this->add_error($register->error->code, $register->error->message);
      } elseif(!empty($register->account)) {
        $data = $register->account;
        $data->sid = $register->login->sid;
      }
      $this->data($data);
    }
  }
}

class NyTechRecourseLogin extends Callback {
  function __construct() {
    $this->variables(['methods' => 'POST']);
    $parent_args = ['non_auth' => true];
    parent::__construct($parent_args);
    $method_ok = $this->check_methods();
    if($method_ok) {
      $register = new NyTechAPILogin($this->request->post['phone'], $this->request->post['pass']);
      if(!empty($register->error)) {
        $this->add_error($register->error->code, $register->error->message);
      } elseif(!empty($register->account)) {
        $data = $register->account;
        $data->sid = $register->login->sid;
      }
      $this->data($data);
    }
  }
}

/*
 * MODELS
 */


class NyTechAPIUsersDeals {

  public $user_id;
  public $count = 0;
  public $objects;

  private $entity_type = 'deal';
  private $bundle      = 'standard';
  private $status      = 1;

  function __construct() {
    $this->user();
    $this->query();
  }

  private function user() {
    global $user;
    $this->user_id = (int) $user->uid;
  }

  private function query() {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entity_type)
      ->entityCondition('bundle', $this->bundle)
      ->propertyCondition('status', $this->status)
      ->addMetaData('account', user_load(1));

    $result = $query->execute();
    if (isset($result[$this->entity_type])) {
      $ids           = array_keys($result[$this->entity_type]);
      $this->objects = entity_load($this->entity_type, $ids);
      $this->count   = count($ids);
    }
  }
}
