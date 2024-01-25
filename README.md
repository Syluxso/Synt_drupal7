How to use

Each endpoint should be highly formulic so that all or most of the logic happens in the class that is fired at that endpoint.

function api_test($value) {
  $meta = new ReqMeta; 
  $auth = new Auth($meta);
  $callback = new NyTechTestBootstrap($auth);
  $synt = new Synt($callback);
  return $synt;
}
