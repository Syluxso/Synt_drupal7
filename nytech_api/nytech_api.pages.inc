<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.callbacks');

/*
 * This is an example of how to use the nytech_api endpoint system.
 */
function api_test($value) {
  $callback = new NyTechTest();
  $callback->add_property('path_var', $value);
  $synt = new Synt($callback);
  return $callback;
}

function api_account() {
  $callback = new NyTechAPIAccount;
  $synt = new Synt($callback);
  return $synt;
}
