<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.callbacks');

/*
 * This is an example of how to use the nytech_api endpoint system.
 */
function endpoint_test($value) {
  $callback = new NyTechTest();
  $callback->add_property('path_var', $value);
  $synt = new Synt($callback);
  return $callback;
}

function endpoint_account() {
  $callback = new NyTechResourceAccount;
  $synt = new Synt($callback);
  return $synt;
}

function endpoint_notifications() {
  $callback = new NyTechResourceNotifications;
  $synt = new Synt($callback);
  return $synt;
}

function endpoint_deals() {
  $args = [
    'methods'  => 'GET,POST',
  ];
  $callback = new NyTechEpDeals($args);
  $synt = new Synt($callback);
  return $synt;
}

function endpoint_register() {
  $callback = new NyTechResourceRegister();
  $synt = new Synt($callback);
  return $synt;
}

function endpoint_login() {
  $callback = new NyTechResourceLogin();
  $synt = new Synt($callback);
  return $synt;
}
