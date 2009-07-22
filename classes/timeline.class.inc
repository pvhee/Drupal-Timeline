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
  var $sets;
  var $color_id = 0;
  
  /**
   * Constructor.
   *
   * @param $view 
   */
  function __construct($view, $sets) {
  	$this->view = $view;
  	$this->sets = $sets;
  	
  	$this->data['events'] = $this->get_events();
  	$this->data['id'] = $this->view->name;
  	$this->data['orientation'] = $view->style_options['display']['type'];
  	$this->data['initial_focus'] = $this->get_defined_date_field($view->style_options['bands']['focus'], $view->style_options['bands']['focus_custom']);
    $this->data['timezone'] = 0; // missing
    $this->data['controls'] = FALSE; // missing
    $this->data['bands'] = $this->get_bands();
    $this->data['summary'] = strtoupper($view->style_options['bands']['band2_unit']);
    // startsOn ?!? ether.js on line 22 -fd
    //$this->data['timeline_start'] = $this->get_defined_date_field($view->style_options['limit']['start'], $view->style_options['limit']['custom_start']);
    //$this->data['timeline_end'] = $this->get_defined_date_field($view->style_options['limit']['end'], $view->style_options['limit']['custom_end']);
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
     foreach ($this->sets as $k => $set) {
       $css_selector = $this->generate_css_selector();
       $color = FALSE;
       if(isset($this->view->style_options['grouping']) && $this->view->style_options['grouping'] != '') {
         $color = $this->get_color();
       }
       foreach ($set as $row) {
         $event = new timeline_event(&$this->view, &$row, $css_selector, $color);
       	 if($event->get_data()) {
       	   $events[] = $event->get_data();
       	 }
       }
     }
    }
    return array(
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
          return timeline_date_conversion(strtotime($options_field), 'timestamp');
        }
    }
    return FALSE;
  }
  
  function get_color() {
    $colors = array(
      '#0A3FAC',
      '#CE1500',
      '#0A8624',
      '#FF6A00',
      '#CE0C72',
      '#F8D100',
    );

    // Override color information if specified
    if(isset($this->view->style_options['colors']['values']) && $this->view->style_options['colors']['values'] != '') {
      $colors = preg_split("/\r\n|\n/", $this->view->style_options['colors']['values']);
    }
        
    $tmp = $this->color_id;
    $this->color_id++;
    
    while($tmp >= count($colors)) {
      $tmp = $tmp - count($colors);
    }
    return $colors[$tmp];
  }
  
  function generate_css_selector() {
  	return $this->view->name.'-set-'.$this->color_id;
  }
}
