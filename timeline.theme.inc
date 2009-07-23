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
  if(_timeline_include($vars['options']['timeline']['theme'])) {
  	  
    // preprocess css information for the template
    $vars['id'] = $vars['view']->name;
    $vars['class'] = $vars['view']->name;
    $vars['height'] = $vars['options']['timeline']['height'];
    $vars['width'] = $vars['options']['timeline']['width'];
    $vars['controls'] = $vars['options']['misc']['controls'];
    $vars['timeline_theme'] = $vars['options']['timeline']['theme'];
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
  	$timeline_mod_path = drupal_get_path('module', 'timeline');
  	
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
    //          Trying to include them within local.js so that they are set before. 
    //          If including it inline it is not getting included when files get included within the header
    //
    elseif(variable_get('timeline_library_source', 'hosted') == 'local') {
      $timeline_lib_path = libraries_get_path('simile_timeline');
      drupal_add_js($timeline_mod_path.'/js/local.js');
      drupal_add_js($timeline_lib_path.'/timeline_js/timeline-api.js');
    }
    drupal_add_css($timeline_mod_path .'/css/timeline.css');
   
    // include custom theme files (if activated)
    if($timeline_theme_name != 'ClassicTheme' && $timeline_theme_name != false) {
      drupal_add_js($timeline_mod_path .'/themes/'.$timeline_theme_name.'/'.$timeline_theme_name.'.js');
      drupal_add_css($timeline_mod_path .'/themes/'.$timeline_theme_name.'/'.$timeline_theme_name.'.css');
    }
    drupal_add_js($timeline_mod_path .'/js/timeline.js');
    return true;
  }
  else {
  	drupal_set_message(t('Simile Timeline Libraries not found'), 'error');
    return false;
  }   
}

