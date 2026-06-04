<?php

/**
 * @file
 * Configuration metadata for Webform Guard Client.
 */

/**
 * Implements hook_config_info().
 */
function webform_guard_client_config_info() {
  $config['webform_guard_client.settings'] = array(
    'label' => 'Webform Guard Client settings',
    'group' => 'system',
  );

  return $config;
}
