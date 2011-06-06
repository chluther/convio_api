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
 * Validates the SSO auth token and establishes the Drupal user session.
 */
function convio_session_validate() {

  // Validate that the API response actually came from Convio.
  $loginHandler = ConvioLoginHandler::getInstance();
  if (! $loginHandler->checkSignature()) {
    watchdog("convio", "Failed to authenticate user. Invalid signature", array(), WATCHDOG_WARNING);
    drupal_goto(url("user/login", 
                    array("absolute" => TRUE,
                          "query" => drupal_get_destination())));
    return;
  }

  if (isset($_GET["cons_id"])) {
    // Establish authenticated Drupal user session.
    $loginHandler->authenticate($_GET["cons_id"]);
  } else if (isset($_GET["code"]) && $_GET["code"] == 204) {

    if (isset($_SESSION[ConvioConstants::$SESSION_CONS_ID])) {
      // Establish anonymous Drupal user session.
      $loginHandler->logout(TRUE);
    }
  }

  drupal_goto();
  return;
}
