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
 * @version $Id: authenticate.inc,v 1.1 2010/01/26 00:13:03 mpih Exp $
 */

/**
 * Clears the Drupal session.
 */
function convio_api_session_clear() {

  // Clear the Drupal session. It needs to be re-established.
  unset($_SESSION[ConvioConstants::$SESSION_CONS_ID]);

  ConvioLoginHandler::logout(TRUE);
  return;
}
