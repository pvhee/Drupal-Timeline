<?php
// $Id: timeline.theme.inc,v 1.13.2.1.2.1 2009/07/08 20:57:09 xamanu Exp $
/**
 * @file
 * Theme functions for timeline.
 */

/**
 * Implementation of template_process for views-view-timeline.
 */
function template_preprocess_views_view_timeline(&$vars) {

  // create actual timeline
  if(_timeline_include($vars['options']['timeline_array']['theme_name'])) {  
    _timeline_build($vars['options']['timeline_array'], $vars['options']['timeline_options']['controls']);
  
    // preprocess css information for the template
    $vars['id'] = $vars['view']->name;
    $vars['class'] = $vars['view']->name;
    $vars['style'] = implode('; ', array_map_assoc(create_function('$k, $v', 'return "$k: $v";'), array('width' => $vars['options']['timeline_options']['width'], 'height' => $vars['options']['timeline_options']['height'],)));
    $vars['controls'] = $vars['options']['misc']['controls'];
  }
  else {
    drupal_set_message(t('Simile Timeline Libraries not found'), 'error');
  }
}

/**
 * Inlude all js and css files that needs the timeline. 
 * Depending on the timeline views settings the external SIMILE server is used.
 * Or otherwise a local installation.
 * 
 * @return: TRUE if everything could be included properly 
 *          FALSE if Simile Exhibit Libraries aren't found
 */
function _timeline_include($timeline_theme_name) {

  if($message = timeline_validate_library()) {
  	// Use SIMILE Server
    if(variable_get('timeline_library_source', 'hosted') == 'hosted') {
      $timeline_api = 'http://static.simile.mit.edu/timeline/api-2.3.0/timeline-api.js?bundle=true';
      drupal_set_html_head('<script type="text/javascript" src="'. $timeline_api .'"></script>'); //  We have to wait to D7 to get rid of this: #91250 - fd
    }
    
    // Timeline libraries are available locally.
    //
    // PROBLEM: timeline 2.x uses another library (simile-ajax) this has to be included with 
    //          setting two global js variable before timeline-api.js is getting included:
    //
    //          Timeline_urlPrefix="/sites/all/libraries/simile_timeline/timeline_js/";
    //          Timeline_ajax_url="http://YOUR_SERVER/javascripts/timeline/timeline_ajax/simile-ajax-api.js"; 
    //
    //          Couldn't get it to run, yet.
    //
    elseif(variable_get('timeline_library_source', 'hosted') == 'local') {
      $timeline_path = libraries_get_path('simile_timeline');
      drupal_add_js($timeline_path.'/timeline_js/timeline-api.js?bundle=true');
    }
    
    // include custom theme files (if activated)
    if($timeline_theme_name != 'classicTheme') {
      drupal_add_js(drupal_get_path('module', 'timeline') .'/themes/'.$timeline_theme_name.'.js');
    }
    
    drupal_add_js(drupal_get_path('module', 'timeline') .'/timeline.js');
    drupal_add_css(drupal_get_path('module', 'teline') .'/timeline.css');
    return true;
  }
  else {
    return false;
  }   
}

/**
 * Build the timeline. Call javascript.
 * All settings (general settings and event sets) are converted to a json array 
 * and the timeline initializing process is getting called 
 * 
 * @param
 * $timeline_array - necesary information for creating the timeline
 * $timeline_sets - sets of events' information
 */
function _timeline_build($args, $controls) {
  $json = drupal_to_js($args);
  $script = 'Drupal.timeline.createWidget('. $json .');';
  if ($controls) {
    $texts = implode(', ', array_map('drupal_to_js', array(t('Filter:'), t('Highlight:'), t('Clear All'))));
    $script = 'var timeline = '. $script ." Drupal.timeline.controls.setup('{$timeline_array->id}-controls', timeline, $texts);";
  }
  $script = 'if (Drupal.jsEnabled) { $(document).ready(function() { '. $script .' }); }';
  
  // Not sure if I can get away with this in all browsers -jm
  drupal_add_js($script, 'inline');
  
}

