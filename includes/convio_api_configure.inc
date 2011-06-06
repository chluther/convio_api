<?php

/**
 * Create the form for managing configuration.
 */
/**
 * Create the form for managing configuration.
 */
function convio_api_admin() {

  $form['convio_api_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Convio Server'),
    '#default_value' => variable_get('convio_api_server', ''),
    '#size' => 75,
    '#maxlength' => 256,
    '#description' => t('The URI reference to the root of the Convio Affiliate site.  Includes protocol, host, and affiliate short name with no trailing slash.  Example: https://secure2.convio.net/shortname'),
    '#required' => TRUE,
  );

  $form['convio_api_ca_verification'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable SSL Certificate Verification.'),
      '#default_value' => variable_get('convio_api_ca_verification', 'FALSE'),
      '#description' => t('Check to enable SSL certificate verification between the Drupal and Convio servers.  See INFO.'),
  );

  $form['convio_api_autosync'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Auto-Sync.'),
      '#default_value' => variable_get('convio_api_autosync', 'TRUE'),
      '#description' => t('Enable Convio API module managed synchronization between the Drupal user & profile databases and Convio Constituent 360 database.  See INFO.'),
  );

  $form['convio_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Convio API Key'),
      '#default_value' => variable_get('convio_api_key', ''),
      '#size' => 75,
      '#maxlength' => 256,
      '#description' => t('The Convio API Key defined in Convio Site Configuration CONVIO_API_KEY.'),
      '#required' => TRUE,
  );

    $form['convio_api_skey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Secret Key File'),
    '#default_value' => variable_get('convio_api_skey', ''),
    '#size' => 75,
    '#maxlength' => 100,
    '#description' => t('The URI to the client-specific secret key for encrypting Convio Open API calls.  See INFO.'),
    '#required' => TRUE,
  );

  $form['convio_api_login_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Convio API Administrative Account'),
      '#default_value' => variable_get('convio_api_login_name', ''),
      '#size' => 25,
      '#maxlength' => 25,
      '#description' => t('The user_name of the Convio administrative account that was created for API access.'),
      '#required' => TRUE,
  );

  $form['convio_api_login_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Convio API Administrative Password'),
      '#default_value' => variable_get('convio_api_login_password', ''),
      '#size' => 25,
      '#maxlength' => 25,
      '#description' => t('The password of the Convio administrative account that was created for API access.'),
      '#required' => TRUE,
  );

  $form['convio_api_enableCKeditor'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable CKeditor Integration?'),
      '#default_value' => variable_get('convio_api_enableCKeditor', 'FALSE'),
  );

  $profile_fields_q = _profile_get_fields('Convio');
  while ($profile_fields_r = db_fetch_array($profile_fields_q)) {
    $profile_fields[$profile_fields_r['fid']] = $profile_fields_r['title'] . ' (' . $profile_fields_r['name'] . ')';
  }
  $form['convio_api_profile_field_id'] = array(
    '#type' => 'select',
    '#title' => t('Convio ID Profile Field'),
    '#description' => t("The profile field that stores the user's Convio ID - used if they can't be found in the {authmap} table."),
    '#default_value' => variable_get('convio_api_profile_field_id', FALSE),
    '#options' => $profile_fields,
  );

  $form['convio_api_test_set'] = array(
        '#title' => t('Connection Testing'),
        '#type' => 'fieldset',
        '#description' => t('To test the connection with the Convio Affiliate server enter either the Convio Account ID OR the primary email of a existing Convio user.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

          $form['convio_api_test_set']['convio_api_test_id'] = array(
                '#type' => 'textfield',
                '#title' => t('Test Cons_ID'),
                '#default_value' => variable_get('convio_api_test_id', ''),
                '#size' => 25,
                '#maxlength' => 25,
          );
          $form['convio_api_test_set']['convio_api_test_email'] = array(
                '#type' => 'textfield',
                '#title' => t('Test Email'),
                '#default_value' => variable_get('convio_api_test_email', ''),
                '#size' => 25,
                '#maxlength' => 25,
          );

          $api_options = convio_api_apis();

           $form['convio_api_test_set']['convio_api_test_api'] = array(
                '#title' => t('Select API'),
                '#type' => 'select',
                '#description' => t('Select the API to test.'),
                '#options' => $api_options,
                '#multiple' => FALSE,
                '#default_value' => variable_get('convio_api_test_api', 'getUser'),
            );

            $form['convio_api_test_set']['convio_api_format'] = array(
                '#type' => 'radios',
                '#title' => t('Format result'),
                '#options' => array(
                               'json' => 'json',
                               'xml' => 'xml',
                            ),
                '#default_value' => variable_get('convio_api_format', 'json'),
            );

            $form['convio_api_test_set']['convio_api_test_button'] = array(
                '#type' => 'submit',
                '#value' => t('Test Now'),
                '#submit' => array('convio_api_test_button_submit'),
                '#executes_submit_callback' => TRUE,
          );

  // Initialize the form from the class.
  /*
  $c = ConvioConfiguration::getInstance();
  if ($c) {
    $form['convio_api_server']['#default_value'] = $c->getURI();
    $form['convio_api_key']['#default_value'] = $c->getPublicKey();
    $form['convio_api_skey']['#default_value'] = $c->getSecretKeyLocation();
    $form['convio_api_login_name']['#default_value'] = $c->getUserName();
    $form['convio_api_login_password']['#default_value'] = $c->getPassword();
  }
  */

  return system_settings_form($form);
}

function convio_api_configuration_form_validate($form, &$form_state) {
  $api_uri = $form_state['values']['convio_api_server'];
  $api_key = $form_state['values']['convio_api_key'];
  $api_skey = $form_state['values']['convio_api_skey'];

  if (! valid_url($api_uri, true)) {
    form_set_error('convio_api_server', t('The API URI is not a valid URL.'));
    return;
  }

  // Validate the secret key file.
  $skey = FALSE;
  try {
    $fh = fopen($api_skey, 'r');
    $skey = trim(fgets($fh));
    fclose($fh);
  } catch (Exception $e) {
    form_set_error('convio_api_skey', t('Unable to read the secret key file. Please check the file location and permissions: ' . $e->getMessage()));
    return;
  }
  if (! $skey) {
    form_set_error('convio_api_skey', t('Invalid secret key file.'));
    return;
  }

  if (! ConvioConfiguration::test($api_uri, $api_key)) {
    form_set_error('convio_api_key', t('The Convio Open API configuration is not valid. Please check the URI and key, and make sure that API access has been enabled in the Convio application.'));
    return;
  }
}


/*
 *
  function convio_api_configuration_form_submit($form, &$form_state) {
  $c = new ConvioConfiguration();
  $c->setURI($form_state['values']['api_uri']);
  $c->setPublicKey($form_state['values']['api_key']);
  $c->setSecretKeyLocation($form_state['values']['api_skey']);
  $c->setUserName($form_state['values']['api_user']);
  $c->setPassword($form_state['values']['api_password']);
  $c->save();

  $form_state['redirect'] = 'admin';
}
*/

/**
* This function tests the request parameters with the Convio server.
*/
function convio_api_test_button_submit($form, &$form_state) {

   system_settings_form_submit($form, $form_state);
  $item = '<b>Convio API Test Begun</b>';
  drupal_set_message($item);
  $test_api = $form_state['values']['convio_api_test_api'];
  $test_format = $form_state['values']['convio_api_format'];
  $test_server = variable_get('convio_api_server', '');
  $test_apikey = variable_get('convio_api_key', '');
  $test_account = variable_get('convio_api_login_name', '');
  $test_pw = variable_get('convio_api_login_password', '');
  $test_id = variable_get('convio_api_test_id', NULL);
  $test_email = variable_get('convio_api_test_email', NULL);
  if ($test_id == '') unset($test_id);
  if ($test_email == '') unset($test_email);
  if (!empty($test_id)) { // move into case statements
      drupal_set_message('Testing API using Convio ID = '. $test_id);
    }
    else {
      drupal_set_message('Testing API using Primary Email = '. $test_email);
    }

  switch ($test_api) {
    case 'create' :
      /* TODO need form input of the following:
     * primary_email - Required. The primary email address that uniquely identifies the constituent.
     * add_center_ids - Optional. Comma-separated list of numeric IDs of centers to add the user to.
     * add_group_ids - Optional. Comma-separated list of numeric IDs of groups to add the user to. Only available when called using an administrator account.
     * add_interest_ids - Optional. Comma-separated list of numeric IDs of email interests to add the user to.
     * source - Optional. Text to associate with the record to identify how or from where the record was added. Only used when creating new records.
     * no_welcome - Optional. Only used when creating new records. Specifies whether to send a "welcome" auto- responder email to the new user. If the value is "f" or "FALSE" (ignoring case) and the site is configured to send welcome auto-responders to new users, then one will be sent. By default, this value is "TRUE", meaning that no auto-responder email will be sent.
     * cons_id - Optional. The constituent ID (Convio ID) that uniquely identifies the constituent. This value is automatically assigned when a new constituent record is created.
     * member_id - Optional. The member ID (Client ID) that uniquely identifies the constituent.
     * remove_center_ids - Optional. Comma-separated list of numeric IDs of centers to remove the user from.
     * remove_group_ids - Optional. Comma-separated list of numeric IDs of groups to remove the user from. Only valid when updating records. Only available when called using an administrator account. Cannot be used for pre-defined groups (those with IDs less than 1000). Cannot be used for administrator security groups.
     * remove_interest_ids - Optional. Comma-separated list of numeric IDs of email interests to remove the user from. Only valid when updating records.
     */
      $test_add_center_ids="";
      $test_add_group_ids="";
      $test_add_interest_ids='';
      $test_source='';
      $test_welcome='';
      //          convio_api_create(  $primary_email,  $add_center_ids="" ,  $add_group_ids="" ,  $add_interest_ids='' ,  $source='' ,  $no_welcome='' ,  $format='', $server='' , $apikey='' , $account='' , $pw='' )
        $string = convio_api_create_call($test_email , $test_add_center_ids, $test_add_group_ids, $test_add_interest_ids, $test_source, $test_no_welcome, $test_format , $test_server , $test_apikey , $test_account , $test_pw  );
        drupal_set_message('<b>create():</b>');
        drupal_set_message( $string);
        // make the call
        $test_result = convio_api_create($test_email , $test_add_center_ids, $test_add_group_ids, $test_add_interest_ids, $test_source, $test_no_welcome, $test_format , $test_server , $test_apikey , $test_account , $test_pw  );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'createOrUpdate' :
        //  TODO need form input:
        $test_member_id = NULL;
        $test_add_center_ids= NULL;
        $test_add_group_ids= NULL;
        $test_add_interest_ids= NULL;
        $test_source= NULL;
        $test_no_welcome= NULL;
        $test_remove_center_ids = NULL;
        $test_remove_group_ids = NULL;
        $test_remove_interest_ids = NULL ;
        //       convio_api_createOrUpdate(      $primary_email,  $cons_id, $member_id,   $add_center_ids,     $add_group_ids,      $add_interest_ids ,
        //                                       $remove_center_ids,     $remove_group_ids,      $remove_interest_ids,      $source,      $no_welcome,
        //                                       $format, $server, $apikey, $account, $pw );
        $string = convio_api_createOrUpdate_call($test_email , $test_id, $test_member_id, $test_add_center_ids, $test_add_group_ids, $test_add_interest_ids,
                                                 $test_remove_center_ids, $test_remove_group_ids, $test_remove_interest_ids, $test_source, $test_no_welcome,
                                                 $test_format , $test_server , $test_apikey , $test_account , $test_pw  );
        drupal_set_message('<b>createOrUpdate():</b>');
        drupal_set_message( $string);
        // make the call
        $test_result = convio_api_createOrUpdate($test_email , $test_id, $test_member_id, $test_add_center_ids, $test_add_group_ids, $test_add_interest_ids,
                                                 $test_remove_center_ids, $test_remove_group_ids, $test_remove_interest_ids, $test_source, $test_no_welcome,
                                                 $test_format , $test_server , $test_apikey , $test_account , $test_pw  );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };

    break;

    case 'getGroupMembers' :
      // TODO - need form input
       $test_group_id = 100;
       $test_max_number = 10;
       $test_fields=  NULL;
       //         convio_api_getGroupMembers     ( $group_id,      $max_number='' ,  $fields='' ,  $format='' ,  $server='' ,    $apikey='' ,   $account='' ,   $pw='' )
        $string = convio_api_getGroupMembers_call($test_group_id , $test_max_number, $test_fields, $test_format , $test_server , $test_apikey , $test_account , $test_pw  );
        drupal_set_message('<b>getGroupMembers():</b>');
        drupal_set_message( $string);
        // make the call
        $test_result = convio_api_getGroupMembers($test_group_id , $test_max_number, $test_fields, $test_format , $test_server , $test_apikey , $test_account , $test_pw   );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'getUser':
        //        convio_api_getUser_call( $consid='' , $email='' , $format='' ,  $server='' ,   $apikey='' ,   $account='' ,   $pw='' )
        $string = convio_api_getUser_call($test_id , $test_email , $test_format , $test_server , $test_apikey , $test_account , $test_pw  );
        drupal_set_message('<b>getUser():</b>');
        drupal_set_message( $string);
        // make the call
        $test_result = convio_api_getUser($test_id , $test_email , $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'getUserGroups':
        $test_member_id = ''; // TODO provide form input
        //        convio_api_getUserGroups(      $consid='' ,$member_id="",$primary_email='',$format='',  $server='' , $apikey='' ,  $account='' ,  $pw='' )
        $string = convio_api_getUserGroups_call( $test_id, $test_member_id, $test_email, $test_format, $test_server, $test_apikey, $test_account, $test_pw );
        drupal_set_message('<b>getUserGroups():</b>');
        drupal_set_message( $string );
        // make the call
        $test_result = convio_api_getUserGroups( $test_id, $test_member_id, $test_email, $test_format, $test_server, $test_apikey, $test_account, $test_pw );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'getUserInterests':
        //        convio_api_getUserInterests(     $consid='' , $email='' ,    $format='' ,   $server='' ,   $apikey='' ,   $account='' ,   $pw='' )
        $string = convio_api_getUserInterests_call($test_id , $test_email , $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        drupal_set_message('<b>getUserInterests():</b>');
        drupal_set_message( $string );
        // make the call
        $test_result = convio_api_getUserInterests($test_id , $test_email , $test_format , $test_server , $test_apikey , $test_account , $test_pw  );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

   case 'isEmailValid':
        //        convio_api_isEmailValid_call( $email,      $format='' ,   $server='' ,   $apikey='' ,   $account='' ,   $pw='' )
        $string = convio_api_isEmailValid_call( $test_email, $test_format , $test_server , $test_apikey , $test_account , $test_pw ) ;
        drupal_set_message('<b>isEmailValid():</b>');
        drupal_set_message( $string );
        $test_result = convio_api_isEmailValid( $test_email, $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'isUserInterested':
      // TODO - handle id vs. email and notify in test ui what is being sent
      //          convio_api_isUserInterested_call( $consid='' , $email='' , $format='' , $server='' , $apikey='' , $account='' ,  $pw='' )
        $string = convio_api_getGroupMembers_call($test_id, $test_email, $test_format, $test_server, $test_apikey, $test_account , $test_pw );
        drupal_set_message('<b>getGroupMembers():</b>');
        drupal_set_message( $string );
        $test_result = convio_api_getGroupMembers($test_id, $test_email, $test_format, $test_server, $test_apikey, $test_account , $test_pw );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

   case 'listInterests':
        //TODO add to test ui - possibly
        $test_include_hidden = FALSE;
        //        convio_api_listInterests_call( $include_hidden='' , $format='' ,   $server='' ,   $apikey='' ,   $account='' ,   $pw='' )
        $string = convio_api_listInterests_call( $test_include_hidden, $test_format, $test_server , $test_apikey , $test_account , $test_pw ) ;
        drupal_set_message('<b>listInterests():</b>');
        drupal_set_message( $string );
        $test_result = convio_api_listInterests( $test_include_hidden, $test_format, $test_server , $test_apikey , $test_account , $test_pw ) ;
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'listUserFieldChoices' :
        // TODO add to test ui
        $test_field = "name.title";
        drupal_set_message('Testing API using field = '. $test_field);
        //        convio_api_listUserFieldChoices_call( $field ,      $format='' ,   $server='' ,   $apikey='' ,   $account='' ,   $pw='' )
        $string = convio_api_listUserFieldChoices_call( $test_field , $test_format , $test_server , $test_apikey , $test_account , $test_pw ) ;
        drupal_set_message('<b>listUserFieldChoices:</b>');
        drupal_set_message( $string );
        $test_result = convio_api_listUserFieldChoices( $test_field , $test_format , $test_server , $test_apikey , $test_account , $test_pw ) ;
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0] ;
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'listUserFields' :
       //  TODO - possibly add $test_access to test ut
       $test_access = NULL;
       //         convio_api_listUserFields_call( $access='' ,   $format='' ,   $server='' ,  $apikey='' ,  $account='' ,  $pw=''
        $string = convio_api_listUserFields_call( $test_access , $test_format , $test_server, $test_apikey, $test_account, $test_pw) ;
        drupal_set_message('<b>listUserFields:</b>');
        drupal_set_message( $string );
        $test_result = convio_api_listUserFields( $test_access , $test_format , $test_server, $test_apikey, $test_account, $test_pw) ;
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'logInteraction' :
       //  TODO - possibly add $test_access to test ut
       $test_member_id = NULL;
       $test_interaction_subject = 'Test_Subject_Heading';
       $test_interaction_body = NULL;
       $test_interaction_cat_id = NULL;
       $test_interaction_count = NULL;
       drupal_set_message('Testing API using interaction_subject = Test_Subject_Heading');

       //         convio_api_logInteraction     ( $primary_email='', $cons_id='', $member_id='',
       //                                    $interaction_subject , $interaction_body='', $interaction_cat_id='', $interaction_count='',
       //                                    $format='' , $server='' , $apikey='' , $account='' , $pw='' )
        $string = convio_api_logInteraction_call( $test_email , $test_id, $test_member_id,
                                            $test_interaction_subject , $test_interaction_body, $test_interaction_cat_id, $test_interaction_count,
                                            $test_format , $test_server, $test_apikey, $test_account, $test_pw) ;
        drupal_set_message('<b>logInteraction:</b>');
        drupal_set_message( $string );
        $test_result = convio_api_logInteraction( $test_email , $test_id, $test_member_id,
                                            $test_interaction_subject , $test_interaction_body, $test_interaction_cat_id, $test_interaction_count,
                                            $test_format , $test_server, $test_apikey, $test_account, $test_pw) ;
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'update' :
        //  TODO need form input:
        $test_member_id = NULL;
        $test_add_center_ids= NULL;
        $test_add_group_ids= NULL;
        $test_add_interest_ids= NULL;
        $test_remove_center_ids = NULL;
        $test_remove_group_ids = NULL;
        $test_remove_interest_ids = NULL ;
        //       convio_api_createOrUpdate(      $primary_email,  $cons_id, $member_id,   $add_center_ids,     $add_group_ids,      $add_interest_ids ,
        //                                       $remove_center_ids,     $remove_group_ids,      $remove_interest_ids,
        //                                       $format, $server, $apikey, $account, $pw );
        $string = convio_api_update_call($test_email , $test_id, $test_member_id, $test_add_center_ids, $test_add_group_ids, $test_add_interest_ids,
                                                 $test_remove_center_ids, $test_remove_group_ids, $test_remove_interest_ids,
                                                 $test_format , $test_server , $test_apikey , $test_account , $test_pw  );
        drupal_set_message('<b>update():</b>');
        drupal_set_message( $string);
        // make the call
        $test_result = convio_api_update($test_email , $test_id, $test_member_id, $test_add_center_ids, $test_add_group_ids, $test_add_interest_ids,
                                                 $test_remove_center_ids, $test_remove_group_ids, $test_remove_interest_ids,
                                                 $test_format , $test_server , $test_apikey , $test_account , $test_pw  );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };

    break;

    case 'authenticateUser' :
            // TODO - how to show test?
            // convio_api_authenticateUser( $format='' , $server='' , $apikey='' , $account='' , $pw='')
            drupal_set_message($test_api .' can not be demonstrated via the Adminstrative UI.', 'warning');
    break;

    case 'changePassword' :
           // TODO how to show test?
           //convio_api_changePassword( $old_password, $user_password, $retype_password, $reminder_hint, $format='' , $server='' , $apikey='' , $account='' , $pw='' )
           drupal_set_message($test_api .' can not be demonstrated via the Adminstrative UI.', 'warning');
    break;

    case 'getSingleSignOnToken' :
        //  TODO need form input:
        $test_member_id = NULL;

        //        convio_api_getSingleSignOnToken     (  $cons_id ='', $member_id ='' , $format=''   , $server=''   , $apikey=''   , $account=''   , $pw='')
        $string = convio_api_getSingleSignOnToken_call($test_id , $test_member_id, $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        drupal_set_message('<b>getSingleSignOnToken():</b>');
        drupal_set_message( $string);
        // make the call
        $test_result = convio_api_getSingleSignOnToken($test_id , $test_member_id, $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };

    break;

    case 'login' :
        //  TODO need form input:
        $test_user_name = NULL;
        $test_password = NULL ;
        $test_send_user_name = NULL ;
        $test_email = NULL;
        $test_logout = NULL;

         //       convio_api_login     ( $user_name ='' , $password ='' , $send_user_name = '', $email = '', $logout = '',
         //                              $format=''   , $server=''   , $apikey=''   , $account=''   , $pw='')
        $string = convio_api_login_call($test_user_name , $test_password, $test_send_user_name, $test_email, $test_logout,
                                         $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        drupal_set_message('<b>login():</b>');
        drupal_set_message( $string);
        // make the call
        $test_result = convio_api_login($test_user_name , $test_password, $test_send_user_name, $test_email, $test_logout,
                                         $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'getAdvocacyAlerts' :
        // TODO need form input
        $test_list_record_offset = NULL;
        $test_list_page_offset = NULL;
        $test_issue_id = NULL;
        $test_alert_status = 'ACTIVE';
        $test_alert_type = 'ACTION';
        $test_publish = NULL;
        $test_expire = NULL;
       // convio_api_getAdvocacyAlerts             ( $list_record_offset ='' , $list_page_offset ='' , $issue_id = '', $alert_status, $alert_type, $publish = '' , $expire = '',
      //                                  $format='' , $server='' , $apikey='' , $account='' , $pw='' )
        $string = convio_api_getAdvocacyAlerts_call($test_list_record_offset , $test_list_page_offset, $test_issue_id, $test_alert_status, $test_alert_type, $test_publish, $test_expire,
                                         $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        drupal_set_message('<b>getAdvocacyAlerts():</b>');
        drupal_set_message( $string);
        // make the call
        $test_result = convio_api_getAdvocacyAlerts($test_record_offset , $test_list_page_offset, $test_issue_id, $test_alert_status, $test_alert_type, $test_publish, $test_expire,
                                         $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    case 'getAdvocacyInteractions' :
       // TODO - need form input
       $test_list_record_offset = NULL;
       $test_list_page_offset = NULL;
       $test_page_size = NULL;
       $test_issue_id = NULL;

       //         convio_api_getAdvocacyInteractions     ( $cons_id, $list_record_offset ='' , $list_page_offset ='' , $list_page_size = '', $issue_id = '',
       //                                 $format='' , $server='' , $apikey='' , $account='' , $pw=''  )
        $string = convio_api_getAdvocacyInteractions_call($test_id , $test_list_record_offset , $test_list_page_offset, $test_page_size, $test_issue_id,
                                         $test_format , $test_server , $test_apikey , $test_account , $test_pw );
        drupal_set_message('<b>getAdvocacyInteractions():</b>');
        drupal_set_message( $string);
        // make the call
        $test_result = convio_api_getAdvocacyInteractions($test_id , $test_list_record_offset , $test_list_page_offset, $test_page_size, $test_issue_id,
                                         $test_format , $test_server , $test_apikey , $test_account , $test_pw);
        if ($test_result[0] != FALSE) {
            drupal_set_message('<b>Response:</b>');
            $test_format == 'xml' ? $response = '<pre>'. htmlentities($test_result[0]) .'</pre>' : $response = $test_result[0];
            drupal_set_message($response);
        }
        else {
            drupal_set_message( $test_result[1] , 'error');
        };
    break;

    default :
      drupal_set_message($test_api .' has not yet been implemented', 'warning');
    break;

  }
}
