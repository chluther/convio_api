<?php
// $Id$

/**
 * @file
 * This file allows the Convio consultant to make deployment-specific
 * modifications by defining token pairs. For more detailed on the format of
 * this token array please see the Convi API module documentation
 * @see http://drupal.org/node/??
 *
 * For additional informaiton on the Token module API please see
 * @see http://drupal.org/node/307140
 */
//  General Constants
  global $CR_CONS_API;
  $CR_CONS_API = "/CRConsAPI";
  global $SR_CONS_API;
  $SR_CONS_API = "/SRConsAPI";
  global $LOGIN_SERVLET;
  $LOGIN_SERVLET = "/UserLogin";
  global $SIGNUP_SERVLET;
  $SIGNUP_SERVLET = "/ConsProfileUser";
  global $CONS_API_VERSION;
  $CONS_API_VERSION = "1.0";
  global $SUPER_USER_ID;
  $SUPER_USER_ID = 1000000;
  // Stores the C360 cons_id for user session synching.
  //global $SESSION_CONS_ID;
  //$SESSION_CONS_ID = "convio_cons_id";


// Implementation Specific Global Variables
  global $CONS_SERVER;
  $CONS_SERVER = "http://www.aidwellbeing.org/site/";
  //$CONS_SERVER = variable_get('convio_api_server', $server) ;
  global $CONS_DONATE;
  $CONS_DONATE = "/PageServer?pagename=donate_support";
  global $usage_terms_text;
  $usage_terms_text = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
            sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
            At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata
            sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
            sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
            At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est
            Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor
            invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores
            et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
            <br>
            Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum
            dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit
            praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.
            Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod
            tincidunt ut laoreet dolore magna aliquam erat volutpat.
            ";
  global $usage_terms_checkbox_text;
  $usage_terms_checkbox_text = "I have read and agree to the 'Terms of Agreement' above.";

/**
 * convio_api_fields($farray)
 * implementation specific Convio to Drupal field mapping to support download
 * a subset of data from Convio to Drupal.
 * The value on the left is the field name in drupal (user profile fields) and
 * the value on the right is the path to the value in the Convio API xml
 * response.
 *
 */
  function convio_api_fields($farray, $xml) {
            if(is_array($farray)) {
             $farray['name'] =                     (string)$xml->user_name;
             $farray['convio_name_title'] =        (string)$xml->name->title;
             $farray['convio_name_first'] =        (string)$xml->name->first;
             $farray['convio_name_middle'] =       (string)$xml->name->middle;
             $farray['convio_name_last'] =         (string)$xml->name->last;
             $farray['convio_name_suffix'] =       (string)$xml->name->suffix;
             $farray['convio_name_prof_suffix'] =  (string)$xml->name->prof_suffix;
             $farray['convio_preferred_phone'] =   (string)$xml->preferred_phone;
             $farray['convio_home_phone'] =        (string)$xml->home_phone;
             $farray['convio_mobile_phone'] =      (string)$xml->mobile_phone;
             // $farray['convio_work_phone'] =        (string)$xml->work_phone; // KK: Not in Convio profile fields; removed
             // $farray['convio_primary_address'] =   (string)$xml->primary_address; // KK: Makes no sense...this is a container.
             $farray['convio_home_street1'] =      (string)$xml->primary_address->street1;
             $farray['convio_home_street2'] =      (string)$xml->primary_address->street2;
             $farray['convio_home_city'] =         (string)$xml->primary_address->city;
             $farray['convio_home_zip'] =          (string)$xml->primary_address->zip;
             $farray['convio_home_country'] =      (string)$xml->primary_address->country;
             $farray['convio_member_id'] =         (string)$xml->member_id;
             $farray['convio_cons_id'] =           (string)$xml->cons_id;
             $farray['convio_employer'] =          (string)$xml->employment->employer;
             $farray['convio_occupation'] =        (string)$xml->employment->occupation;
             // $farray['convio_membership_type'] =   (string)$xml->membership_level_id; // Not working (Kaland) - need to figure out how to deal with this

             return($farray);
            } else {
              return(FALSE);
            }
  }

/**
 * Convio API implementation of hook_token_values().
 * @param type
 *   A flag indicating the class of substitution tokens to use. If an
 *   object is passed in the second param, 'type' should contain the
 *   object's type. For example, 'node', 'comment', or 'user'. If your
 *   implemention of the hook inserts globally applicable tokens that
 *   do not depend on a particular object, it should only return values
 *   when $type is 'global'.
 * @param object
 *   Optionally, the object to use for building substitution values.
 *   A node, comment, user, etc.
 * @return
 *   A keyed array containing the substitution tokens and the substition
 *   values for the passed-in type and object.
 */
function convio_api_token_values($type, $object = NULL) {
  // TODO create an array of values which are processed by module
  $item = 'convio_api_token_values() called';
  firep($item);

  global $user;
  $values = array();
  switch ($type) {
    case 'global':
        $values['global_test'] = 'global test good';
        $values['now_long'] =  date("D M j G:i:s");
        $values['today']  = date("m.d.y");
    break;

    case 'node':
        $values['node_test'] = 'node test good';
    break;

    case 'user':
        $values['drupal_username']     = $user -> name;
        $values['convio_user_name']   = $user -> convio_user_name;
        $values['cons_id']            = $user -> convio_cons_id;
        $values['name_title']         = $user -> convio_name_title ? $user -> convio_name_title : $user -> profile_name_title ;
        $values['name_first']         = $user -> convio_name_first ? $user -> convio_name_first : $user -> profile_name_first ;
        $values['name_middle']        = $user -> convio_name_middle ? $user -> convio_name_middle : $user -> profile_name_middle ;
        $values['name_last']          = $user -> convio_name_last ? $user -> convio_name_last : $user -> profile_name_last ;
        $values['name_suffix']        = $user -> convio_name_suffix ? $user -> convio_name_suffix : $user -> profile_name_suffix;
        $values['name_prof_suffix']   = $user -> convio_name_prof_suffix ? $user -> convio_name_prof_suffix : $user -> profile_name_prof_suffix ;
        $values['accepts_postal_mail'] = $user -> convio_accepts_postal_mail ? $user -> convio_accepts_postal_mail : $user -> profile_accepts_postal_mail;
        $values['number_id']           = $user -> convio_number_id;
        $values['preferred_phone']        =  $user -> convio_preferred_phone;
        $values['home_phone']             =  $user -> convio_home_phone;
        $values['mobile_phone']           =  $user -> convio_mobile_phone;
        $values['email_primary_address']  =  $user -> convio_email_primary_address;
        $values['work_phone']               =  $user -> convio_work_phone;
        $values['home_city']                =  $user -> convio_home_city;
        $values['home_country']             =  $user -> convio_home_country;
        $values['home_state']               = $user -> convio_home_state;
        $values['home_street1']             = $user -> convio_home_street1;
        $values['home_street2']             = $user -> convio_home_street2;
        $values['home_zip']                 = $user -> convio_home_zip;
        $values['interest']                 = $user -> convio_interest_label;

     break;

  }
  return $values;
}

/**
 * Implementation of hook_token_list().
 * This function is used to provide help and inline documentation for all
 * of the possible replacement tokens.
 * TBD - move to convio_api_custom.inc
 */
function convio_api_token_list($type = 'all') {
  firep('convio_api_token_list() called');
  if ($type == 'global' || $type == 'all') {

  }
  if ($type == 'node' || $type == 'all') {
    // Node tokens here.

  }
  if ($type == 'user' || $type == 'all') {
    // user tokens here.
  }

  return $tokens;
}

function convio_api_transactions($xml) {
  // TODO: Write this function
  // Need to sync transactions to appropriate userpoint table locations. In the case of PDFs, convio_transaction_items should be updated. We may need a table to store PDF purchases down the line...but there are only 5, so I think the current way is fine.

  // KLUDGE: The following stuff shouldn't be in Convio API really, but doing otherwise needs some thought we don't have time for right now
  $farray = array();
  $farray['convio_transaction_items'] = '';
  // TODO: Actually get the transactions
  $transactions = array();
  foreach ($transactions as $tid => $transaction) {
    // TODO: Finish this off
  }
  return $farray;
}

