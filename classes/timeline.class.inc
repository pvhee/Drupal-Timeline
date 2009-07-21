<?php 
// $Id$

/**
 *   id - the XHTML identifier of the timeline element
 *   orientation - HORIZONTAL or VERTICAL
 *   incial_focus - the initial focus date of the timeline
 *   timezone - the timezone represented by # of hours difference from GMT
 *   controls - whether to show the filtering controls
 *   bands - interval settings of the bands
 *   summary - interval settings of the summary band
 *   timeline_start - A date to limit the beginning of the timeline.
 *   timeline_end - A date to limit the end of the timeline.
 *   theme_name - for the use of several themes
 *   bubble_max_height - A maximal height for bubbles (A scrollbar would be added on taller bubbles)
 *   bubble_width - Width of the bubbles
 */
class timeline {

  var $view;
  var $data;
  var $events;
  
  /**
   * Constructor.
   *
   * @param $view 
   */
  function __construct(&$view) {
  	$this->view = &$view;
  	
  	$this->data['events'] = $this->get_events();
  	$this->data['id'] = $view->name;
  	$this->data['orientation'] = $view->style_options['display']['type'];
    $this->data['initial_focus'] = $this->get_defined_date_field($view->style_options['bands']['focus'], $this->data['events']);
    $this->data['timezone'] = 0; // missing
    $this->data['controls'] = FALSE; // missing
    $this->data['bands'] = $this->get_bands();
    $this->data['summary'] = strtoupper($view->style_options['bands']['band2_unit']);
    // startsOn ?!? ether.js on line 22 -fd
    $this->data['timeline_start'] = $this->get_defined_date_field($view->style_options['limit']['start'], $this->data['events']);
    $this->data['timeline_end'] = $this->get_defined_date_field($view->style_options['limit']['end'], $this->data['events']);
    $this->data['theme'] = $view->style_options['display']['appearance']['theme'];
    $this->data['bubble_max_height'] = $view->style_options['display']['appearance']['bubble_height'];
    $this->data['bubble_width'] = $view->style_options['display']['appearance']['bubble_width'];
  }


  /**
   * 
   */
  function timeline_to_js() {
   	drupal_add_js(array('timeline' => array($this->data['id'] => $this->data)), 'setting');  	
  }
  
  
  /**
   * 
   */
  function get_events() {
  	$events = array();
    if(!empty($this->view->result)) {
     foreach (array($this->view->result) as $k => $set) {
       foreach ($set as $row) {
         $event = new timeline_event(&$this->view, &$row);
       	 if($event->get_data()) {
       	   $events[] = $event->get_data();
       	 }
       }
     }
    }
    return array(
      'wikiURL' => '',
      'wikiSection' => '',
      'dateTimeFormat' => 'rfc2822',
      'events' => $events,
    );
  }
  
  function get_bands() {
  	$bands = array();
    $bands[] = strtoupper($this->view->style_options['bands']['band1_unit']);
  	return $bands;
  }

  
  /**
   * Get the actual field value based on the timeline views settings and the result set of events.
   * 
   * @param
   *   $option - name of the selected option
   *   result - the result set of events
   * 
   * @return
   *   formated date
   * 
   * Note: -fd
   * First and last fields have to be improved:
   * If the view is not sorted on the start date this would lead wrong dates.
   * Last should also check if the event has an "end" date
   */ 
  function get_defined_date_field($option, $options_field = '') {    	
    switch ($option) {
      case 'today':
        return timeline_date_conversion(time(), 'timestamp');
      case 'first':
        return $this->data['events']['events'][0]['start'];
      case 'last':
       return $this->data['events']['events'][count($this->data - 1)]['start'];
      case 'custom':
        if($options_field != '') {
          return format_date(timeline_date_conversion($options_field, 'timestamp'));
        }
    }
    return FALSE;
  }
}