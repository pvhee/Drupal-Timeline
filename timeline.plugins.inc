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
  $info['TimelinePlugin'] = array(
    'hidden' => TRUE,
    'handler' => array(
      'class' => 'TimelinePlugin',
      'file' => 'TimelinePlugin.inc',
      'path' => $path,
    ),
  );
  $info['TimelineNodePlugin'] = array(
    'handler_name' => 'views_handler_field_date',
    'hidden' => FALSE,
    'handler' => array(
      'class' => 'TimelineNodePlugin',
      'file' => 'TimelineNodePlugin.inc',
      'path' => $path,
    ),
  );
  $info['TimelineDatePlugin'] = array(
    'handler_name' => 'date_handler_field_multiple',
    'hidden' => FALSE,
    'handler' => array(
      'class' => 'TimelineDatePlugin',
      'file' => 'TimelineDatePlugin.inc',
      'path' => $path,
    ),
  );
  
  return $info;
}
