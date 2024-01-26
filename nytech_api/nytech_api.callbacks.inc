<?php

module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');


/*
 * All other classes in this file are to extend this class.
 */
class Callback {
  public $status;
  public $message;
  public $data;
  public $errors;
  
  function __construct($auth) {
    drupal_add_http_header('Access-Control-Allow-Origin', "*");
    $this->auth($auth);
    unset($this->data);
  }
  
  function auth($auth) {
    $this->authorized = false;
    if($auth->authorized != true) {
      $this->add_error(403, 'Unauthorized');
    } elseif($auth->authorized == true) {
      $this->authorized = true;
    }
  }
  
  function permit_methods($string = 'GET,POST') {
    $array = explode(',', $string);
    drupal_add_http_header('Access-Control-Allow-Methods', $string);
    if(!in_array($_SERVER['REQUEST_METHOD'], $array)) {
      $this->add_error(405, 'Method Not Allowed');
    }
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
}

/*
 * Classes that extend Callback.
 */