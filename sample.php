<?php
// $Id: user_import.module,v 1.26 2009/09/20 11:58:10 MegaGrunt Exp $

// Update options for existing users
define ('UPDATE_NONE', 0);
define ('UPDATE_REPLACE', 1);
define ('UPDATE_ADD', 2);

/**
 * @file
 * Import or update users with data from a comma separated file (csv).
 */

/**
 * - - - - - - - -  HOOKS - - - - - - - -
 */

/**
 * Implementation of hook_theme().
 */
function user_import_theme() {
  return array(
    'user_import_list' => array(
      'arguments' => array(),
    ),
    'user_import_edit' => array(
      'arguments' => array('form' => NULL),
    ),
    'user_import_errors_display' => array(
      'arguments' => array('settings' => NULL, 'data' => NULL, 'total' => NULL),
    ),
    'user_import_username_errors' => array(
      'arguments' => array('errors' => NULL),
    ),
  );
}

/**
 * Implementation of hook_help().
 */
function user_import_help($path, $arg) {
  switch ($path) {
    case 'admin/user/user_import':
      return t("Import or update users from a comma separated file (csv). Click 'Import' to start a new import.");
  }
}

/**
 * Implementation of hook_perm().
 */
function user_import_perm() {
  return array('import users');
}

/**
 * Implementation of hook_menu().
 */
function user_import_menu() {
  $items['admin/user/user_import'] = array(
      'title' => 'User Imports',
      'description' => 'Import or update users from a comma separated file (csv).',
      'page callback' => 'user_import_list',
      'access arguments' => array('import users'),
      );
  $items['admin/user/user_import/list'] = array(
      'title' => 'List Imports',
      'access arguments' => array('import users'),
      'weight' => -10,
      'type' => MENU_DEFAULT_LOCAL_TASK
      );
  $items['admin/user/user_import/add'] = array(
    'title' => 'Import',
    'page callback' => 'user_import_preferences',
    'access arguments' => array('import users'),
    'weight' => -5,
    'type' => MENU_LOCAL_TASK
  );
  $items['admin/user/user_import/continue'] = array(
    'title' => 'Continue',
    'page callback' => 'user_import_continue',
    'access arguments' => array('import users'),
    'type' => MENU_CALLBACK
  );
  $items['admin/user/user_import/import'] = array(
    'title' => 'Import',
    'page callback' => 'user_import_import',
    'access arguments' => array('import users'),
    'type' => MENU_CALLBACK
  );
  $items['admin/user/user_import/delete'] = array(
    'title' => 'Delete Import',
    'page callback' => 'user_import_delete',
    'access arguments' => array('import users'),
    'type' => MENU_CALLBACK
  );
  $items['admin/user/user_import/configure'] = array(
    'title' => 'Configure',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('user_import_configure_form'),
    'access arguments' => array('import users'),
    'type' => MENU_LOCAL_TASK
  );
  $items['user_import/delete'] = array(
    'title' => 'Remove Info',
    'page callback' => 'user_import_limited_delete',
    'type' => MENU_CALLBACK,
    'access arguments' => array('limited user import'),
  );
  $items['user_import/errors'] = array(
    'title' => 'Import Errors',
    'page callback' => 'user_import_limited_errors',
    'type' => MENU_CALLBACK,
    'access arguments' => array('limited user import'),
  );

  return $items;
}

/**
 * Implementation of hook_cron().
 */
function user_import_cron() {

    $imports = _user_import_settings_select();
    if (!$imports) return;

    foreach ($imports as $import) {

        if ($import['setting'] == 'test' || $import['setting'] == 'import') _user_import_process($import);
    }

    return;
}

// - - - - - - - -  FORMS - - - - - - - -

/**
 * Configuration form define (settings affect all user imports)
 */
function user_import_configure_form() {

    $form['performance'] = array(
        '#type' => 'fieldset',
        '#title' => t('Performance'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );

    $form['performance']['user_import_max'] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum Users/Process'),
        '#default_value' => variable_get('user_import_max', 250),
        '#size' => 10,
        '#maxlength' => 10,
        '#description' => t('Maximum number of users to import each time the file is processed, useful for controling the rate at which emails are sent out.'),
    );

    $form['performance']['user_import_line_max'] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum length of line'),
        '#default_value' =>  variable_get('user_import_line_max', 1000),
        '#size' => 10,
        '#maxlength' => 10,
        '#description' => t('The default is set at 1,000 characters, if a line in your csv is longer than this you should set a higher maximum here. Setting higher maximums will slow down imports.'),
    );


    $saved_templates = _user_import_settings_select(NULL, 'GET TEMPLATES');

    if (!empty($saved_templates)) {

        $form['settings_templates'] = array(
            '#type' => 'fieldset',
            '#title' => t('Settings Templates'),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
        );

        $templates_list = array('-- none --');

        foreach ($saved_templates AS $template) {
            $templates_list[$template['import_id']] = $template['name'];
            $templates_delete[$template['import_id']] = $template['name'];
        }

        $form['settings_templates']['user_import_settings'] = array(
            '#type' => 'select',
            '#title' => t('Default Settings'),
            '#description' => t('Select if you want to use a previously saved set of settings as default for all imports.'),
            '#default_value' => variable_get('user_import_settings', 0),
            '#options' => $templates_list,
        );


        $form['settings_templates']['templates'] = array(
            '#type' => 'checkboxes',
            '#title' => t('Delete Templates'),
            '#options' => $templates_delete,
        );

    }

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        );

    return $form;
}

function user_import_configure_form_validate($form, &$form_state) {

    if (is_numeric($form_state['values']['user_import_max'])) {
        if ($form_state['values']['user_import_max'] < 10) form_set_error('user_import_max', t("Value should be at least 10."));
    }
    else {
        form_set_error('user_import_max', t('Value must be a number.'));
    }

    if (is_numeric($form_state['values']['user_import_line_max'])) {
        if ($form_state['values']['user_import_line_max'] < 1000) form_set_error('user_import_line_max', t("Value must be higher than 1000."));
        if ($form_state['values']['user_import_line_max'] > 1000000) form_set_error('user_import_line_max', t("Value must be lower than 1,000,000."));
    }
    else {
        form_set_error('user_import_line_max', t('Value must be a number.'));
    }

    return;
}

function user_import_configure_form_submit($form, &$form_state) {

  settype($form_state['values']['user_import_max'], 'integer');
  settype($form_state['values']['user_import_line_max'], 'integer');
  variable_set('user_import_max', $form_state['values']['user_import_max']);
  variable_set('user_import_line_max', $form_state['values']['user_import_line_max']);
  variable_set('user_import_settings', $form_state['values']['user_import_settings']);

  if (!empty($form_state['values']['templates'])) {

      foreach ($form_state['values']['templates'] as $import_id) {

          if (!empty($import_id)) {

              $template = _user_import_settings_select($import_id);
              if (!empty($deleted)) $deleted .= ', ';
              $deleted .= $template['name'];
              _user_import_settings_deletion($import_id);
          }
      }
  }

  if (!empty($deleted)) drupal_set_message(t('Settings templates deleted: @deleted', array('@deleted' => $deleted)));

  drupal_set_message(t('Configuration settings have been saved.'));
  $form_state['redirect'] = 'admin/user/user_import';
}

function user_import_add_form($import_id = NULL) {

    $form = array();
    $ftp_files = _user_import_ftp_files();
    user_import_add_file_form($form, $ftp_files);

    $settings = _user_import_settings_select(NULL, 'get saved');

    if ($settings) {

        $saved_settings = array(t('-- none --'));
        foreach ($settings AS $settings_set) {
            $saved_settings[$settings_set['import_id']] = $settings_set['name'];
        }

        $form['import_template_select'] = array(
            '#type' => 'select',
            '#title' => t('Saved Settings'),
            '#description' => t('Select if you want to use a previously saved set of settings.'),
            '#default_value' => variable_get('user_import_settings', 0),
            '#options' => $saved_settings,
        );

    }

    $form['next'] = array(
        '#type' => 'submit',
        '#value' => t('Next')
    );

    // Set form parameters so we can accept file uploads.
    $form['#attributes'] = array('enctype' => 'multipart/form-data');

    return $form;
}

function user_import_add_form_validate($form, &$form_state) {

  $file = _user_import_file(NULL, $form_state['values']['file_ftp']);

  // check file uploaded OK
  if (empty($file->filename)) form_set_error('file_upload', t('A file must be uploaded or selected from FTP updates.'));

  /**
   * @todo check file matches saved settings selected
   */

  return;
}

function user_import_add_form_submit($form, &$form_state) {

  $file = _user_import_file(NULL, $form_state['values']['file_ftp']);
  $form_state['values']['options']['ftp'] = empty($form_state['values']['file_ftp']) ? FALSE : TRUE;
  $form_state['values']['filename'] = $file->filename;
  $form_state['values']['oldfilename'] = $file->filename;
  $form_state['values']['filepath'] = $file->filepath;
  $form_state['values']['setting'] = 'file set';

  // create import setting
  $import = _user_import_settings_save($form_state['values']);
  if (!empty($form_state['values']['import_template_select'])) $settings_template = check_plain($form_state['values']['import_template_select']);

  $form_state['redirect'] = 'admin/user/user_import/add/'. $import['import_id'] .'/'. $settings_template;
}

function user_import_edit($form_state, $import_id, $template_id = NULL) {

  // load code for supported modules
  user_import_load_supported();

  $form = array();
  $import = _user_import_settings_select($import_id);
  $import['template_id'] = $template_id;

  $form['ftp'] = array(
      '#type' => 'value',
      '#value' => $import['options']['ftp'],
  );

  // add setting template values
  if ($import['setting'] == 'file set') $import = _user_import_initialise_import($import);

  $form['import_id'] = array(
      '#type' => 'value',
      '#value' => $import_id,
  );

  $form['setting'] = array(
      '#type' => 'value',
      '#value' => $import['setting'],
  );

  $form['return_path'] = array(
      '#type' => 'value',
      '#default_value' => 'admin/user/user_import',
  );

  $form['og_id'] = array(
      '#type' => 'value',
      '#default_value' => 0,
  );

  // don't use hook because these need to be added in this order;
  user_import_edit_file_fields($form, $import);
  user_import_form_field_match($form, $import);

  $collapsed = (empty($import['name'])) ? FALSE: TRUE;
  $additional_fieldsets = module_invoke_all('user_import_form_fieldset', $import, $collapsed);
  if (is_array($additional_fieldsets)) $form = $form + $additional_fieldsets;

  $update_user = module_invoke_all('user_import_form_update_user');

  if (is_array($update_user)) {

    $form['update_user'] = array(
      '#type' => 'fieldset',
      '#title' => t('Update Existing Users'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    foreach ($update_user as $module => $display) {

      $options = array(UPDATE_NONE => t('No Update'), UPDATE_REPLACE => t('Replace Data'), UPDATE_ADD => t('Add Data'));
      if ($display['exclude_add'] == TRUE) unset($options[UPDATE_ADD]);
      if ($display['exclude_replace'] == TRUE) unset($options[UPDATE_REPLACE]);

      $form['update_user'][$module] = array(
        '#type' => 'radios',
        '#title' => $display['title'],
        '#options' => $options,
        '#default_value' => empty($import['options']['update_user'][$module]) ? UPDATE_NONE : $import['options']['update_user'][$module],
        '#description' => $display['description'],
      );
    }
  }

  // don't show test option if import has started
  if ($import['setting'] != 'import' && $import['setting'] != 'imported') {

    $form['test'] = array(
        '#type' => 'submit',
        '#value' => t('Test'),
        '#weight' => 100,
        '#submit' => array('user_import_test_submit', 'user_import_edit_submit'),
    );
  }

  $form['import'] = array(
      '#type' => 'submit',
      '#value' => t('Import'),
      '#weight' => 100,
            '#submit' => array('user_import_import_submit', 'user_import_edit_submit'),
  );

  $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#weight' => 100,
      '#validate' => array('user_import_edit_cancel_validate'),
  );

  return $form;
}

function user_import_edit_cancel_validate($form, &$form_state) {
  // if import was being added - delete file
  if ($form_state['values']['setting'] == 'file set') {
      $settings = _user_import_settings_select($form_state['values']['import_id']);
      _user_import_settings_deletion($form_state['values']['import_id']);
      _user_import_file_deletion($settings['filepath'], $settings['filename'], $settings['oldfilename'], $settings['options']['ftp']);
  }

  $form_state['redirect'] = 'admin/user/user_import';
}

function user_import_edit_validate($form, &$form_state) {

  $fields = array();

  foreach ($form_state['values']['field_match'] as $row => $values) {

    // check each field is unique
    if ($values['field_match'] != '0' && $values['field_match'] != '-------------' && in_array($values['field_match'], $fields)) {
      form_set_error('field_match', t('Database fields can only be matched to one column of the csv file.'));
    }

    $fields[$values['field_match']] = $values['field_match'];

    // check email address has been selected
    if ($values['field_match'] == 'user-email') $email = TRUE;
  }

  if (!$email) form_set_error('email', t('One column of the csv file must be set as the email address.'));

  if ($form_state['values']['name']) {
     $form_state['values']['name'] = rtrim($form_state['values']['name']);

     if (drupal_strlen($form_state['values']['name']) < 1 || drupal_strlen($form_state['values']['name']) > 25) {
      form_set_error('name', t('Name of saved settings must be 25 characters or less.'));
     }
  }

  return;
}

/**
 *  Save a new template.
 */
function user_import_template_new_submit($form, &$form_state) {

  // save settings for import
  _user_import_settings_save($form_state['values']);

  // save settings for template
  $import_id = $form_state['values']['import_id'];
  $form_state['values']['setting'] = 'template';
  unset($form_state['values']['import_id']);
  _user_import_initialise_import($form_state['values']);
  drupal_set_message(t("'%name' was saved as a settings template.", array('%name' => $form_state['values']['name'])));

  // reload settings page
  $form_state['redirect'] = 'admin/user/user_import/add/' . $import_id;
  return;
}

/**
 *  Update an existing template.
 */
function user_import_template_update_submit($form, &$form_state) {

  // save settings for import
  $import_id = $form_state['values']['import_id'];
  _user_import_settings_save($form_state['values']);

  // get template details
  $template_id = db_result(db_query("SELECT import_id from {user_import} where setting = 'template' AND name= '%s' LIMIT 1", $form['#current_template']));

  // save settings for template
  $form_state['values']['setting'] = 'template';
  $form_state['values']['import_id'] = $template_id;
  $form_state['values']['name'] = $form['#current_template'];
  _user_import_initialise_import($form_state['values']);
  drupal_set_message (t("'%name' settings template was updated.", array('%name' => $form['#current_template'])));

  // reload settings page
  $form_state['redirect'] = 'admin/user/user_import/add/' . $import_id;
  return;
}

/**
 *
 */
function user_import_test_submit($form, &$form_state) {
  $form_state['values']['setting'] = 'test';
  drupal_set_message(t('Tested'));
}

/**
 *
 */
function user_import_import_submit($form, &$form_state) {
    $form_state['values']['setting'] = 'import';
    drupal_set_message (t('Imported'));
}

/**
 *
 */
function user_import_edit_submit($form, &$form_state) {

    if ($form_state['values']['setting'] == 'file set') $filepath = file_move($form_state['values']['filepath'], file_directory_path() . '/' . $form_state['values']['filename']);
    if (!empty($form_state['values']['og_id'])) $form_state['values']['groups'][$form_state['values']['og_id']] = $form_state['values']['og_id'];
    $form_state['values']['options']['ftp'] = $form_state['values']['ftp'];
  $form_state['values'] = _user_import_settings_save($form_state['values']);
  $form_state['values']['save']['update'] = NULL;
  $form_state['values']['import_template_id'] = NULL;
  $form_state['values']['save']['name'] = NULL;
  $form_state['redirect'] = $form_state['values']['return_path'];
  _user_import_process($form_state['values']);
}

function user_import_add_file_form(&$form, $ftp_files = NULL) {

    $form['browser'] = array(
        '#type' => 'fieldset',
        '#title' => t('Browser Upload'),
        '#collapsible' => TRUE,
        '#description' => t("Upload a CSV file."),
    );

    if (function_exists('file_upload_max_size')) $file_size = t('Maximum file size: !size MB.', array('!size' => file_upload_max_size()));

    $form['browser']['file_upload'] = array(
        '#type' => 'file',
        '#title' => t('CSV File'),
        '#size' => 40,
        '#description' => t('Select the CSV file to be imported. ') . $file_size,
    );

    if (!empty($ftp_files)) {

      $form['ftp'] = array(
          '#type' => 'fieldset',
          '#title' => t('FTP Upload'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#description' => t("Any files uploaded to the 'user_import' directory using FTP can be selected for import here. Useful if the import file is too large for upload via the browser."),
      );

       $form['ftp']['file_ftp'] = array(
            '#type' => 'radios',
            '#title' => t('Files'),
            '#default_value' => 0,
            '#options' => $ftp_files,
       );

      // reload the page to show any files that have been added by FTP
      $form['ftp']['scan'] = array(
          '#type' => 'submit',
          '#value' => t('Check for new files'),
          '#validate' => array(),
          '#submit' => array(),
      );

    }

    return;
}

/**
 * Saves options on content type configuration form
 * @todo check if this is cruft
 * @todo check $form['type']
 */
function user_import_content_type_submit($form, &$form_state) {
  // user import template for Organic Groups content type
  $templates = variable_get('user_import_og_template', array());
  $templates[$form['type']] = $form_state['values']['user_import_og'];
  variable_set('user_import_og_template', $templates);
}

// - - - - - - - -  PAGES - - - - - - - -

function user_import_preferences($import_id = NULL, $template_id = NULL) {

  if (empty($import_id)) {
    $output .= drupal_get_form('user_import_add_form');
  } else {
    $output .= drupal_get_form('user_import_edit', $import_id, $template_id);
  }

  return $output;
}

function user_import_list($action = NULL, $import_id = NULL) {

    // clear incomplete imports
    _user_import_incomplete_deletion();

    if (!empty($import_id) && is_numeric($import_id)) {

      $pager_id = 1;
      $max = 25;
      $import = _user_import_settings_select($import_id);

      $total = db_result(db_query("SELECT count(data) FROM {user_import_errors} WHERE import_id = %d", $import['import_id']));
      $results = pager_query("SELECT * FROM {user_import_errors} WHERE import_id = %d", $max, $pager_id, NULL, array($import['import_id']));

      while ($line = db_fetch_array($results)) {

       $line['data'] = unserialize($line['data']);
          $line['errors'] = unserialize($line['errors']);

          $file_lines[] = $line;
      }

      $output = theme('user_import_errors_display', $import, $file_lines, $total);
      $output .= theme('pager', NULL, $max, $pager_id);

    } else {

      $output =  theme_user_import_list();
    }



    return $output;
}

function user_import_continue($import_id = NULL) {

    if (!empty($import_id) && is_numeric($import_id)) {

    $import = _user_import_settings_select($import_id);
    _user_import_process($import);
    }

    drupal_goto('admin/user/user_import');
}

function user_import_import($import_id = NULL) {

    if (!empty($import_id) && is_numeric($import_id)) {

    $import = _user_import_settings_select($import_id);
    _user_import_initialise_import($import);
    }

    drupal_goto('admin/user/user_import');
}

function user_import_errors_display($import_id = NULL) {

  if (empty($import_id) || !is_numeric($import_id)) drupal_goto('admin/user/user_import');

  $import = _user_import_settings_select($import_id);
  $output = theme('user_import_errors_display', $import);
  return $output;
}

function user_import_limited_errors($import_id = NULL, $template_id = NULL) {

  if (empty($import_id) || !is_numeric($import_id)) drupal_goto('user_import/' . $template_id);

  $pager_id = 1;
  $max = 25;
  $import = _user_import_settings_select($import_id);

  $total = db_result(db_query("SELECT count(data) FROM {user_import_errors} WHERE import_id = %d", $import['import_id']));

  if (empty($total))  {

    $output .= theme('There were no import errors');

  } else {

    $results = pager_query("SELECT * FROM {user_import_errors} WHERE import_id = %d", $max, $pager_id, NULL, array($import['import_id']));

    while ($line = db_fetch_array($results)) {

      $line['data'] = unserialize($line['data']);
      $line['errors'] = unserialize($line['errors']);
      $file_lines[] = $line;
    }

    $output .= theme('user_import_errors_display', $import, $file_lines, $total);
    $output .= theme('pager', NULL, $max, $pager_id);
  }

  $output .= l(t('Return'), "user_import/$template_id/$import_id");

  return $output;
}


// errors for user being imported
function user_import_errors($error = FALSE, $clear = FALSE) {

  static $errors = array();
  if ($clear) $errors = array();
  if ($error) $errors[] = $error;
  return $errors;
}

function user_import_limited_delete($import_id = NULL, $template_id = NULL) {

  user_import_delete($import_id, "user_import/$template_id");
}

function user_import_delete($import_id = NULL, $return_path = 'admin/user/user_import') {

    if (empty($import_id) || !is_numeric($import_id)) drupal_goto($return_path);

    $import = _user_import_settings_select($import_id);
    _user_import_settings_deletion($import_id);
    _user_import_file_deletion($import['filepath'], $import['filename'], $import['oldfilename'], $import['options']['ftp']);
    drupal_goto($return_path);
    return;
}

// - - - - - - - -  THEMES - - - - - - - -

function theme_user_import_list() {

  $imports = _user_import_settings_select();

  if (!$imports) return ' ';

  foreach ($imports as $import) {

      // header labels
      $import_label = ($import['setting'] == 'tested' || $import['setting'] == 'test') ? t('importable') : t('imported');
      $header = array(t('file'), t('started'), t('processed'), $import_label, t('errors'), t('status'));

      // info row
      $errors = db_result(db_query("SELECT COUNT(import_id) FROM {user_import_errors} WHERE import_id = %d", $import['import_id']));
      $errors_link = ($errors == 0) ? '0': l($errors, 'admin/user/user_import/errors/' . $import['import_id']);

      $rows[0] = array(
          check_plain($import['oldfilename']),
          format_date($import['started'], 'small'),
          array("data" => $import['processed'], "align" => 'center'),
          array("data" => $import['valid'], "align" => 'center'),
          array("data" => $errors_link, "align" => 'center'),
          $import['setting'],
      );

      $output .= theme('table', $header, $rows);

      // action buttons
      $settings_link = l(t('Settings'), 'admin/user/user_import/add/' . $import['import_id']);
      $delete_link = l(t('Delete'), 'admin/user/user_import/delete/' . $import['import_id']);
      $continue_link = l(t('Continue Processing'), 'admin/user/user_import/continue/' . $import['import_id']);
      $import_link = l(t('Import'), 'admin/user/user_import/import/' . $import['import_id']);

      $output .= $settings_link  . ' | ';
      $output .= $delete_link;
      if ($import['setting'] == 'tested' || $import['setting'] == 'test') $output .= ' | ' . $import_link;
      if ($import['setting'] == 'test' || $import['setting']  == 'import') $output .= ' | ' . $continue_link;
  }

  return $output;
}

function theme_user_import_edit($form) {

  $header = array(t('csv column'), t('Drupal fields'), t('username'), t('abbreviate'));

  foreach (element_children($form['field_match']) as $key) {

      $rows[] = array(
          drupal_render($form['field_match'][$key]['csv']),
          drupal_render($form['field_match'][$key]['field_match']),
          drupal_render($form['field_match'][$key]['username']),
          drupal_render($form['field_match'][$key]['abbreviate']),
      );
  }

  $form['field_match']['#value'] = theme('table', $header, $rows);

  $output .= drupal_render($form['remove']);
  $output .= drupal_render($form['options']);
  $output .= drupal_render($form['field_match']);
  $output .= drupal_render($form);
  return $output;
}

function theme_user_import_errors_display($settings, $data, $total) {

    $error_count = 0;
    $field_match = _user_import_unconcatenate_field_match($settings['field_match']);
    $header[0] = t('Email Address');

    foreach ($data as $data_row) {

        $row = NULL;

        foreach ($data_row['data'] as $type => $fields) {

          if (!empty($fields)) {

            foreach ($fields as $field_id => $field_data) {

              foreach ($field_match as $column_info) {

                if ($column_info['type'] ==  $type && $column_info['field_id'] == $field_id) {

                  if (!empty($column_info['username'])) {
                    $header[$column_info['username']] = t('Name %sort', array('%sort' => $column_info['username']));
                    $row[$column_info['username']] = array("data" => $field_data[0], "align" => "left");
                  }

                  if ($column_info['field_id'] == 'email') {
                    $row[0] = array("data" => $field_data[0], "align" => "left");
                  }
                }
              }

            }
          }
        }

        ksort($row);
        $row[] = implode('<br />', $data_row['errors']);
        $rows[] = $row;
    }

    $output .= '<p>' . t('<strong>CSV File:</strong> %file', array('%file' => $settings['oldfilename'])) . '<br />';
    $output .= t('<strong>Errors:</strong> !total', array('!total' => $total)) . '</p>';

    $header['errors'] = t('Errors');
    $output .= theme('table', $header, $rows);
    return $output;
}

function theme_user_import_username_errors($errors) {

    if (empty($errors)) {
        $output = '<p><strong>' . t('All usernames are OK.') . '</strong></p>';
    } else {
        $header = array(t('User ID'), t('Email'), t('Username'), t('Error'));
        $output = theme('table', $header, $errors);
    }

    return $output;
}


// - - - - - - - -  FILE HANDLING - - - - - - - -

function _user_import_file_deletion($filepath, $filename, $old_filename, $ftp, $message = TRUE) {

  if ($ftp) {
    drupal_set_message(t("File '%filename' was uploaded using FTP and should be deleted manually once the import has been completed.", array('%filename' => $filename)));
    return;
  }

  $removed = file_delete($filepath);

  if (!$message) return;

  if (empty($removed)) {
      drupal_set_message(t("File error: file '%old_filename' (%filename) could not be deleted.", array('%old_filename' => $oldfilename, '%filename' => $filename)), 'error');
  }
  else {
      drupal_set_message(t("File '%old_filename' was deleted.", array('%old_filename' => $old_filename)));
  }

  return;
}

/*
 * File being used
 * $import_id - use file info stored in database
 * $ftp_file - chosen from FTP uploaded files
 * $uploaded_file - uploaded through browser
 */
function _user_import_file($import_id = NULL, $ftp_file_selected = NULL) {

    static $file;
    if (!empty($file)) return $file;

    // file was uploaded through browser
    $file = file_save_upload('file_upload');
    if (!empty($file) ) return $file;

    // file was uploaded by FTP
    if (!empty($ftp_file_selected)) {
        $ftp_files = _user_import_ftp_files();
        $filepath = drupal_get_path('module', 'user_import');
        $filename = $ftp_files[$ftp_file_selected];
        $file = new stdClass();
        $file->filepath = "$filepath/$filename";
        $file->filename = $filename;
        return $file;
    }

   // use file info stored in database
   if (!empty($import_id)) {
        $import = _user_import_settings_select($import_id);
        $file->filepath = $import['filepath'];
        $file->oldfilename = $import['oldfilename'];
        $file->filename = $import['filename'];
        return $file;
   }

    return;
}

/**
 * open file
 */
function _user_import_file_open($filepath, $filename) {

  ini_set('auto_detect_line_endings', true);
  $handle = @fopen($filepath, "r");

  if (!$handle) {
    form_set_error('file', t("Could not find the csv file '%filename'", array('%filename' => $filename)), 'error');
    return t("Please add your file again.");
  }

  return $handle;
}

// get first row of file
function _user_import_file_row($filename, $handle) {

    $data_row = @fgetcsv ($handle, 1000000, ",");
    if (!$data_row) {
        form_set_error('file', t("Could not get data, the file '%filename' is either empty or has incompatible line endings.", array('%filename' => $filename)), 'error');
    }
    return $data_row;
}

// get info on files  uploaded via FTP
function _user_import_ftp_files() {

  $directory = opendir( drupal_get_path('module', 'user_import') );
  $filenames[] = t('none');

  while ($file = readdir($directory)) {
    if ($file != '.' && $file != '..' && $file != '.DS_Store' && $file != 'CVS' && $file != '.svn' && $file != 'README.txt' && $file != 'LICENSE.txt' && $file != 'UPDATES.txt' && $file != 'user_import.module' && $file != 'user_import.mysql' && $file != 'user_import.install' && $file != 'user_import.info' && $file != 'supported' && $file != 'tests') $filenames[] = $file;
  }

  closedir($directory);
  return $filenames;
}

// - - - - - - - -  MISC - - - - - - - -

function user_import_edit_file_fields(&$form, $import) {

    $form['filename'] = array(
        '#type' => 'value',
        '#value' => $import['filename'],
    );

    $form['oldfilename'] = array(
        '#type' => 'value',
        '#value' => $import['oldfilename'],
    );

    $form['filepath'] = array(
        '#type' => 'value',
        '#value' => $import['filepath'],
    );

    return;
}

function user_import_form_field_match(&$form, $import) {

  $collapsed = (empty($import['name'])) ? FALSE: TRUE;
  $handle = _user_import_file_open($form['filepath']['#value'], $form['filename']['#value']);
  $data_row = _user_import_file_row($form['filename']['#value'], $handle);

  $fieldmatch_description = t("Match columns in CSV file to profile fields, leave as '----' if there is no match.");
  $fieldmatch_description .= '<br /><strong>' . t('Username') . ': </strong>' . t("The Username will be built from CSV columns in the order selected.");
  $fieldmatch_description .= '<br /><strong>' . t('Abbreviate') . ': </strong>' . t("Use the first letter of a field in uppercase for the Username, e.g. 'john' -> 'J'.");
  $fieldmatch_description .= '<br />' . t("If no CSV fields are selected, the Username will be randomly generated.");

  $form['field_match'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field Match'),
      '#description' => $fieldmatch_description,
      '#weight' => -90,
      '#collapsible' => TRUE,
      '#collapsed' => $collapsed,
      '#tree' => TRUE,
  );

  // add default and email address options
  $user_fields[0] = '-------------';
  $additional_user_fields = module_invoke_all('user_import_form_field_match');

  foreach ($additional_user_fields as $type => $type_options) {
    if (is_array($type_options)) {
      foreach ($type_options as $field_id => $label) {
        $user_fields["$type-$field_id"] = $label;
      }
    }
  }

  asort($user_fields);

  $row = 0;
  $sort = array(t('no'), 1, 2, 3, 4);

  if (empty($data_row)) return;

  foreach ($data_row as $data_cell) {

      $form['field_match'][$row]= array(
          '#tree' => TRUE,
      );

      $form['field_match'][$row]['csv'] = array(
          '#value' => check_plain(drupal_substr($data_cell, 0, 40)),
      );

      $form['field_match'][$row]['field_match'] = array(
          '#type' => 'select',
          '#default_value' => ($import['field_match'][$row]['field_match']) ? $import['field_match'][$row]['field_match'] : $user_fields[0],
          '#options' => $user_fields,
      );

      $form['field_match'][$row]['username'] = array(
          '#type' => 'select',
          '#default_value' => ($import['field_match'][$row]['username']) ? $import['field_match'][$row]['username'] : $sort[0],
          '#options' => $sort,
      );

      $form['field_match'][$row]['abbreviate'] = array(
          '#type' => 'checkbox',
          '#default_value' => ($import['field_match'][$row]['abbreviate']) ? $import['field_match'][$row]['abbreviate'] : NULL,
      );

      $row++;
  }

  return;
}

function _user_import_create_username($order, $data, $abbreviate, $username_space) {

    if (is_array($order)) {

      asort($order);
      reset($order);

      $username = '';

      while (list ($file_column, $sequence) = each ($order)) {

          if (!empty($username) && !empty($username_space)) $username .= ' ';
          $username .= ($abbreviate[$file_column] == 1) ? trim(drupal_strtoupper(chr(ord($data[$file_column])))) : trim($data[$file_column]);
      }
    }

    if (empty($username)) $username = _user_import_random_username();

    $username = _user_import_sanitise_username($username);
    $username = _user_import_unique_username($username, TRUE);
    return $username;
}

/**
 *  conform to Drupal username rules
 */
function _user_import_sanitise_username($username) {

  // username cannot contain an illegal character
  $username = preg_replace('/[^\x80-\xF7 [:alnum:]@_.-]/', '', $username);
  $username = preg_replace(
    '/[\x{80}-\x{A0}'.          // Non-printable ISO-8859-1 + NBSP
    '\x{AD}'.                 // Soft-hyphen
    '\x{2000}-\x{200F}'.      // Various space characters
    '\x{2028}-\x{202F}'.      // Bidirectional text overrides
    '\x{205F}-\x{206F}'.      // Various text hinting characters
    '\x{FEFF}'.               // Byte order mark
    '\x{FF01}-\x{FF60}'.      // Full-width latin
    '\x{FFF9}-\x{FFFD}'.      // Replacement characters
    '\x{0}]/u',
    '', $username);

  // username cannot contain multiple spaces in a row
  $username = preg_replace('/[ ]+/', ' ', $username);

  // username must be less than 56 characters
  $username = substr($username, 0, 56);

  // username cannot begin or end with a space
  $username = trim($username);
  return $username;
}

/**
 *  deal with duplicate usernames
 */
function _user_import_unique_username($username, $start = FALSE) {

  static $suffix = 1;
  if ($start) $suffix = 1;

  if ($suffix < 2) {
    $duplicate = db_result(db_query("SELECT uid from {users} where name = '%s' LIMIT 1", $username));
  }
  else {
    $duplicate = db_result(db_query("SELECT uid from {users} where name = '%s' LIMIT 1", "$username $suffix"));
  }

  // loop until name is valid
  if (!empty($duplicate)) {
    $suffix++;
    _user_import_unique_username($username);
  }

  // add number at end of username if it already exists
  $username = ($suffix < 2) ? $username : "$username $suffix";

  return $username;
}

function _user_import_profile($key = 'fid', $return_value = NULL) {

    if (!module_exists('profile')) return;

    static $fields_static;
        $fields = array();

    // avoid making more than one database call for profile info
    if (empty($fields_static)) {

        $results = db_query("SELECT * FROM {profile_fields}");

        while ($row = db_fetch_object($results)) {
            // don't include private fields
            if (user_access('administer users') || $row->visibility != PROFILE_PRIVATE) {
                $fields_static[] = $row;
            }
        }
    }

    if (empty($fields_static)) return array();

    // return all profile fields info, or just specific type
    if (empty($return_value)) {

        foreach ($fields_static as $field) {
            $fields[$field->{$key}] = $field;
        }
    }
    else {
        foreach ($fields_static as $field) {
            $fields[$field->{$key}] = $field->{$return_value};
        }
    }

    asort($fields);
    return $fields;
}

function _user_import_settings_save($settings) {
    /**
     * @todo refactor to put all settings into 'options' column, except settings that have their own column
     */

    // move settings into 'options' column
    // 'options' column will be used to store new control options - instead of creating a new column for each option
    $settings['options']['groups'] = $settings['groups'];
    $settings['options']['existing_og_subscribe'] = $settings['existing_og_subscribe'];
    $settings['options']['existing_og_subject'] = $settings['existing_og_subject'];
    $settings['options']['existing_og_markup'] = $settings['existing_og_markup'];
    $settings['options']['existing_og_message'] = $settings['existing_og_message'];
    $settings['options']['existing_og_css'] = $settings['existing_og_css'];

    $settings['options']['subject'] = $settings['subject'];
    $settings['options']['message'] = $settings['message'];
    $settings['options']['message_format'] = $settings['message_format'];
    $settings['options']['message_css'] = $settings['message_css'];

    $settings['options']['activate'] = $settings['activate'];
    $settings['options']['subscribed'] = $settings['subscribed'];
    $settings['options']['update_user'] = $settings['update_user'];

        $settings['options']['roles_new'] = $settings['roles_new'];

    // Update settings for existing import
    if ($settings['import_id']) {

        db_query("UPDATE {user_import}
            SET name = '%s', filename = '%s', oldfilename = '%s', filepath = '%s', pointer = %d, processed = %d, valid= %d, first_line_skip = %d, contact = %d, username_space = %d, send_email = %d, field_match = '%s', roles = '%s', options = '%s', setting = '%s'
            WHERE import_id = %d
            ", trim($settings['name']), $settings['filename'], $settings['oldfilename'], $settings['filepath'], $settings['pointer'], $settings['processed'], $settings['valid'], $settings['first_line_skip'], $settings['contact'], $settings['username_space'], $settings['send_email'], serialize($settings['field_match']), serialize($settings['roles']), serialize($settings['options']), $settings['setting'], $settings['import_id']);

        // Save settings for new import
    }
    else {

        db_query("INSERT INTO {user_import}
            (name, filename, oldfilename, filepath, started, pointer, processed, valid, first_line_skip, contact, username_space, send_email, field_match, roles, options, setting)
            VALUES ('%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s', '%s', '%s')
            ", trim($settings['name']), $settings['filename'], $settings['oldfilename'], $settings['filepath'], time(), $settings['pointer'], $settings['processed'], $settings['valid'], $settings['first_line_skip'], $settings['contact'], $settings['username_space'], $settings['send_email'], serialize($settings['field_match']), serialize($settings['roles']), serialize($settings['options']), $settings['setting']);

        switch ($GLOBALS['db_type']) {
          case 'mysql':
          case 'mysqli':
            $settings['import_id'] = db_result(db_query("SELECT LAST_INSERT_ID()"));
            break;
          case 'pgsql':
            $settings['import_id'] = db_result(db_query("SELECT currval('{user_import_import_id_seq}')"));
            break;
        }
    }

    return $settings;
}

// Update settings for existing import
function _user_import_settings_update($pointer, $processed, $valid, $setting, $import_id) {

  if (empty($import_id)) return;
  db_query("UPDATE {user_import} SET pointer = %d, processed = %d, valid= %d, setting = '%s' WHERE import_id = %d", $pointer, $processed, $valid, $setting, $import_id);
}

function _user_import_errors_display_save($import_id, $data, $email, $errors) {

    $data['email'] = $email;

    db_query("INSERT INTO {user_import_errors}
        (import_id, data, errors)
        VALUES (%d, '%s', '%s')
        ", $import_id, serialize($data), serialize($errors));
     return;
}

function _user_import_settings_select($import_id = NULL, $template = FALSE) {

  if (!empty($import_id) && !is_numeric($import_id)) return;

  if (!empty($import_id)) {
      $sql = 'SELECT * FROM {user_import} WHERE import_id = %d';
      if ($template) $sql .=  " AND setting = 'template'";
      $import = db_fetch_array(db_query($sql . ' LIMIT 1', $import_id));

      if (empty($import)) return FALSE;

      $import['field_match'] = unserialize($import['field_match']);
      $import['roles'] = unserialize($import['roles']);
      $import['options'] = unserialize($import['options']);
      /**
       * @todo loop through options and add them to first dimension of array
       *       i.e. $import['groups'] instead of $import['options']['groups']
       *       all use of $import['options'][foo] will also need to be updated
       */
  }
  else {

      $query = ($template) ? "SELECT * FROM {user_import} WHERE setting = 'template'" : "SELECT * FROM {user_import} WHERE setting != 'template' ORDER BY started DESC";
      $results = db_query($query);
      while ($row = db_fetch_array($results)) {
          $row['field_match'] = unserialize($row['field_match']);
          $row['roles'] = unserialize($row['roles']);
          $row['options'] = unserialize($row['options']);
        /**
         * @todo loop through options and add them to first dimension of array
         *       i.e. $import['groups'] instead of $import['options']['groups']
         *       all use of $import['options'][foo] will also need to be updated
         */
          $import[] = $row;
      }
  }

  return $import;
}

function _user_import_settings_deletion($import_id) {

    db_query("DELETE FROM {user_import} WHERE import_id = %d", $import_id);
    db_query("DELETE FROM {user_import_errors} WHERE import_id = %d", $import_id);
    return;
}

function _user_import_random_username() {

    $vowels = 'aoueiy';
    $consonants = 'bcdfghjklmnpqrstvwxz';
    $length = 8;

    mt_srand ((double) microtime() * 10000000);
    $next_vowel = 0;

    for ($count = 0; $count <= $length; $count++) {

        if ($next_vowel) {
            $rand = mt_rand(0, 5);
            $username.= $vowels{$rand};
            $next_vowel = 0;

        }
        else {
            $rand = mt_rand(0, 19);
            $username .= $consonants{$rand};
            $next_vowel = 1;
        }
    }

    return $username;
}

// check if any updates are to be made
function _user_import_update_user_check($settings) {

  foreach ($settings as $setting) {
    if ($setting != UPDATE_NONE) return TRUE;
  }

  return FALSE;
}

function _user_import_process($settings) {

    user_import_load_supported();
    $line_max = variable_get('user_import_line_max', 1000);
    $import_max = variable_get('user_import_max', 250);
    $field_match = _user_import_unconcatenate_field_match($settings['field_match']);
    $update_setting = _user_import_update_user_check($settings['options']['update_user']);
    $update_setting_per_module = $settings['options']['update_user'];

    ini_set('auto_detect_line_endings', true);
    $handle = @fopen($settings['filepath'], "r");

    // move pointer to where test/import last finished
    if ($settings['pointer'] != 0) fseek ($handle, $settings['pointer']);

    // start count of imports on this cron run
    $send_counter = 0;

    while ($data = fgetcsv($handle, $line_max, ',')) {

        $errors = user_import_errors(FALSE, TRUE);

        // if importing, check we are not over max number of imports per cron
        if ($settings['setting'] == 'import' && $send_counter >= $import_max) {
            $finished = TRUE;
            break;
        }

        // don't process empty lines
        $line_filled = (count($data) == 1 && drupal_strlen($data[0]) == 0) ? FALSE : TRUE;

        if ($line_filled) {

            // check if this is first line - if so should we skip?
            if (!empty($settings['first_line_skip']) && $settings['processed'] == 0) {
                // reset to false on second process
                $first_line_skip = (empty($first_line_skip)) ? TRUE : FALSE;
            }

            if (!$first_line_skip) {

                unset($password, $errors, $fields);
                reset($field_match);

                foreach ($field_match as $column_id => $column_settings) {

                  $type = $column_settings['type'];
                  $field_id = $column_settings['field_id'];

                  // Skip if this is a field used as part of a username but
                  // not otherwise mapped for import.
                  if ($type != 'username_part') {
                    $fields[$type][$field_id] = module_invoke_all('user_import_data', $settings, $update_setting, $column_settings, $type, $field_id, $data, $column_id);
                  }
                  // Read in data if present for concatenating a user name.
                  if ($column_settings['username'] > 0) {

                    $username_data[$column_id] = $data[$column_id];
                    $username_order[$column_id] = $column_settings['username'];
                    $username_abbreviate[$column_id]= $column_settings['abbreviate'];
                  }
                }

                $errors = user_import_errors();
                $account = array();
                $existing_account = FALSE;
                $updated = FALSE;

                // if we update existing users matched by email (and therefore passed validation even if this email already exists)
                // look for and use an existing account.
                if ($update_setting && !empty($fields['user']['email'][0])) {
                  $existing_account = user_load(array('mail' => $fields['user']['email'][0]));
                  if ($existing_account) $account = (array) $existing_account;
                }

                // if $account['uid'] is not empty then we can assume the account is being updated
                $account_additions = module_invoke_all('user_import_pre_save', $settings, $account, $fields, $errors, $update_setting_per_module);

                foreach($account_additions as $field_name => $value) {
                  $account[$field_name] = $value;
                }

                if (empty($errors)) {

                   if ($settings['setting'] == 'import') {

                     if ($existing_account) {
                       $account = user_save($existing_account, $account);
                       $updated = TRUE;
                     }
                     else {
                       // Only set a user name if we are not updating an existing record.
                       $account['name'] = _user_import_create_username($username_order, $username_data, $username_abbreviate, $settings['username_space']);
                       $password = $account['pass'];
                       $account = user_save('', $account);
                     }

                     module_invoke_all('user_import_after_save', $settings, $account, $password, $fields, $updated, $update_setting_per_module);
                     $send_counter++;
                   }

                  $settings['valid']++;
                }

                $settings['processed']++;
            }

            $settings['pointer'] = ftell($handle);

            // save lines that have fatal errors
            if (!empty($errors)) _user_import_errors_display_save($settings['import_id'], $fields, $account['email'], $errors);
        }
    }

    fclose ($handle);
    if ($settings['setting'] == 'import' && !$finished) $settings['setting'] = 'imported';
    if ($settings['setting'] == 'test') $settings['setting'] = 'tested';
    _user_import_settings_update($settings['pointer'], $settings['processed'], $settings['valid'], $settings['setting'], $settings['import_id']);
    return $settings;
}

function user_import_usernames_invalid($delete = NULL) {

    $users = db_query("SELECT uid, name, mail from {users} WHERE uid != 0 AND uid != 1");

    while ($user = db_fetch_object($users)) {

        $error = user_validate_name($user->name);

        if (!empty($error)) {
            $errors[$user->uid]['uid'] = $user->uid;
            $errors[$user->uid]['mail'] = $user->mail;
            $errors[$user->uid]['name'] = $user->name;
            $errors[$user->uid]['error'] = $error;

            if (!empty($delete)) {
                $form_state['values']['account'] = $user;
                /**
                 * @todo check if this breaks - user_confirm_delete_submit() has changed substantially
                 */
                user_confirm_delete_submit($form, $form_state);
            }
        }
    }

    $output = theme('user_import_username_errors', $errors);
    return $output;
}

// move from one stage to the next
// set up all necessary variables
function _user_import_initialise_import($import) {

    switch ($import['setting']) {
        case 'imported':
            drupal_set_message(t('File has already been imported'), 'error');
            break;

        // add setting template values to new import settings
        case 'file set':
            if (empty($import['template_id'])) return $import;
            $template  = _user_import_settings_select($import['template_id']);
            $template['import_id'] = $import['import_id'];
            $template['filename'] = $import['filename'];
            $template['oldfilename'] = $import['oldfilename'];
            $template['filepath'] = $import['filepath'];
            $template['started'] = 0;
            $template['setting'] = 'file set';
            return $template;

        case 'test':
        case 'tested':
            $import['setting'] = 'import';
            $import['started'] = 0;
            $import['pointer'] = 0;
            $import['processed'] = 0;
            $import['valid'] = 0;
            _user_import_errors_display_delete($import['import_id']);
            _user_import_settings_save($import);
            _user_import_process($import);
            break;

        case 'template':
            unset($import['filename']);
            unset($import['oldfilename']);
            unset($import['filepath']);
            $import['started'] = 0;
            $import['pointer'] = 0;
            $import['processed'] = 0;
            $import['valid'] = 0;
            _user_import_settings_save($import);
            break;

        default:
            _user_import_process($import);
            drupal_set_message(t('Imported'));
            break;
    }

    return;
}

function _user_import_errors_display_delete($import_id) {

    db_query("DELETE FROM {user_import_errors} WHERE import_id = %d", $import_id);
    return;
}

// delete incomplete import settings, where only the file has been uploaded
function  _user_import_incomplete_deletion() {

  $results = db_query("SELECT * FROM {user_import} WHERE setting = 'file set'");

  while ($import = db_fetch_object($results)) {
    $options = unserialize($import->options);
    _user_import_file_deletion($import->filepath, $import->filename, $import->oldfilename, $options['ftp'], FALSE);
    _user_import_settings_deletion($import->import_id);
  }

  return;
}


function user_import_profile_string($prefix = ',') {

    if (!module_exists('profile')) return;

    $profile_fields = _user_import_profile('fid', 'name');

    if (!empty($profile_fields)) $profile_string = ', !' . implode($prefix .' !', $profile_fields);

    return $profile_string;
}

function user_import_profile_load($user) {

  $result = db_query('SELECT f.name, f.type, f.fid, v.value FROM {profile_fields} f INNER JOIN {profile_values} v ON f.fid = v.fid WHERE uid = %d', $user->uid);

  while ($field = db_fetch_object($result)) {

    if (empty($profile[$field->fid])) {

      $profile[$field->fid] = _profile_field_serialize($field->type) ? unserialize($field->value) : $field->value;
    }

  }

  return $profile;
}

function _user_import_unconcatenate_field_match($settings) {

  $settings_updated = array();
  foreach ($settings as $column_id => $values) {

    if (!empty($values['field_match']) || !empty($values['username'])) {
      // If we have a username but no field_match, set a special type.
      // This allows us to skip saving the field but still use it in
      // concatenating a username value.
      if (empty($values['field_match'])) {
        $values['type'] = 'username_part';
        $values['field_id'] = 'username_part_'. $column_id;
      }
      else {
        $key_parts = explode('-', $values['field_match']);
        $values['type'] = array_shift($key_parts);
        $values['field_id'] = implode('-', $key_parts);
      }
      unset($values['field_match']);
      $settings_updated[$column_id] = $values;
    }
  }

  return $settings_updated;
}

/**
 * Loads the hooks for the supported modules.
 */
function user_import_load_supported() {
  static $loaded = FALSE;
  if (!$loaded) {
    $path = drupal_get_path('module', 'user_import') . '/supported';
    $files = drupal_system_listing('.*\.inc$', $path, 'name', 0);
    foreach ($files as $module_name => $file) {
      if (module_exists($module_name)) {
        include_once($file->filename);
      }
    }
    $loaded = TRUE;
  }
}

/**
* Implementation of hook_simpletest().
*/
function user_import_simpletest() {
  $module_name = 'user_import';
  $dir = drupal_get_path('module', $module_name). '/tests';
  $tests = file_scan_directory($dir, '\.test$');
  return array_keys($tests);
}





