<?php
// $Id: $

/**
 * @file
 * CTools plugins declarations.
 */

/**
 * Break out for feeds_feed_plugins().
 */
function _timeline_timeline_plugins() {
  $path = drupal_get_path('module', 'timeline') .'/plugins';

  $info = array();
  $info['DatePlugin'] = array(
    'hidden' => TRUE,
    'handler' => array(
      'class' => 'DatePlugin',
      'file' => 'DatePlugin.inc',
      'path' => $path,
    ),
  );
  $info['StandardNodePlugin'] = array(
    'hidden' => TRUE,
    'handler' => array(
      'class' => 'StandardNodePlugin',
      'file' => 'StandardNodePlugin.inc',
      'path' => $path,
    ),
  );
  
  return $info;
}
