<?php
/**
 * Copyright (C) 2010 Convio, Inc. All rights reserved.
 *
 * This program is free open source software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version. The GNU General Public License is available at
 * http://www.gnu.org/licenses/.
 *
 * =====================================================
 *
 * @file Single-sign-on (SSO) validation
 * @version $Id: authenticate.inc,v 1.1 2010/01/26 00:13:03 mpih Exp $
 */

/**
 * Menu callback; Validates the SSO auth token and logs-in the current user.
 */
function convio_api_session_status() {

  $api_config = ConvioConfiguration::getInstance();
  $api_uri = $api_config->getURI();


  $cons_id = 0;
  if (isset($_SESSION[ConvioConstants::$SESSION_CONS_ID])) {
    $cons_id = $_SESSION[ConvioConstants::$SESSION_CONS_ID];
  }

  $reset_url = url('convio_api/session/clear', array('query' => drupal_get_destination()));


  drupal_set_header('Content-Type: text/javascript; charset=UTF-8');

  // Prevent caching of the dynamic JS.
  drupal_set_header('Pragma: No-Cache');
  drupal_set_header('Cache-control: private,no-cache,no-store,max-age=0,must-revalidate');
  drupal_set_header('Expires: -1');

  print "
/**
 * Authentication beacon used to keep Convio and Drupal sessions in synch.
 */
(function() {
  var doUpdateSession = function(o,s) {
    var cons_id = " . $cons_id . "; // The expected cons ID.
    var reset_url = '" . $reset_url . "';

    var reset = false;
    o = o || {};
    if (o.loginResponse) {
      if (cons_id != o.loginResponse.cons_id) {
        // User is logged in, but sessions are out of sync.
        reset = true;
      }
    } else if (o.errorResponse && o.errorResponse.code == 204) {
      if (cons_id != 0) {
        // User should not be logged in, but sessions are out of sync.
        reset = true;
      }
    } else {
      // Unrecognized response.
      throw 'Unrecognized response: json=' + o + '; status=' + s;
    }

    if (reset) {
      // Reset the session.
      window.location.href = reset_url;
      return;
    }
  };

  CONVIO.OPEN.callConsAPI('loginTest', doUpdateSession, null);
})();
";

  return;
}
