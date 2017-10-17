<?php

/**
 * Topic view callback.
 */
function rubico_topic_view($topic) {
  drupal_set_title(entity_label('rubico_topic', $topic));
  return entity_view('rubico_topic', array(entity_id('rubico_topic', $topic) => $topic), 'full');
}
