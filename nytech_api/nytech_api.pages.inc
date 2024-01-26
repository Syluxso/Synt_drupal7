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

function api_notifications() {
  $callback = new NyTechAPINotifications;
  $synt = new Synt($callback);
  return $synt;
}

function api_jobs($org_id) {
  $args = [
    'org_id' => $org_id,
    'methods'  => 'GET,POST',
  ];
  $callback = new NyTechAPIImage($args);
  $synt = new Synt($callback);
  return $synt;
}

function api_jobs_image_view($org_id, $job_id, $image_id) {
  $args = [
    'org_id'   => $org_id,
    'job_id'   => $job_id,
    'image_id' => $image_id,
    'methods'  => 'GET,POST',
  ];
  $callback = new NyTechAPIImage($args);
  $synt = new Synt($callback);
  return $synt;
}
