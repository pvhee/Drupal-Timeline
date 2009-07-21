<?php 
// $Id$

/**
 * 
 */
class timeline_event {
  var $view;
  var $data;
  
 /**
   * Constructor.
   *
   * @param $view 
   */
  function __construct($view, $row) {
  	$this->view = $view;  	
  	$this->data = $this->build_data($row);	
  }

  /**
   * 
   */
  function get_data() {
  	return $this->data;
  }
  /**
   * 
   */
  function build_data($row) {
  	if(!empty($this->view->result)) {
      $tmp = $this->get_defined_field_usage('start');

  	  if(isset($row->{$tmp['alias']})) {
        $data = array();
  	
        $tmp = $this->get_defined_field_usage('title');
        $data['title'] = $row->{$tmp['alias']};
        $data['link'] = url('node/'. $row->nid);
    
    
        $events_validated = $this->logic($row);
        $data['start'] = $events_validated['start']['formated'];
        if(isset($events_validated['end']['formated'])) $data['end'] = $events_validated['end']['formated'];
        if(isset($events_validated['earliestEnd']['formated'])) $data['earliestEnd'] = $events_validated['earliestEnd']['formated'];
        if(isset($events_validated['latestStart']['formated'])) $data['latestStart'] = $events_validated['latestStart']['formated'];
        $data['durationEvent'] = $events_validated['durationEvent'];
      
        $data['description'] = '';
        foreach ($this->view->display_handler->handlers['field'] as $key => $field) {
          if ($field->options['exclude'] != 1) {
            $data['description'] .= $field->render($row);
           }
        }
        return $data;
  	  }
    }
    return FALSE;
  }
  
  
  /**
   * This function validates the dates of one event that have a relation. Like start and end date.
   * There is a logic change to timeline. This conversion is done here.
   * 
   * @return
   *   $event: Array with the converted date and a unix timestamp on which some logic has been done
   */
  function logic($row) {

  $event = array();
    foreach(array('start', 'end', 'earliestStart', 'latestEnd') as $event_field) {
    	
      $field = $this->get_defined_field_usage($event_field);
      
      if(isset($row->{$field['alias']})) {
        $event[$event_field]['formated'] = timeline_date_conversion($row->{$field['alias']}, $field['date_format']);
        $event[$event_field]['timestamp'] = timeline_date_conversion($row->{$field['alias']}, $field['date_format'], 'timestamp');
        
      }
    }
    // Validate date values
    if(isset($event['earliestStart'])) {
      if($event['start']['timestamp'] <= $event['earliestStart']['timestamp']) {
        unset($event['earliestStart']);
      }
      else {
        $event['latestStart'] = $event['start'];
        $event['start'] = $event['earliestStart'];
      }
    }
    
    if(isset($event['latestEnd'])) {
      if($event['latestEnd']['timestamp'] <= $event['start']['timestamp']) {
        unset($event['latetEnd']);
      }
      else {
        $event['earliestEnd'] = $event['end'];
        $event['end'] = $event['latestEnd'];
      }
    }
    
    if(isset($event['end']) && $event['end']['timestamp'] <= $event['start']['timestamp']) {
      unset($event['end']);
      // to be tolerant for errors -fd
      if(isset($event['earliestEnd']) && $event['earliestEnd']['timestamp'] <= $event['start']['timestamp']) {
      	unset($event['earliestEnd']);
      }
    }
    
    if(isset($event['end'])) {
   	  $event['durationEvent'] = TRUE;
    }
    else {
      $event['durationEvent'] = FALSE;
    }
    return $event;
  }
  /**
   * The function returns the field that was defined by the views timeline settings.
   * 
   * @param 
   *   String that is expected to be "title", "start", "end", "earliestStart", "latestEnd"
   * 
   * @return
   *   array with the following attributes set:
   *   
   *   alias - The alias of the field
   *   handler - A string with the name of the handler
   *   date_format _ If it is an supported date information the expected date format
   */
  function get_defined_field_usage($type) {

  	if($timeline_settings_fields = $this->view->style_options['fields']) {
  		
  	  if(isset($timeline_settings_fields[$type])) {
  	  	if(is_string($timeline_settings_fields[$type])) {
  	  	  $fieldname = $timeline_settings_fields[$type];
  	  	}
  	  }
  	  elseif(isset($timeline_settings_fields['advanced'][$type])) {
  	  	if(is_string($timeline_settings_fields['advanced'][$type])) {
  	  	  $fieldname = $timeline_settings_fields['advanced'][$type];
  	  	}
  	  }
      if(isset($fieldname) && $fieldname != '') {
      	
        $field_information = array(
          'alias' => $this->view->field[$fieldname]->field_alias,
          'handler' => $this->view->field[$fieldname]->definition['handler'],
        );
        // check how to handle data from supported date fields
  	    switch($field_information['handler']) {
          case 'views_handler_field_date': // 1. standard views node date fields (unix timestamp)
            $field_information['date_format'] = 'timestamp';
  	        break;
          case 'date_handler_field_multiple': // 2. support for date module's date fields
            $field_information['date_format'] = $this->view->field[$fieldname]->content_field['type'];
            break;
  	    }
  	  return $field_information;
  	  }
  	}
  	else {
  	  return FALSE;
  	}
  }
}