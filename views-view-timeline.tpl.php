<div class="<?php print $class; ?>-wrapper">
  <noscript>This page uses Javascript to show you a Timeline. Please enable Javascript in your browser to see the full page. Thank you.</noscript>
  <div id="<?php print $id; ?>" class="<?php print $class; ?>" style="<?php print $style; ?>" style="height: 150px; border: 1px solid #aaa"></div>
  <?php if (isset($controls)): ?>
    <div id="<?php print $id; ?>-controls" class="<?php print $class; ?>-controls"></div>
  <?php endif; ?>
</div>