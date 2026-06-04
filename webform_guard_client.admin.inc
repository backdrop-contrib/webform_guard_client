<?php

/**
 * @file
 * Admin settings form for Webform Guard Client.
 */

/**
 * Settings form callback.
 */
function webform_guard_client_settings_form($form, &$form_state) {
  $config = config('webform_guard_client.settings');

  $last_test_time = $config->get('last_test_time');
  $last_test_status = $config->get('last_test_status');
  $last_test_version = $config->get('last_test_version');

  if ($last_test_time) {
    $status_class = (strpos($last_test_status, 'OK') !== FALSE) ? 'ok' : 'error';
    $markup = '<div class="webform-guard-connection-status webform-guard-connection-status--' . $status_class . '">'
      . '<strong>' . t('Last connection test:') . '</strong> '
      . format_date($last_test_time, 'short')
      . ' &mdash; ' . check_plain($last_test_status);
    if ($last_test_version) {
      $markup .= ' &mdash; ' . t('Server version: @v', array('@v' => $last_test_version));
    }
    $markup .= '</div>';

    $form['connection_status'] = array(
      '#type' => 'item',
      '#markup' => $markup,
      '#weight' => -10,
    );
  }

  $form['server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Guard server base URL'),
    '#description' => t('The base URL of the guard server, without a trailing slash. Example: https://guard.example.com'),
    '#default_value' => $config->get('server_url'),
    '#required' => TRUE,
  );

  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('The API key used to authenticate requests to the guard server.'),
    '#default_value' => $config->get('api_key'),
    '#required' => TRUE,
  );

  $form['site_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Site identifier'),
    '#description' => t('A unique identifier for this site sent with every request. Must match the site identifier on the guard server.'),
    '#default_value' => $config->get('site_identifier'),
  );

  $form['fallback_on_error'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fallback on service error'),
    '#description' => t('If the guard service is unavailable, allow normal submission delivery instead of blocking it.'),
    '#default_value' => $config->get('fallback_on_error'),
  );

  $form['test_connection'] = array(
    '#type' => 'submit',
    '#value' => t('Test connection'),
    '#submit' => array('webform_guard_client_test_connection_submit'),
    '#limit_validation_errors' => array(array('server_url'), array('api_key')),
    '#weight' => 9,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Submit handler: test the connection to the guard server.
 *
 * Calls /api/v1/status with the configured API key, stores the result, and
 * displays a message on the settings page.
 */
function webform_guard_client_test_connection_submit($form, &$form_state) {
  $server_url = rtrim(trim($form_state['values']['server_url']), '/');
  $api_key = trim($form_state['values']['api_key']);

  if (empty($server_url) || empty($api_key)) {
    backdrop_set_message(t('Please enter a server URL and API key before testing.'), 'warning');
    $form_state['rebuild'] = TRUE;
    return;
  }

  $options = array(
    'method' => 'GET',
    'headers' => array(
      'Authorization' => 'Bearer ' . $api_key,
    ),
    'timeout' => 8,
  );

  $response = backdrop_http_request($server_url . '/api/v1/status', $options);
  $config = config('webform_guard_client.settings');
  $version = '';

  if (!empty($response->error) || $response->code != 200) {
    $error = !empty($response->error) ? $response->error : 'HTTP ' . $response->code;
    $status = t('Failed — @error', array('@error' => $error));
    backdrop_set_message(t('Connection failed: @error', array('@error' => $error)), 'error');
  }
  else {
    $data = json_decode(webform_guard_client_extract_json($response->data));
    if (is_object($data) && !empty($data->status) && $data->status === 'ok') {
      $version = !empty($data->version) ? $data->version : '';
      if (!empty($data->authenticated)) {
        $status = t('OK — authenticated as @client', array('@client' => !empty($data->client) ? $data->client : t('unknown')));
        backdrop_set_message(t('Connection successful. Authenticated as: @client', array('@client' => !empty($data->client) ? $data->client : t('unknown'))), 'status');
      }
      else {
        $status = t('OK — server reachable but API key not recognised');
        backdrop_set_message(t('Server is reachable but the API key was not recognised. Check the key matches the client record on the guard server.'), 'warning');
      }
    }
    else {
      $status = t('Invalid response from server');
      backdrop_set_message(t('Server returned an unexpected response. The URL may be incorrect.'), 'warning');
    }
  }

  $config->set('last_test_time', REQUEST_TIME);
  $config->set('last_test_status', $status);
  $config->set('last_test_version', $version);
  $config->save();

  $form_state['rebuild'] = TRUE;
}

/**
 * Form submit handler.
 */
function webform_guard_client_settings_form_submit($form, &$form_state) {
  $config = config('webform_guard_client.settings');

  $config->set('server_url', $form_state['values']['server_url']);
  $config->set('api_key', $form_state['values']['api_key']);
  $config->set('site_identifier', $form_state['values']['site_identifier']);
  $config->set('fallback_on_error', !empty($form_state['values']['fallback_on_error']));
  $config->save();

  backdrop_set_message(t('Webform Guard Client settings saved.'));
}
