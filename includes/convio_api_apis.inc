<?php





function _CONSTITUENT_API() {
  // ONLY FOR THE OUTLINER
}

function convio_api_create_call( $primary_email, $add_center_ids="" , $add_group_ids="" , $add_interest_ids='' , $source='' , $no_welcome='' ,
                                 $format='', $server='' , $apikey='' , $account='' , $pw='' ) {
   // CAPI 1

    /* TODO need form input:
     *
     * https://secure2.convio.net/organization/site/SRConsAPI?method=create
     * & api_key =value
     * [ & response_format =xml |json ]
     * & v =value [ & suppress_response_codes =value ]
     * & login_name =value & login_password =value
     * & primary_email =value
     * [ & add_center_ids =value ]
     * [ & add_group_ids =value ]
     * [ & add_interest_ids =value ]
     * [ & source =value ]
     * [ & no_welcome =value ]
    */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=create';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    if (!empty($primary_email)) {
          $v_primary_email = '&primary_email='. $primary_email;
        }
        else {
          return "ERROR: convio_api_create_call() - primary_email not provided";
        };
    !empty($add_center_ids) ? $v_add_center_ids = '&add_center_ids='. $add_center_ids : $v_add_center_ids = '';
    !empty($add_group_ids) ? $v_add_group_ids = '&add_group_ids='. $add_group_ids : $v_add_group_ids = '';
    !empty($add_interest_ids) ? $v_add_interest_ids = '&add_interest_ids='. $add_interest_ids : $v_add_interest_ids = '';
    !empty($source) ? $v_source ='&source='. $source : $v_source = '';
    !empty($no_welcome) ? $v_no_welcome = '&no_welcome'. $no_welcome : $v_no_welcome = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_primary_email . $add_center_ids . $add_group_ids . $add_interest_ids . $source . $no_welcome ;
    return "$string";
}

function convio_api_create( $primary_email, $add_center_ids="" , $add_group_ids="" , $add_interest_ids='' , $source='' , $no_welcome='' ,
                            $format='', $server='' , $apikey='' , $account='' , $pw='' ) {


    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification
    $api = convio_api_create_call( $primary_email, $add_center_ids, $add_group_ids, $add_interest_ids, $source, $no_welcome, $format, $server, $apikey , $account , $pw);
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}


function convio_api_createOrUpdate_call( $primary_email='' , $cons_id='' , $member_id='' ,
                                         $add_center_ids='' , $add_group_ids='' , $add_interest_ids='' ,
                                         $remove_center_ids='' , $remove_group_ids='' , $remove_interest_ids='',
                                         $source='' , $no_welcome='' ,
                                         $format='', $server='', $apikey='', $account='', $pw='' ) {
   // CAPI 2
  $item = 'convio_api_createOrUpdate_call() called';

     /*
     * https://secure2.convio.net/organization/site/SRConsAPI?method=createOrUpdate
     * & api_key =value [ & response_format =xml |json ]
     * & v =value [ & suppress_response_codes =value ]
     * & login_name =value & login_password =value
     * [ & cons_id =value ]
     * [ & member_id =value ]
     * [ & primary_email =value ]
     * [ & add_center_ids =value ]
     * [ & add_group_ids =value ]
     * [ & add_interest_ids =value ]
     * [ & remove_center_ids =value ]
     * [ & remove_group_ids =value ]
     * [ & remove_interest_ids =value ]
     * [ & source =value ]
     * [ & no_welcome =value ]
    */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=createOrUpdate';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    !empty($cons_id) ? $v_cons_id = '&cons_id='. $cons_id : $v_cons_id = '';
    !empty($member_id) ? $v_member_id = '&member_id='. $member_id : $v_member_id = '';
    !empty($primary_email) ? $v_primary_email = '&primary_email='. $primary_email : $v_primary_email = '';
    !empty($add_center_ids) ? $v_add_center_ids = '&add_center_ids='. $add_center_ids : $v_add_center_ids = '';
    !empty($add_group_ids) ? $v_add_group_ids = '&add_group_ids='. $add_group_ids : $v_add_group_ids = '';
    !empty($add_interest_ids) ? $v_add_interest_ids = '&add_interest_ids='. $add_interest_ids : $v_add_interest_ids = '';
    !empty($remove_center_ids) ? $v_remove_center_ids = '&remove_center_ids='. $remove_center_ids : $v_remove_center_ids = '';
    !empty($remove_group_ids) ? $v_remove_group_ids = '&remove_group_ids='. $remove_group_ids : $v_remove_group_ids ='';
    !empty($remove_interest_ids) ? $v_remove_interest_ids = '&remove_interest_ids='. $remove_interest_ids : $v_remove_interest_ids = '';
    !empty($source) ? $v_source ='&source='. $source : $v_source = '';
    !empty($no_welcome) ? $v_no_welcome = '&no_welcome'. $no_welcome : $v_no_welcome = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_cons_id . $v_member_id . $v_primary_email . $v_add_center_ids
              . $v_add_group_ids . $v_add_interest_ids . $v_remove_center_ids
              . $v_remove_group_ids . $v_remove_interest_ids . $v_source . $v_no_welcome ;
    return "$string";
}

function convio_api_createOrUpdate( $primary_email='' , $cons_id='' , $member_id='' ,
                                    $add_center_ids='' , $add_group_ids='' , $add_interest_ids='' ,
                                    $remove_center_ids='' , $remove_group_ids='' , $remove_interest_ids='',
                                    $source='' , $no_welcome='' ,
                                    $format='', $server='', $apikey='', $account='', $pw='' ) {
  $item = 'convio_api_createOrUpdate() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification
    $api =  convio_api_createOrUpdate_call( $primary_email, $cons_id, $member_id,
                                         $add_center_ids, $add_group_ids, $add_interest_ids ,
                                         $remove_center_ids, $remove_group_ids, $remove_interest_ids,
                                         $source, $no_welcome,
                                         $format, $server, $apikey, $account, $pw );
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function convio_api_getGroupMembers_call( $group_id, $max_number='' , $fields='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
   // CAPI 3
  $item = 'convio_api_getGroupMembers_call() called';

    /* https://secure2.convio.net/organization/site/SRConsAPI?method=getGroupMembers
     * & api_key =value
     * [ & response_format =xml |json ]
     * & v =value [ & suppress_response_codes =value ]
     * & login_name =value & login_password =value
     * & group_id =value
     * [ & max_number =value ]
     * [ & fields=value ]
    */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=getGroupMembers';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    if (!empty($group_id)) {
      $v_group_id = '&group_id='. $group_id;
      }
      else {
       return "ERROR: convio_api_getGroupMembers_call() - group_id not provided";
      }
    !empty($max_number) ? $v_max_number = '&max_number='. $max_number : $v_max_number = '';
    !empty($fields) ? $v_fields = '&fields='. $fields : $v_fields = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_group_id . $v_max_number . $v_fields ;
    return "$string";
}

function convio_api_getGroupMembers( $group_id, $max_number='' , $fields='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // TODO
  $item = 'convio_api_getGroupMembers() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification
    $api = convio_api_getGroupMembers_call( $group_id, $max_number , $fields , $format , $server , $apikey , $account , $pw ); // update when copying
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function convio_api_getUser_call( $consid='' , $email='' , $format='' , $server='' , $apikey='' , $account='' , $pw='', $fields='' ) {
  // CAPI 4
  $item = 'convio_api_getUser_call() called';

  // https://secure2.convio.net/organization/site/SRConsAPI?method=getUser & api_key=value
  // [ & response_format =xml |json ] & v =value [ & suppress_response_codes =value ]
  // & login_name =value & login_password =value [ & cons_id =value ]
  // [ & member_id =value ] [ & primary_email =value ] [ & fields =value ]
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=getUser';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    if (!empty($consid)) {
      $v_consid = '&cons_id='. $consid;
      }
      else {
      $v_consid = '';
      }
    if (!empty($email)) {
      $v_email = '&primary_email='. urlencode($email); // Fight the good fight against plus signs in emails
      }
      else {
      $v_email = '';
      }
    if (!empty($fields)) {
      $v_fields = '&fields=' . $fields;
    }
    else $v_fields = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_consid . $v_email . $v_fields ;
    return "$string";
}

/*
 * convio_api_getUser()
 *
 * This method returns a single constituent record.
 *
 * @param type $consid
 *  Optional. The constituent ID (Convio ID) that uniquely identifies the
 *  constituent. At least one unique identifier parameter is required.
 * @param type $email
 *  Optional. The primary email address that uniquely identifies the
 *  constituent. At least one unique identifier parameter is required.
 * @param type $format
 *  Optional. Used to specify either XML or JSON formatting for the response.
 *  By default, uses setting from Convio Admin panel.
 * @param type $server
 *   Optional. The Convio server. By default, uses setting from
 *   Convio Admin panel.
 * @param type $apikey
 *   Optional. An arbitrary value that must be passed when invoking the Convio
 *   Client and Server APIs. The value passed by the caller must match the value
 *   in the CONVIO_API_KEY site configuration parameter, which is unique for
 *   each Convio site. By default, uses setting from Convio Admin panel.
 * @param type $account
 *   Optional. The user_name of the Convio administrative account that was
 *   created for API access. Note that this must be a special administrator
 *   account used just for API access. By defaul, uses setting from Convio Admin
 *   panel.
 * @param type $pw
 *   Optional. The password of the Convio administrative account that was
 *   created for API access. Note that this must be a special administrator
 *   account used just for API access. By defaul, uses setting from Convio Admin
 *   panel.
 * @return
 *   The requested user record in either XML or JSON format.
 *
 */

function convio_api_getUser( $consid='' , $email='' , $format='' , $server='' , $apikey='' , $account='' , $pw='', $fields='' ) {
    $item = 'convio_api_getUser() called';


    // place the call to the Convio server
    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification - TODO (is it ever turned on?)
    $getUser = convio_api_getUser_call(  $consid, $email, $format, $server, $apikey, $account, $pw, $fields);

    try {
         // Things that didn't work
         // Tried feed into SimpleXML object - error -  API requires POST
         //    $user = simplexml_load_file($getUser);
         // Tried using http post - error - requires HTTPS
         //   http_post_data($getUser);

         // try curl - create a new cURL resource and set to POST
        $ch = curl_init($getUser);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);
    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }
    return $result;

}

function convio_api_getUserGroups_call( $consid='' , $member_id="" , $primary_email='' , $format='' , $server='' , $apikey='' , $account='' , $pw=''  ) {
   // CAPI 5
  $item = 'convio_api_getUserGroups_call() called';

    /* https://secure2.convio.net/organization/site/SRConsAPI?method=getUserGroups
     * & api_key =value
     * [ & response_format =xml |json ]
     * & v =value [ & suppress_response_codes =value ]
     * & login_name =value & login_password =value
     * [ & cons_id =value ]
     * [ & member_id =value ]
     * [ & primary_email=value ]
    */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=getUserGroups';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    if (!empty($consid)) {
      $v_consid = '&cons_id='. $consid;
      }
      else {
      $v_consid = '';
      };

      if (!empty($member_id_email)) {
      $v_member_id = '&member_id='. $member_id;
      }
      else {
      $v_member_id = '';
      };

      if (!empty($primary_email)) {
      $v_primary_email = '&primary_email='. $primary_email;
      }
      else {
      $v_primary_email = '';
      };

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_consid . $_v_member_id . $v_primary_email ;
    return "$string";
}

function convio_api_getUserGroups( $consid='' , $member_id="" , $primary_email='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  $item = 'convio_api_getUserGroups() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification
    $api = convio_api_getUserGroups_call( $consid, $member_id, $primary_email, $format, $server, $apikey, $account, $pw ); // update when copying
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function convio_api_getUserInterests_call( $consid='' , $email='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // CAPI 6
  $item = 'convio_api_getUserInterest_call() called';


  // https://secure2.convio.net/organization/site/SRConsAPI?method=getUserInterests & api_key=value
  // [ & response_format =xml |json ] & v =value [ & suppress_response_codes =value ]
  // & login_name =value & login_password =value [ & cons_id =value ]
  // [ & member_id =value ] [ & primary_email =value ] [ & fields =value ]
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=getUserInterests';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    if (!empty($consid)) {
      $v_consid = '&cons_id='. $consid;
      }
      else {
      $v_consid = '';
      }
    if (!empty($email)) {
      $v_email = '&primary_email='. $email;
      }
      else {
      $v_email = '';
      }

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_consid . $v_email ;
    return "$string";
}

function convio_api_getUserInterests( $consid='' , $email='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  $item = 'convio_api_getUserIntersts() called';


    // place the call to the Convio server
    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_getUserInterests_call( $consid, $email, $format, $server, $apikey, $account, $pw );
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

/* https://secure2.convio.net/organization/site/SRConsAPI?method=getUserTransactions 
& api_key =value 
& login_name =value 
& login_password =value 
& v =value 
[ & center_id =value ] 
[ & response_format =xml |json ] 
[ & source =value ] 
[ & sub_source =value ] 
[ & suppress_response_codes =value ] 
[ & begin_date =value ] 
[ & cons_id =value ] 
[ & end_date =value ] 
[ & list_page_offset =value ] 
[ & list_page_size =value ] 
[ & list_record_offset =value ] 
[ & member_id =value ] 
[ & primary_email =value ] 
[ & transaction_type =DONATION |PLEDGE |RECURRING_GIFT |TICKET |DONATION_PENDING |DONATION_PENDING_ACCEPTED |DONATION_PENDING_REJECTED |DONATION_PENDING_ACCEPTED_FAILED |DONATION_PENDING_REJECTED_FAILED |DONATION_ACCEPTED_FAILED |DONATION_REJECTED_FAILED |DONATION_SETTLED |DONATION_REJECTED |F2F_REGISTRATION |TRIBUTES_REGISTRATION |ECOMMERCE_PURCHASE |REFUND |FAILED_RECURRING_DONATION_DECLINED |FAILED_RECURRING_DONATION_FRAUD ]  */

// TODO: Clean this up to respect the new parameters, etc.
function convio_api_getUserTransactions_call( $begin_date='', $consid='' , $end_date = '', $list_page_offset = '', $list_page_size = '', $list_record_offset = '', $member_id = '', $email='' , $transaction_type = '', $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // CAPI 6
  $item = 'convio_api_getUserTransaction_call() called';

  $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=getUserTransactions';
  $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
  empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
  $v_options = '&v=1.0&suppress_response_codes=FALSE';
  $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
  list($v_begin_date, $v_consid, $v_end_date, $v_list_page_offset, $v_list_page_size, $v_list_record_offset, $v_member_id, $v_email, $v_transaction_type) = array($begin_date, $consid, $end_date, $list_page_offset, $list_page_size, $list_record_offset, $member_id, $email, $transaction_type); // Blank out variables by default
  if (!empty($begin_date)) {
    $v_begin_date = '&begin_date='. $begin_date;
  }
  if (!empty($consid)) {
    $v_consid = '&cons_id='. $consid;
  }
  if (!empty($end_date)) {
    $v_end_date = '&end_date='. $end_date;
  }
  if (!empty($list_page_offset)) {
    $v_list_page_offset = '&list_page_offset='. $list_page_offset;
  }
  if (!empty($list_page_size)) {
    $v_list_page_size = '&list_page_size='. $list_page_size;
  }
  if (!empty($list_record_offset)) {
    $v_list_record_offset = '&list_record_offset='. $list_record_offset;
  }
  if (!empty($member_id)) {
    $v_member_id = '&member_id='. $member_id;
  }
  if (!empty($email)) {
    $v_email = '&primary_email='. $email;
  }
  if (!empty($transaction_type)) {
    $v_transaction_type = '&transaction_type='. $transaction_type;
  }

  // build URL string
  $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_begin_date . $v_consid . $v_end_date . $v_list_page_offset . $v_list_page_size . $v_list_record_offset . $v_member_id . $v_email . $v_transaction_type;
  return "$string";
}

function convio_api_getUserTransactions( $begin_date='', $consid='' , $end_date = '', $list_page_offset = '', $list_page_size = '', $list_record_offset = '', $member_id = '', $email='' , $transaction_type = '', $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  $item = 'convio_api_getUserTransactions() called';


    // place the call to the Convio server
    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_getUserTransactions_call( $begin_date, $consid, $end_date, $list_page_offset, $list_page_size, $list_record_offset, $member_id, $email, $transaction_type, $format, $server, $apikey, $account, $pw );
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function convio_api_isEmailValid_call( $email, $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // CAPI 7
  $item = 'convio_api_isEmailValid_call() called';


  /* https://secure2.convio.net/organization/site/SRConsAPI?method=isEmailValid
   * & api_key =value
   * [ & response_format =xml |json ]
   * & v =value [ & suppress_response_codes =value ]
   * & login_name =value &login_password=value
   * &email=value
  */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=isEmailValid';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    if (!empty($email)) {
      $v_email = '&email='. $email;
      }
      else {
      return "ERROR: convio_api_isEmailValid_call() - email not provided";
      }

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_email ;
    return "$string";
}

function convio_api_isEmailValid( $email, $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  $item = 'convio_api_isEmailValid() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_isEmailValid_call( $email, $format, $server, $apikey, $account, $pw );
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }
    return $result;
}

function convio_api_isUserInterested_call( $consid='' , $email='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // CAPI 8
  $item = 'convio_api_isUserInterested_call() called';


  /* https://secure2.convio.net/organization/site/SRConsAPI?method=isUserInterested
   * & api_key =value
   * [ & response_format =xml |json ]
   * & v =value [ & suppress_response_codes =value ]
   * & login_name=value &login_password=value
  */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=isUserInterested';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    if (!empty($consid)) {
      $v_consid = '&cons_id='. $consid;
      }
      else {
      $v_consid = '';
      }
    if (!empty($email)) {
      $v_email = '&primary_email='. $email;
      }
      else {
      $v_email = '';
      }

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_consid . $v_email ;
    return "$string";
}

function convio_api_isUserInterested( $consid='' , $email='' , $format='' , $server='' , $apikey='' , $account='' , $pw='') {
  $item = 'convio_api_isUserInterested() called';


    // place the call to the Convio server
    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_isUserInterested_call( $consid, $email, $format, $server, $apikey, $account, $pw  );
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function convio_api_listInterests_call( $include_hidden='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // CAPI 9
  $item = 'convio_api_listInterests_call() called';


  /* https://secure2.convio.net/organization/site/SRConsAPI?method=listInterests
   * & api_key =value
   * [ & response_format =xml |json ]
   * & v =value [ & suppress_response_codes =value ]
   * & login_name=value &login_password=value
   * [&include_hidden=value ]
   */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=listInterests';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    if (!empty($include_hidden)) {
      $v_include_hidden = '&include_hidden='. $include_hidden;
      }
      else {
      $v_include_hidden = '';
      };

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_include_hidden ;
    return "$string";
}

function convio_api_listInterests($include_hidden='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
    $item = 'convio_api_listIntersts() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification
    $api = convio_api_listInterests_call( $include_hidden, $format, $server, $apikey, $account, $pw ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }
    return $result;
}

function convio_api_listUserFieldChoices_call( $mfield , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // CAPI 10
  $item = 'convio_api_listUserFieldChoices_call() called';


  /* https://secure2.convio.net/organization/site/SRConsAPI?method=listUserFieldChoices
   * & api_key =value
   * [ & response_format =xml |json ]
   * & v =value [ & suppress_response_codes =value ]
   * &login_name=value &login_password=value
   * &field=value
   */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=listUserFieldChoices';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    if (empty($mfield)) {
      return "ERROR: convio_api_listUserFieldChoices_call() - field not provided";
      }
      else {
      $v_field = '&field='. $mfield;
      };

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_field ;
    return "$string";
}

function convio_api_listUserFieldChoices($mfield , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
    $item = 'convio_api_listUserFieldChoices() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification
    $api = convio_api_listUserFieldChoices_call( $mfield, $format, $server, $apikey, $account, $pw ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }
    return $result;
}

function convio_api_listUserFields_call( $access='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // CAPI 11
  $item = 'convio_api_listUserFields_call() called';


  /* https://secure2.convio.net/organization/site/SRConsAPI?method=listUserFields
   * & api_key =value
   * [ & response_format =xml |json ]
   * & v =value [ & suppress_response_codes =value ]
   * & login_name =value & login_password =value
   * [ & access =view |update ]
   */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=listUserFields';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    !empty($access) ? $v_access = '&access='. $access : $v_access = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_access ;
    return "$string";
}

function convio_api_listUserFields($access='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
    $item = 'convio_api_listUserFields() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification
    $api = convio_api_listUserFields_call( $access, $format, $server, $apikey, $account, $pw ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }
    return $result;
}

function convio_api_logInteraction_call( $primary_email='', $cons_id='', $member_id='',
                                         $interaction_subject , $interaction_body='', $interaction_cat_id='', $interaction_count='',
                                         $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // CAPI 12
  // TODO - returning Bad Request - Your browser sent a request that this server could not understand. - The request line contained invalid characters following the protocol string.
  // NOT handling spaces

  $item = 'convio_api_logInteraction_call called';


  /* https://secure2.convio.net/organization/site/SRConsAPI?method=logInteraction
   * & api_key =value
   * [ & response_format =xml |json ]
   * & v =value [ & suppress_response_codes =value ]
   * & login_name =value & login_password =value
   * [ & interaction_body =value ]
   * [ & interaction_cat_id =value ]
   * [ & interaction_count =value ]
   * & interaction_subject =value
   * [ & cons_id =value ]
   * [ & member_id =value ]
   * [ & primary_email =value ]
   */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=logInteraction';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    !empty($interaction_body) ? $v_interaction_body = '&interaction_body=\''. $interaction_body .'\'' : $v_interaction_body = '';  // this could take spaces and other special chars
    !empty($interaction_cat_id) ? $v_interaction_cat_id = '&interaction_cat_id='. $interaction_cat_id : $v_interaction_cat_id = '';
    !empty($interaction_count) ? $v_interaction_count = '&interaction_count='. $interaction_count : $v_interaction_count = '';

    if (empty($interaction_subject)) {
      return "ERROR: convio_api_logInteraction_call() - interaction_subject not provided";
      }
      else {
      $v_interaction_subject = '&interaction_subject=\''. $interaction_subject .'\''; // this could take spaces and other special chars
      };
    !empty($cons_id) ? $v_cons_id = '&cons_id='. $cons_id : $v_cons_id = '';
    !empty($member_id) ? $v_member_id = '&member_id='. $member_id : $v_member_id = '';
    !empty($primary_email) ? $v_primary_email = '&primary_email='. $primary_email : $v_primary_email = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_interaction_body . $v_interaction_cat_id . $v_interaction_count
                        . $v_interaction_subject . $v_cons_id . $v_member_id . $v_primary_email;
    return "$string";
}

function convio_api_logInteraction( $primary_email='', $cons_id='', $member_id='',
                                         $interaction_subject , $interaction_body='', $interaction_cat_id='', $interaction_count='',
                                         $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
    $item = 'convio_api_logInteraction() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification
    $api = convio_api_logInteraction_call( $primary_email, $cons_id, $member_id,
                                           $interaction_subject , $interaction_body, $interaction_cat_id, $interaction_count,
                                           $format , $server , $apikey , $account , $pw ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }
    return $result;
}

function convio_api_update_call( $primary_email='' , $cons_id='' , $member_id='' ,
                                         $add_center_ids='' , $add_group_ids='' , $add_interest_ids='' ,
                                         $remove_center_ids='' , $remove_group_ids='' , $remove_interest_ids='',
                                         $format='', $server='', $apikey='', $account='', $pw='' ) {
  // CAPI 13

  $item = 'convio_api_update_call called';


  /* https://secure2.convio.net/organization/site/SRConsAPI?method=update
   * & api_key =value
   * [ & response_format =xml |json ]
   * & v =value [ & suppress_response_codes =value ]
   * & login_name =value & login_password =value
   * [ & cons_id =value ]
   * [ & member_id =value ]
   * & primary_email =value
   * [ & add_center_ids =value ]
   * [ & add_group_ids =value ]
   * [ & add_interest_ids =value]
   * [ & remove_center_ids =value ]
   * [ & remove_group_ids =value ]
   * [ & remove_interest_ids =value ]
   *
   */
    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=update';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);
    !empty($cons_id) ? $v_cons_id = '&cons_id='. $cons_id : $v_cons_id = '';
    !empty($member_id) ? $v_member_id = '&member_id='. $member_id : $v_member_id = '';
    !empty($primary_email) ? $v_primary_email = '&primary_email='. $primary_email : $v_primary_email = '';
    !empty($add_center_ids) ? $v_add_center_ids = '&add_center_ids='. $add_center_ids : $v_add_center_ids = '';
    !empty($add_group_ids) ? $v_add_group_ids = '&add_group_ids='. $add_group_ids : $v_add_group_ids = '';
    !empty($add_interest_ids) ? $v_add_interest_ids = '&add_interest_ids='. $add_interest_ids : $v_add_interest_ids = '';
    !empty($remove_center_ids) ? $v_remove_center_ids = '&remove_center_ids='. $remove_center_ids : $v_remove_center_ids = '';
    !empty($remove_group_ids) ? $v_remove_group_ids = '&remove_group_ids='. $remove_group_ids : $v_remove_group_ids ='';
    !empty($remove_interest_ids) ? $v_remove_interest_ids = '&remove_interest_ids='. $remove_interest_ids : $v_remove_interest_ids = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_cons_id . $v_member_id . $v_primary_email
                        . $v_add_center_ids . $v_add_group_ids . $v_add_interest_ids . $v_remove_center_ids
                        . $v_remove_group_ids . $v_remove_interest_ids ;
    return "$string";
}

function convio_api_update( $primary_email='' , $cons_id='' , $member_id='' ,
                                         $add_center_ids='' , $add_group_ids='' , $add_interest_ids='' ,
                                         $remove_center_ids='' , $remove_group_ids='' , $remove_interest_ids='',
                                         $format='', $server='', $apikey='', $account='', $pw='') {
  $item = 'convio_api_update() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl verification
    $api =  convio_api_update_call( $primary_email, $cons_id, $member_id,
                                         $add_center_ids, $add_group_ids, $add_interest_ids ,
                                         $remove_center_ids, $remove_group_ids, $remove_interest_ids,
                                         $format, $server, $apikey, $account, $pw );
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function _SSO_API() {
  // ONLY FOR THE OUTLINER
}


function convio_api_authenticateUser_call( $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // SSOAPI 1
  $item = 'convio_api_authenticateUser_call() called';


/*
 * https://secure2.convio.net/organization/site/SRConsAPI?method=authenticateUser
 * & api_key =value
 * [ & response_format =xml |json ]
 * & v =value [ & suppress_response_codes =value ]
 * & login_name=value &login_password=value
 */

    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=authenticateUser';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin;
    return "$string";
}

function convio_api_authenticateUser( $format='' , $server='' , $apikey='' , $account='' , $pw='') {
  $item = 'convio_api_authenticateUser() called';


    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_authenticateUser_call( $format, $server, $apikey, $account, $pw  ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}


function convio_api_changePassword_call( $old_password, $user_password, $retype_password, $reminder_hint, $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // SSOAPI 2
  $item = 'convio_api_changePassword_call() called';

/*
 * https://secure2.convio.net/organization/site/SRConsAPI?method=changePassword
 * & api_key =value
 * [ & response_format =xml |json ]
 * & v =value [ & suppress_response_codes =value ]
 * & login_name=value &login_password=value
 * &old_password=value
 * &user_password=value
 * & retype_password=value
 * &reminder_hint=value
 */

    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=changePassword';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);

    if (empty($old_password)) {
      return "ERROR: convio_api_changePassword_call() - old_password not provided";
      }
      else {
      $v_old_password = '&old_password=\''. $old_password .'\''; //
      };

     if (empty($user_password)) {
      return "ERROR: convio_api_changePassword_call() - user_password not provided";
      }
      else {
      $v_user_password = '&user_password=\''. $user_password .'\''; //
      };

     if (empty($retype_password)) {
      return "ERROR: convio_api_changePassword_call() - retype_password not provided";
      }
      else {
      $v_retype_password = '&retype_password=\''. $retype_password .'\''; //
      };

     if (empty($reminder_hint)) {
      return "ERROR: convio_api_changePassword_call() - reminder_hint not provided";
      }
      else {
      $v_reminder_hint = '&reminder_hint=\''. urlencode($reminder_hint) .'\''; // this could take spaces and other special chars
      };

      //Check password for match and no spaces
     if (!($user_password == $retype_password)) {
      return "ERROR: convio_api_changePassword_call() - retype_password does not match user_password";
      } ;

      if (strstr($user_password, ' ')) {
      return "ERROR: convio_api_changePassword_call() - passwords can not contain spaces.";
      } ;

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_old_password . $v_user_password . $v_retype_password . $v_reminder_hint ;
    return "$string";
}

function convio_api_changePassword(  $old_password, $user_password, $retype_password, $reminder_hint, $format='' , $server='' , $apikey='' , $account='' , $pw='') {
  $item = 'convio_api_changePassword() called';


    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_changePassword_call( $old_password, $user_password, $retype_password, $reminder_hint, $format , $server , $apikey, $account, $pw  ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function convio_api_getSingleSignOnToken_call( $cons_id ='' , $member_id ='' , $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // SSOAPI 3
  $item = 'convio_api_getSingleSignOnToken_call() called';


/*
 * https://secure2.convio.net/organization/site/SRConsAPI?method=getSingleSignOnToken
 * & api_key =value
 * [ & response_format =xml |json ]
 * & v =value [ & suppress_response_codes =value ]
 * &login_name=value &login_password=value
 * [&cons_id=value ]
 * [&member_id=value ]
 *
 */

    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=getSingleSignOnToken';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);

    if (empty($cons_id) && empty($member_id)) {
      return "ERROR: convio_api_getSingleSignOnToken_call() - Either the cons_id or member_id must be provided.";
      };

    !empty($cons_id) ? $v_cons_id = '&cons_id='. $cons_id : $v_cons_id = '';
    !empty($member_id) ? $v_member_id = '&member_id='. $member_id : $v_member_id = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_cons_id . $v_member_id;
    return "$string";
}

function convio_api_getSingleSignOnToken(  $cons_id ='' , $member_id ='' , $format='' , $server='' , $apikey='' , $account='' , $pw='') {
  $item = 'convio_api_getSingleSignOnToken() called';


    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_getSingleSignOnToken_call($cons_id , $member_id , $format , $server , $apikey , $account , $pw ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function convio_api_loginTest_call( $success_redirect='',  $error_redirect='', $format='' , $server='' , $apikey='' ) {
  // SSOAPI 4
  $item = 'convio_api_loginTest_call() called';


/*
 * https://secure2.convio.net/organization/site/CRConsAPI?method=loginTest 
     &api_key=value 
     &v=value 
     [ &center_id=value ] 
     [ &error_redirect=value ] 
     [ &redirect=value ] 
     [ &response_format=xml | json ] 
     [ &sign_redirects=value ] 
     [ &source=value ] 
     [ &sub_source=value ] 
     [ &success_redirect=value ] 
     [ &suppress_response_codes=value ]
 *
 */

    $v_server = variable_get('convio_api_server', $server) .'/site/CRConsAPI?method=loginTest';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE&sign_redirects=true';
    $v_success_redirect = "&success_redirect=" . convio_api_build_redirect_url($success_redirect, array('absolute' => TRUE, 'query' => 'cons_id=${loginResponse/cons_id}'));
    $v_error_redirect = "&error_redirect=" . convio_api_build_redirect_url($error_redirect, array('absolute' => TRUE, 'query' => 'cons_id=0'));

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_success_redirect . $v_error_redirect;
    return "$string";
}

function convio_api_loginTest( $success_redirect='',  $error_redirect='', $format='' , $server='' , $apikey='' ) {
  $item = 'convio_api_loginTest() called';


    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_loginTest_call($success_redirect, $error_redirect,  $format , $server , $apikey); // call
    convio_api_redirect_http($api);
}

function convio_api_login_call( $user_name ='' , $password ='' , $send_user_name = '', $email = '', $logout = '',  $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // SSOAPI 4
  $item = 'convio_api_login_call() called';


/*
 * https://secure2.convio.net/organization/site/SRConsAPI?method=login
 * & api_key =value
 * [ & response_format =xml |json ]
 * & v =value [ & suppress_response_codes =value ]
 * & login_name =value & login_password =value
 * [ & user_name =value ]
 * [ & password =value ]
 * [ & send_user_name =value ]
 * [ & email =value ]
 * [ & logout =value ]
 *
 */

    $v_server = variable_get('convio_api_server', $server) .'/site/SRConsAPI?method=login';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);

    !empty($user_name) ? $v_user_name= '&user_name='. $user_name : $v_user_name = '';
    !empty($password) ? $v_password = '&password='. $password : $v_password = '';
    !empty($send_user_name) ? $v_send_user_name = '&send_user_name='. $send_user_name : $v_send_user_name = '';
    !empty($email) ? $v_email = '&email='. $email : $v_email = '';
    !empty($logout) ? $v_logout = '&logout='. $logout : $v_logout = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_user_name . $v_password . $v_send_user_name . $v_email . $v_logout;
    return "$string";
}

function convio_api_login( $user_name ='' , $password ='' , $send_user_name = '', $email = '', $logout = '',  $format='' , $server='' , $apikey='' , $account='' , $pw='') {
  $item = 'convio_api_login() called';


    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_login_call($user_name , $password, $send_user_name, $email, $logout,  $format , $server , $apikey , $account , $pw ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function convio_api_logout_call( $success_redirect='',  $error_redirect='', $format='' , $server='' , $apikey='' ) {
  // SSOAPI 4
  $item = 'convio_api_logout_call() called';


/*
 * https://secure2.convio.net/organization/site/CRConsAPI?method=logout 
     &api_key=value 
     &v=value 
     [ &center_id=value ] 
     [ &error_redirect=value ] 
     [ &redirect=value ] 
     [ &response_format=xml | json ] 
     [ &sign_redirects=value ] 
     [ &source=value ] 
     [ &sub_source=value ] 
     [ &success_redirect=value ] 
     [ &suppress_response_codes=value ]
 *
 */

    $v_server = variable_get('convio_api_server', $server) .'/site/CRConsAPI?method=logout';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE&sign_redirects=true';
    $v_success_redirect = "&success_redirect=" . convio_api_build_redirect_url($success_redirect, array('absolute' => TRUE));
    $v_error_redirect = "&error_redirect=" . convio_api_build_redirect_url($error_redirect, array('absolute' => TRUE));

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_success_redirect . $v_error_redirect;
    return "$string";
}

function convio_api_logout( $success_redirect='',  $error_redirect='', $format='' , $server='' , $apikey='' ) {
  $item = 'convio_api_logout() called';


    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_logout_call($success_redirect, $error_redirect,  $format , $server , $apikey); // call
    convio_api_redirect_http($api);
}

function _ADVOCACY_API() {
  // ONLY FOR THE OUTLINER
}

function convio_api_getAdvocacyAlerts_call( $list_record_offset ='' , $list_page_offset ='' , $issue_id = '', $alert_status, $alert_type, $publish = '' , $expire = '',
                                        $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // AAPI 1
  $item = 'convio_getAdvocacyAlerts_call() called';


/*
 * https://secure2.convio.net/organization/site/SRAdvocacyAPI?method=getAdvocacyAlerts
 * & api_key =value
 * [ & response_format =xml |json ]
 * & v =value [ & suppress_response_codes =value ]
 * & login_name =value & login_password =value
 * [ & list_record_offset =value ]
 * [ & list_page_offset =value ]
 * [ & list_page_size =value ]
 * [ & issue_id =value ]
 * & alert_status =DRAFT |SCHEDULED |ACTIVE |EXPIRED |ARCHIVED
 * & alert_type =ACTION |CALL |LTE |VOTE
 * [ &publish=value ]
 * [&expire=value ]
 *
 * TODO - BUG - SRAdvocacyAPI -> unknown servlet - SRConsAPI -> error code 6 - no method specified.
 */

    $v_server = variable_get('convio_api_server', $server) .'/site/SRAdvocacyAPI?method=getAdvocacyAlerts';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);

    !empty($list_record_offset) ? $v_list_record_offset= '&list_record_offset='. $list_record_offset : $v_list_record_offset = '';
    !empty($list_page_offset) ? $v_list_page_offset = '&list_page_offset='. $list_page_offset : $v_list_page_offset = '';
    !empty($issue_id) ? $v_issue_id = '&issue_id='. $issue_id : $v_issue_id = '';

    if (empty($alert_status)) {
      return "ERROR: convio_getAdvocacyAlerts_call() - alert_status not provided";
      }
      else {
      $v_alert_status = '&alert_status=\''. $alert_status .'\''; //
      };

    if (empty($alert_type)) {
      return "ERROR: convio_getAdvocacyAlerts_call() - alert_type not provided";
      }
      else {
      $v_alert_type = '&alert_type=\''. $alert_type .'\''; //
      };

    !empty($publish) ? $v_publish = '&publish='. $publish : $v_publish = '';
    !empty($expire) ? $v_expire = '&expire='. $expire : $v_expire = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_list_record_offset . $v_list_page_offset . $v_issue_id . $v_alert_status
                        . $v_alert_type . $v_publish . $v_expire ;
    return "$string";
}

function convio_api_getAdvocacyAlerts( $list_record_offset ='' , $list_page_offset ='' , $issue_id = '', $alert_status, $alert_type, $publish = '' , $expire = '',
                                        $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  $item = 'convio_api_getAdvocacyAlerts() called';


    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_getAdvocacyAlerts_call($list_record_offset , $list_page_offset , $issue_id, $alert_status, $alert_type, $publish , $expire,
                                        $format , $server , $apikey , $account , $pw ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

function convio_api_getAdvocacyInteractions_call( $cons_id, $list_record_offset ='' , $list_page_offset ='' , $list_page_size = '', $issue_id = '',
                                        $format='' , $server='' , $apikey='' , $account='' , $pw='' ) {
  // AAPI 2
  $item = 'convio_getAdvocacyInteractions_call() called';


/*
 * https://secure2.convio.net/organization/site/ SRAdvocacyAPI?method=getAdvocacyInteractions
 * & api_key =value
 * [ & response_format =xml |json ]
 * & v =value [ & suppress_response_codes =value ]
 * & login_name =value & login_password =value
 * [ & list_record_offset =value ]
 * [ & list_page_offset =value ]
 * [ & list_page_size=value ]
 * &cons_id=value
 * [&issue_id=value ]
 *
 * TODO - BUG - SRAdvocacyAPI -> unknown servlet - SRConsAPI -> error code 6 - no method specified.
 */

    $v_server = variable_get('convio_api_server', $server) .'/site/SRAdvocacyAPI?method=getAdvocacyInteractions';
    $v_apikey = '&api_key='. variable_get('convio_api_key', $apikey);
    empty($format) ? $v_format = '' : $v_format = '&response_format='. $format ;
    $v_options = '&v=1.0&suppress_response_codes=FALSE';
    $v_admin = '&login_name='. variable_get('convio_api_login_name', $account) .'&login_password='. variable_get('convio_api_login_password', $pw);

    !empty($list_record_offset) ? $v_list_record_offset= '&list_record_offset='. $list_record_offset : $v_list_record_offset = '';
    !empty($list_page_offset) ? $v_list_page_offset = '&list_page_offset='. $list_page_offset : $v_list_page_offset = '';
    !empty($list_page_size) ? $v_list_page_size = '&list_page_size='. $list_page_size : $v_list_page_size = '';

    if (empty($cons_id)) {
      return "ERROR: convio_api_getAdvocacyInteractions_call() - cons_id not provided";
      }
      else {
      $v_cons_id = '&cons_id=\''. $cons_id .'\'' ; //
      };

    !empty($issue_id) ? $v_issue_id = '&issue_id='. $issue_id : $v_issue_id = '';

    // build URL string
    $string = $v_server . $v_apikey . $v_format . $v_options . $v_admin . $v_list_record_offset . $v_list_page_offset
                        . $v_list_page_size . $v_cons_id . $v_issue_id ;
    return "$string";
}

function convio_api_getAdvocacyInteractions( $cons_id, $list_record_offset ='' , $list_page_offset ='' , $list_page_size = '', $issue_id = '',
                                        $format='' , $server='' , $apikey='' , $account='' , $pw=''  ) {
  $item = 'convio_api_getAdvocacyInteractions() called';

    $ca_verify = variable_get('convio_api_ca_verification', FALSE); // turn off ssl
    $api = convio_api_getAdvocacyInteractions_call($cons_id, $list_record_offset, $list_page_offset , $list_page_size , $issue_id,
                                        $format , $server, $apikey , $account , $pw ); // call
    try {
         // create a new cURL resource and set to POST
        $ch = curl_init($api);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE); // check
        if ($ca_verify == FALSE) curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // stop cURL from verifying the peer's certificate
        // grab URL and pass it to the browser
        $result[0] = curl_exec($ch);
        // close cURL resource, and free up system resources
        if ($result[0] == FALSE) {
           $httpcode = convio_api_http_status_code_string(curl_getinfo($ch, CURLINFO_HTTP_CODE), TRUE);
           $result[1] = 'CONNECTION ERROR - cURL: '. curl_errno($ch) .' '. curl_error($ch) .', HTTP: '. $httpcode;
        }
        else {
           $result[1] = NULL;
        }
        curl_close($ch);

    } catch (Exception $result) {
        drupal_set_message($result->getMessage(), 'error');
        watchdog('convio_api', 'Error: ', $result->getMessage(), 'WATCHDOG_ERROR');
    }

    return $result;
}

/**
 * Build a Convio Client API redirect URL (often used for success_redirect and error_redirect parameters) that takes the current destination into account.
 */
function convio_api_build_redirect_url($url, $parameters) {
  if (isset($_REQUEST['destination'])) {
    $destination = $_REQUEST['destination'];
  }
  else if (isset($_REQUEST['edit']['destination'])) {
    $destination = $_REQUEST['edit']['destination'];
  }

  if ($destination) {
    $url_parameters['destination'] = $destination;
  }

  return url($url, $parameters);
}

/**
 * Redirect to the Convio API, accounting for the fact that there may be a destination set and making sure it is preserved, but not redirected to yet.
 */
function convio_api_redirect_http($url) {
  if (isset($_REQUEST['destination'])) {
    $destination = $_REQUEST['destination'];
  }
  else if (isset($_REQUEST['edit']['destination'])) {
    $destination = $_REQUEST['edit']['destination'];
  }

  if ($destination) {
    unset($_REQUEST['destination'], $_REQUEST['edit']['destination']);
  }

  drupal_goto($url);
}

