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
 * Redirects to COM registration page.
 */
function convio_api_user_signup() {
  $data = ConvioConfiguration::getInstance();
  $url = $data->getURI() . ConvioConstants::$SIGNUP_SERVLET;

  drupal_goto($url);
}
