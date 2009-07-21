// $Id: README.txt,v 1.7.2.1 2009/06/11 13:01:55 jmiccolis Exp $

DESCRIPTION
-----------
Timeline is a DHTML-based AJAXy widget for visualizing temporal information.
It works in a similar way as Google Maps but for time-based events.

This Drupal module is based on a JavaScript widget developed by the MIT
SIMILE project. Several demo timelines demonstrating the full range of 
capabilities for this widget are available on the SIMILE project site at:
  http://code.google.com/p/simile-widgets/

The Views module is required to provide the list of nodes to render:
  http://drupal.org/project/views

This module provides the "Timeline" view type for the Views module.

In addition, a default views are provided to get you started quickly:
- timeline_nodes: plots any nodes by their creation dates.

Timeline events with only a start date are displayed as points on the
timeline view, whilst events with both a start and date are displayed as
continuous colored blocks (not dissimilar to Gantt chart entries).

REQUIREMENTS
------------
Please refer to the accompanying INSTALL.txt file.

INSTALLATION
------------
Please refer to the accompanying INSTALL.txt file.

USAGE
-----
After installing the module, do the following to create a new timeline view:

1. Make sure the `views' and `views_ui' modules are installed and enabled.
2. Go to administer >> views >> add.
3. Make a new view that is of the view type "Node"
4. Under Fields, add that fields that you would like to have populate the title, 
   start and end dates and body of the timeline popup.
   Note: Make sure that you are using exclusive date fields. Don't choose "Show from and to date"
5. Set any other views settings, such as the maximum number of nodes to
   show, filters to select only certain nodes or dates, etc. You'll likely
   want to disable the view's pager option in order to get all events to
   show up on the timeline.
6. Under "Basic settings" >> "Style" select the option "Timeline".
7. Configure the Timeline display (click the gear next to the style selector). 
   Set the orientation, display and field settings that control how the 
   timeline looks and behaves.
8. Save your view and navigate to it to test out the timeline. In case you
   don't see the timeline at all, that most likely indicates the view
   selection criteria didn't result in any nodes to be displayed.


TROUBLESHOOTING
---------------
To post bug reports or feature requests please use the following issue
trackers for the Drupal module and the MIT SIMILE widget, respectively:
- http://drupal.org/project/issues/timeline
- http://code.google.com/p/simile-widgets/issues/list
