How to use

Each endpoint should be highly formulic so that all or most of the logic happens in the class that is fired at that endpoint.

```php
function api_test($value) {
  /*
   *  $value is only present if the has an argument 'page_arguments' => [3] for example.
   */
  
  /*
   * ReqMeta creates the following properties.
   * - User Agent
   * - Basic = basic->user & basic->pass
   * - Session = session->sid
   * - $_GET data if any. This automatically excludes the Drupal q variable in the get request.
   * - $_POST data if any.
   * - Args: This is any custom key/value set you want to pass in. it is intended to be used with the page_arguments data if any.
   */
  $meta = new ReqMeta;

  /*
   * Auth take the $meta as the only argument.
   * Authenticates either basic auth data or session data from $meta. If there is BOTH then the session data will be honored.
   * Properties
   * - user as the loaded object or false.
   * - user_id as ain int or false.
   * - authorized  as true/false.
   * - type as either 'session' or 'basic'.
   * - Use the ->non_auth method to skip authentication. All properties will be false other than ->authorized = true.
   */
  $auth = new Auth($meta);

  /*
   * This class must extend the Callback class.
   * Many dynamic properties may be assiged as you wish.
   * To function properly you must use the following methods
   * ->auth($auth) and pass in the $auth class from above. If auth fails:
   * -- The data property of the response will be set to false.
   * -- add_error(403, 'Not authorized') will automatically be set.
   * ->permit_methods('GET,POST'). This will limit what type of call can be made against this and will automatically throw an error if the wrong method is used.
   * ->status($code, $message). Use this for setting success messages. it will be used in the responce json as well as the header status code.
   * ->add_error($code, $message). Use for any errors. The responce will have the last error reported and the header status will be set. This overrides the ->status() method.
   */
  $callback = new NyTechTestBootstrap($auth);

  /*
   * This class will automatically use the code, message, data, and errors from and format them for uniformity.
   * ->code
   * ->message
   * ->data (false if auth fails)
   * ->errors (if ther are errors).
   * 
   * Any other properties passed in will be ignored.
   */
  $synt = new Synt($callback);
  return $synt;
}
```

