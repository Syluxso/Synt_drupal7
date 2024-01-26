<?php
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.classes');
module_load_include('inc', basename(__DIR__), basename(__DIR__) . '.callbacks');

/*
 * This is an example of how to use the nytech_api endpoint system.
 */
function api_test($value) {
  $meta = new ReqMeta; // Gets session, basic auth, post, get, and custom values into a class.
  $meta->add_data('custom_key', $value); // The $value is the 'page argument' pass to the 'page callback' function.
  $auth = new Auth($meta); // Authorize the user and load the user data.
  $callback = new NyTechTestBootstrap($auth); //
  $synt = new Synt($callback);
  return $synt;
}

