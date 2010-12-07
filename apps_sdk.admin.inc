<?php
// $Id: apps_sdk.admin.inc,v 0.1 2010/11/29 ccunningham $

/**
 * @file
 * apps_sdk.admin.inc
 */

/**
 * Administrative listing of all apps.
 */
function apps_sdk_admin_list() {

  $rows = array();

  $header = array(
    array('data' => t('Name'), 'field' => 'name'),
    array('data' => t('Key'), 'field' => 'appkey'),
    array('data' => t('URL'), 'field' => 'url'),
    array('data' => t('CSS'), 'field' => 'css'),
    array('data' => t('Status'), 'field' => 'status'),
    array('data' => t('Edit'), 'field' => 'edit'),
  );

  $sql = 'SELECT appid, appkey, name, url, css,status FROM {apps_apps}';
  $sql .= tablesort_sql($header);
  $result = pager_query($sql, 50, 0);

  while ($app = db_fetch_object($result)) {
    $options = l(t('edit'), "admin/build/apps/$app->appid/edit");
    $rows[] = array(
      check_plain($app->name),
    check_plain($app->appkey),
    check_plain($app->url),
    check_plain($app->css),
      $app->status == 1 ? t('enabled') : t('disabled'),
      $options);
  }

  if (empty($rows)) {
    drupal_set_message('There are currently no apps defined.');
  }

  return theme('table', $header, $rows);
}

/**
 * Administratively add/update an app.
 */
function apps_sdk_admin(&$form_state, $app = NULL) {

  $form = array();

  $form['name'] = array(
    '#title' => t('App name'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($app->name) ? check_plain($app->name) : '',
    '#description' => t('The full name of your application.  This name will be used when building menus and for navigation.'),
  );

  $form['appkey'] = array(
    '#title' => t('App Key'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($app->appkey) ? check_plain($app->appkey) : '',
    '#description' => t('The shared key (this must be included as the ID in the javascript SDK initialisation on the child application).'),
  );

  $form['url'] = array(
    '#title' => t('App URL'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($app->url) ? check_plain($app->url) : '',
    '#description' => t('The full url of your application.'),
  );

  $form['css'] = array(
    '#title' => t('CSS'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($app->css) ? check_plain($app->css) : '',
    '#description' => t('The full url to the css to be applied to the application.'),
  );

  $form['status'] = array(
    '#title' => t('Status'),
    '#type' => 'radios',
    '#options' => array(1 => t('Enabled'), 2 => t('Disabled')),
    '#default_value' => isset($app->status) ? $app->status : 1,
    '#description' => t('Apps will only be shown when enabled.'),
  );

  $form['appid'] = array(
    '#type' => 'hidden',
    '#value' => isset($app->appid) ? $app->appid : 0,
  );

  $form['submit'] = array(
    '#value' => isset($app->appid) ? t('Update App') : t('Add App'),
    '#type' => 'submit',
  );
  if (isset($app->appid)) {
    $form['delete'] = array(
      '#value' => t('Delete App'),
      '#type' => 'submit',
    );
    $form['cancel'] = array(
      '#value' => l(t('Cancel'), 'admin/build/apps'),
    );
  }

  return $form;
}

/**
 * Validate the form
 */
function apps_sdk_admin_validate($form, &$form_state) {
  if (preg_match('/[&*(),<>?\[\]]/', $form_state['values']['name'])) {
    form_set_error('name', t('You have entered an unsupported character.  The following characters can not be used in the app name: &*(),<>?[]'));
  }
}

/**
 * Add/update app information.
 */
function apps_sdk_admin_submit($form, &$form_state) {

  if ($form_state['clicked_button']['#value'] == t('Delete App')) {
    drupal_set_message(t('App %app deleted.', array('%app' => $form_state['values']['name'])));
    db_query('DELETE FROM {apps_apps} WHERE appid = %d', $form_state['values']['appid']);
  }
  else {
    db_query("UPDATE {apps_apps} SET name = '%s', appkey='%s', url='%s', css='%s', status = %d WHERE appid = %d", check_plain($form_state['values']['name']), check_plain($form_state['values']['appkey']), $form_state['values']['url'], $form_state['values']['css'], $form_state['values']['status'], $form_state['values']['appid']);
    if (!db_affected_rows()) {
      drupal_set_message(t('App %app added.', array('%app' => $form_state['values']['name'])));
      db_query("INSERT INTO {apps_apps} (name, appkey, url, css, status) VALUES ('%s', '%s', '%s','%s',%d)", check_plain($form_state['values']['name']), check_plain($form_state['values']['appkey']), $form_state['values']['url'], $form_state['values']['css'], $form_state['values']['status']);
    }
    else {
      drupal_set_message(t('App %app updated.', array('%app' => $form_state['values']['name'])));
    }
  }
  // clean page cache and block cache
  cache_clear_all();

  // rebuild the menu
  menu_rebuild();
  drupal_goto('admin/build/apps');
}
