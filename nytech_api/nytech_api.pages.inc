<?php

function api_test($value) {
  $meta = new ReqMeta;
  $auth = new Auth($meta);
  $callback = new NyTechTestBootstrap($auth);
  $synt = new Synt($callback);
  return $synt;
}



class NyTechTestBootstrap extends Callback {

  public $test;
  public $auth;
  public $meta;

  function __construct($auth) {
    $this->auth($auth);
    $this->permit_methods('GET');
    $this->status(201, 'Content created yo');
    $this->data([
      'ping' => 234,
      'ping1' => 234,
      'ping2' => 234,
      'ping3' => 234,
    ]);
  }
}
