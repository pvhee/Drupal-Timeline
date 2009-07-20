<?php
// $Id: views_views_plugin_style_timeline.inc,v 1.1.2.2.2.1 2009/07/08 20:57:09 xamanu Exp $
/**
 * Extending the view_plugin_style class to provide a timeline view style.
 */
class views_views_plugin_style_timeline extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => TIMELINE_HORIZONTAL);
    $options['display']['width'] = array('default' => 200);
    $options['display']['height'] = array('default' => 100);
    $options['display']['focus'] = array('default' => 'today');
    $options['bands']['band1_unit'] = array('default' => 'day');
    $options['bands']['band2_unit'] = array('default' => 'month');
    $options['misc']['controls'] = array('default' => FALSE);
    $options['fields']['title'] = array('default' => '');
    $options['fields']['start'] = array('default' => '');
    $options['fields']['end'] = array('default' => ''); 
    return $options;
  }

  /**
   * Add settings for the particular timeline.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['display'] = array('#type' => 'fieldset', '#title' => t('Display settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
    
    $form['display']['type'] = array(
      '#type' => 'radios',
      '#title' => t('Timeline type'),
      '#options' => array(TIMELINE_HORIZONTAL => t('Horizontal'), TIMELINE_VERTICAL => t('Vertical')),
      '#default_value' => $this->options['type'],
    );
    $form['display']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => isset($this->options['display']['width']) ? $this->options['display']['width'] : variable_get('timeline_default_width', '100%'),
      '#size' => 6,
      '#maxlength' => 6,
      '#description' => t('The width of the timeline (in units of em, px or %), e.g. 600px or 90%. Leave blank to use default value.'),
    );
    $form['display']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#default_value' => isset($this->options['display']['height']) ? $this->options['display']['height'] : variable_get('timeline_default_height', '400px'),
      '#size' => 6,
      '#maxlength' => 6,
      '#description' => t('The height of the timeline (in units of em, px or %), e.g. 400px. Leave blank to use default value.'),
    );
    // Appearance settings
    // TODO make theme options flexible
    $form['display']['appearance'] = array('#type' => 'fieldset', '#title' => t('Appearance and themes'), '#collapsible' => TRUE, '#collapsed' => TRUE);
    $form['display']['appearance']['theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme'),
      '#default_value' => $this->options['display']['appearance']['theme'],
      '#options' => timeline_available_themes(),
      '#description' => t('Select a theme. You can create our own themes. Read documentation.'),
    );
    $form['display']['appearance']['bubble_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width of bubbles'),
      '#default_value' => $this->options['display']['appearance']['bubble_width'],
      '#size' => 6,
      '#maxlength' => 6,
      '#description' => t('The width of the bubbles in the timeline (in units of px), e.g. 200px. Leave blank to use default value from theme.'),
    );
    $form['display']['appearance']['bubble_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Max height of bubbles'),
      '#default_value' => $this->options['display']['appearance']['bubble_height'],
      '#size' => 6,
      '#maxlength' => 6,
      '#description' => t('The maximum height of the bubbles in the timeline (in units of px), e.g. 200px (Scrollbar will be added for taller bubbles). Leave blank to let the bubbles scale automatically.'),
    );
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your field settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
    }
    else {
      // Fields
      $field_names = array('' => '--');
      foreach ($handlers as $field => $handler) {
        $field_names[$field] = $handler->ui_name();
        if ($label = $handler->label()) {
          $field_names[$field] .= " (\"$label\")";
        }
      }

      $form['fields'] = array(
        '#type' => 'fieldset',
        '#title' => 'Field usage',
        '#description' => t('Select the fields that contain the title,
                            start time and end time of each item. If selected, 
                            the class field will be used to apply a class to  
                            each point. Remaining fields will be available as 
                            "content" of the item.'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['fields']['title'] = array(
        '#type' => 'select',
        '#title' => 'Title',
        '#options' => $field_names,
        '#default_value' => $this->options['fields']['title'],
        '#required' => true,
      );
      
      $form['fields']['start'] = array(
        '#type' => 'select',
        '#title' => 'Start',
        '#options' => $field_names,
        '#default_value' => $this->options['fields']['start'],
        '#required' => true,
      );
            
      $form['fields']['end'] = array(
        '#type' => 'select',
        '#title' => 'End',
        '#options' => $field_names,
        '#default_value' => $this->options['fields']['end'],
        '#required' => true,
      );
    
      $form['fields']['advanced'] = array('#type' => 'fieldset', '#title' => t('TODO: Advanced visualization settings'), '#collapsible' => TRUE, '#collapsed' => TRUE);
      $form['fields']['advanced']['earliestStart'] = array(
        '#type' => 'select',
        '#title' => t('Earliest start'),
        '#options' => $field_names,
        '#default_value' => $this->options['fields']['advanced']['earliestStart'],
      );
    
      $form['fields']['advanced']['latestEnd'] = array(
        '#type' => 'select',
        '#title' => t('Latest end'),
        '#options' => $field_names,
        '#default_value' => $this->options['fields']['advanced']['latestEnd'],
      );
      $form['fields']['advanced']['isDuration'] = array(
        '#type' => 'select',
        '#title' => t('"Is duration" attribute'),
        '#options' => $field_names,
        '#default_value' => $this->options['fields']['advanced']['latestEnd'],
        '#description' => t('Please select only Boolean fields! For more information on this field visit !url', array('!url' => l(t('http://code.google.com/p/simile-widgets/wiki/Timeline_Event_Display'), 'http://code.google.com/p/simile-widgets/wiki/Timeline_Event_Display'))),
      );
    }
        
    // Band/interval settings
    $form['bands'] = array('#type' => 'fieldset', '#title' => t('Band/interval settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
    $intervals = array('second' => t('Second'), 'minute' => t('Minute'), 'hour' => t('Hour'), 'day' => t('Day'), 'week' => t('Week'), 'month' => t('Month'), 'year' => t('Year'), 'decade' => t('Decade'), 'century' => t('Century'), 'millennium' => t('Millennium'));
    $form['bands']['band1_unit'] = array(
      '#type' => 'select',
      '#title' => t('Main band interval unit'),
      '#default_value' => $this->options['bands']['band1_unit'],
      '#options' => $intervals,
      '#description' => t(''),
    );
    $form['bands']['band2_unit'] = array(
      '#type' => 'select',
      '#title' => t('Summary band interval unit'),
      '#default_value' => $this->options['bands']['band2_unit'],
      '#options' => array_merge(array('' => t('Not displayed')), $intervals),
      '#description' => t(''),
    );
    
    $form['bands']['focus'] = array(
      '#type' => 'select',
      '#title' => t('Initial date focus'),
      '#default_value' => $this->options['bands']['focus'],
      '#options' => array('first' => t('First event in timeline'), 'last' => t('Last event in timeline'), 'today' => t('The current date'), 'custom' => t('Custom')),
      '#description' => t('Determines which event or date the timeline view should initially be centered on after it has loaded. Leave blank to use default value.'),
    );
    $form['bands']['focus_custom'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom inicial date focus'),
      '#default_value' => isset($this->options['bands']['focus_custom']) ? $this->options['bands']['focus_custom'] : '',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-style-options-bands-focus' => array('custom')),
      '#description' => t('Please enter a custom inicial date to focus the timeline on. Use this format: YYYY-MM-DD HH:MM:SS'),
    );
    $form['limit'] = array('#type' => 'fieldset', '#title' => t('Limit timeline bands'), '#collapsible' => TRUE, '#collapsed' => TRUE);
    $form['limit']['start'] = array(
      '#type' => 'select',
      '#title' => t('Timeline start date'),
      '#default_value' => isset($this->options['limit']['start']) ? $this->options['limit']['start'] : '',
      '#options' => array('endless' => 'Endless', 'first' => t('First event in timeline'), 'last' => t('Last event in timeline'), 'today' => t('The current date'), 'custom' => t('Custom')),
      '#description' => t('Limit timeline bands to start at this defined date'),
    );
    
    $form['limit']['custom_start'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom start date'),
      '#default_value' => isset($this->options['limit']['custom_start']) ? $this->options['limit']['custom_start'] : '',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-style-options-limit-start' => array('custom')),
      '#description' => t('Please enter a custom end date for the timeline. Use this format: YYYY-MM-DD HH:MM:SS'),
    );
    
    $form['limit']['end'] = array(
      '#type' => 'select',
      '#title' => t('Timeline end date'),
      '#default_value' => isset($this->options['limit']['end']) ? $this->options['limit']['end'] : '',
      '#options' => array('endless' => 'Endless', 'first' => t('First event in timeline'), 'last' => t('Last event in timeline'), 'today' => t('The current date'), 'custom' => t('Custom')),
      '#description' => t('Limit timeline bands to end at this defined date'),
    );
    $form['limit']['custom_end'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom end date'),
      '#default_value' => isset($this->options['limit']['custom_end']) ? $this->options['limit']['custom_end'] : '',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-style-options-limit-end' => array('custom')),
      '#description' => t('Please enter a custom end date for the timeline. Use this format: YYYY-MM-DD HH:MM:SS'),
    );

    // Miscellanous settings
    $form['misc'] = array('#type' => 'fieldset', '#title' => t('TODO: Miscellaneous settings'), '#collapsible' => TRUE, '#collapsed' => TRUE);
    $form['misc']['controls'] = array(
      '#type' => 'radios',
      '#title' => t('Enable controls'),
      '#default_value' => $this->options['misc']['controls'],
      '#options' => array(FALSE => t('Disabled'), TRUE => t('Enabled')),
      '#description' => t('Whether to display filtering and highlighting controls under the timeline. Not recommended for block mode.'),
    );
  }

  /**
   * Render the timeline style. 
   * Data for the simile timeline is getting collected within the view
   */
  function render() {

  	// collect data for simile timeline
    $this->options['timeline_array'] = $this->_build_timeline_array(); 
    
    // options that are used for template preprocessor
    $this->options['timeline_options'] = array(
      'class' => $this->view->name,
      'width' => _timeline_to_dim($this->view->style_options['display']['width']),
      'height' => _timeline_to_dim($this->view->style_options['display']['height']),
      'align' => '',
      'controls' => $this->view->style_options['misc']['controls'],
    );   

    //$sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    return theme($this->theme_functions(), $this->view, $this->options);
  }

  /**
   * Collects all necesarry information for the timeline. The result can be converted to a json string and pushed to the timeline.js
   * 
   * @return
   *   array with the following attributes set:
   * 
   *   timeline_sets - prepared data of events that are shown on the timeline
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
  function _build_timeline_array() {
  	
    // Support for more than one band. UI is missing - fd
    $bands = array();
    $bands[] = strtoupper($this->view->style_options['bands']['band1_unit']);
    $timeline_sets = $this->_collect_timeline_data_sets(); // event data
    // constructing array to give to timeline.js
    
  	return array(
  	  'timeline_sets' => $timeline_sets,
  	  'id' => $this->view->name, // id
  	  'orientation' => $this->view->style_options['display']['type'], // orientation. horizontal or vertical
  	  'initial_focus' => $this->_get_defined_date_field($this->view->style_options['bands']['focus'], $timeline_sets['events']), // inicial date
  	  'timezone' => '0', // timezone TODO
  	  'bands' => $bands, // bands
  	  'summary' => strtoupper($this->view->style_options['bands']['band2_unit']), // summary (band)
  	  
  	  'timeline_start' => $this->_get_defined_date_field($this->view->style_options['limit']['start'], $timeline_sets['events']), // timeline_start
      'timeline_end' => $this->_get_defined_date_field($this->view->style_options['limit']['end'], $timeline_sets['events']), // timeline_end
      'theme_name' => $this->view->style_options['display']['appearance']['theme'], // theme_name
      'bubble_max_height' => $this->view->style_options['display']['appearance']['bubble_height'], // bubble_height
      'bubble_width' => $this->view->style_options['display']['appearance']['bubble_width'], // bubble_width
  	);
  }
  
  /**
   * Builds sets of events for timeline based on the views result.
   * 
   * @return
   *   array of all events collected based on the views's result 
   * 
   * (note: don't forget be sure that excluding fields is possible! - fd)
   */
  function _collect_timeline_data_sets() {  	
    if(!empty($this->view->result)) {

      foreach (array($this->view->result) as $k => $set) {
        foreach ($set as $row) {
          $field = $this->_get_defined_field('start');
          
          if(isset($row->{$field['alias']})) {

            $field = $this->_get_defined_field('title');
            // do we really want to link it everytime? -fd
            $event = array('link' => url('node/'. $row->nid), 'description' => '');
            $event['title'] = $row->{$field['alias']};

            // fill event with dates            
            $events_validated = $this->_timeline_event_logic($row);
            $event['start'] = $events_validated['start']['formated'];  
            if(isset($events_validated['end']['formated'])) $event['end'] = $events_validated['end']['formated'];
            if(isset($events_validated['latestEnd']['formated'])) $event['latestEnd'] = $events_validated['latestEnd']['formated'];
            if(isset($events_validated['earliestStart']['formated'])) $event['earliestStart'] = $events_validated['earliestStart']['formated'];
            
            // i don't think that this is correct -fd
            foreach ($this->view->display_handler->handlers['field'] as $key => $field) {
              if ($field->options['exclude'] != 1) {
                $event['description'] .= $field->render($row);
              }
            }
          $bands[] = $event;
          }
        }
      }
      // return a json string
      return array(
        'wikiURL' => '',
        'wikiSection' => '',
        'dateTimeFormat' => 'rfc2822',
        'events' => $bands,
      );
    }
  } 


  // HELPER FUNCTIONS TO BUILD TIMELINE ARRAY
  
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
  function _get_defined_field($type) {
  	
  	if($timeline_settings_fields = $this->view->style_options['fields']) {
      if(isset($timeline_settings_fields[$type])) {
  	    $fieldname = $timeline_settings_fields[$type];
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
  
  /**
   * Get the actual field value based on the timeline views settings and the result set of events.
   * 
   * @param
   *   $option - name of the selected option
   *   $events - the result set of events
   * 
   * @return
   *   formated date
   * 
   * Note: -fd
   * First and last fields have to be improved:
   * If the view is not sorted on the start date this would lead wrong dates.
   * Last should also check if the event has an "end" date
   */ 
  function _get_defined_date_field($option, $events) {
	
    switch ($option) {
      case 'today':
        return timeline_date_conversion(time(), 'timestamp');
      case 'first':
        return $events[0]['start'];
      case 'last':
       return $events[count($events - 1)]['start'];
      //case 'custom':
      //  return timeline_date_conversion($this->view->style_options[$options_group]['focus_'.$options_field], 'iso8601');
    }
    return FALSE;
  }
  
  
  /**
   * This function validates the dates of one event that have a relation. Like start and end date.
   * 
   * @return
   *   $event: Array with the converted date and a unix timestamp on which some logic has been done
   */
  function _timeline_event_logic($row) {

  $event = array();
    foreach(array('start', 'end', 'earliestStart', 'latestEnd') as $event_field) {
      $field = $this->_get_defined_field($event_field);
      
      if(isset($row->{$field['alias']})) {
        $event[$event_field]['formated'] = timeline_date_conversion($row->{$field['alias']}, $field['date_format']);
        $event[$event_field]['timestamp'] = timeline_date_conversion($row->{$field['alias']}, $field['date_format'], 'timestamp');
      }
    }

    // Validate date values
    if(isset($event['earliestStart']) && $event['start']['timestamp'] <= $event['earliestStart']['timestamp']) {
      unset($event['earliestStart']);
    }
  
    if(isset($event['end']) && $event['end']['timestamp'] <= $event['start']['timestamp']) {
      unset($event['end']);
    }
  
    if(isset($event['latestEnd']) && $event['lastestEnd']['timestamp'] <= $event['start']['timestamp']) {
      unset($event['latestEnd']);
    }
     
    if(!isset($event['end'])) {
   	  $event['durationEvent']['value'] = TRUE;
    }
    else {
      $event['durationEvent']['value'] = FALSE;
    }
    dsm($event);
    return $event;
  }
}

