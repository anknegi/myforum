<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on rubico_tasks being loaded from the database.
 *
 * This hook is invoked during $rubico_topic loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $rubico_topic entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_rubico_topic_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a $example_task is inserted.
 *
 * This hook is invoked after the $example_task is inserted into the database.
 *
 * @param ExampleTask $example_task
 *   The $example_task that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_rubico_topic_insert(RubicoTopic $rubico_topic) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('rubico_topic', $rubico_topic),
      'extra' => print_r($rubico_topic, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $example_task being inserted or updated.
 *
 * This hook is invoked before the $example_task is saved to the database.
 *
 * @param ExampleTask $example_task
 *   The $example_task that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_rubico_topic_presave(RubicoTopic $rubico_topic) {
  $rubico_topic->name = 'foo';
}

/**
 * Responds to a $example_task being updated.
 *
 * This hook is invoked after the $example_task has been updated in the database.
 *
 * @param ExampleTask $example_task
 *   The $example_task that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_rubico_topic_update(RubicoTopic $rubico_topic) {
  db_update('mytable')
    ->fields(array('extra' => print_r($rubico_topic, TRUE)))
    ->condition('id', entity_id('rubico_topic', $rubico_topic))
    ->execute();
}

/**
 * Responds to $example_task deletion.
 *
 * This hook is invoked after the $example_task has been removed from the database.
 *
 * @param ExampleTask $example_task
 *   The $example_task that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_rubico_topic_delete(RubicoTopic $rubico_topic) {
  db_delete('mytable')
    ->condition('pid', entity_id('rubico_topic', $rubico_topic))
    ->execute();
}

/**
 * Act on a example_task that is being assembled before rendering.
 *
 * @param $example_task
 *   The example_task entity.
 * @param $view_mode
 *   The view mode the example_task is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $example_task->content prior to rendering. The
 * structure of $example_task->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_rubico_topic_view($rubico_topic, $view_mode, $langcode) {
  $rubico_topic->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for example_tasks.
 *
 * @param $build
 *   A renderable array representing the example_task content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * example_task content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the example_task rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_example_task().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_rubico_topic_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on example_task_type being loaded from the database.
 *
 * This hook is invoked during example_task_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of example_task_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_rubico_topic_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a example_task_type is inserted.
 *
 * This hook is invoked after the example_task_type is inserted into the database.
 *
 * @param ExampleTaskType $example_task_type
 *   The example_task_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_rubico_topic_type_insert(RubicoTopicType $example_task_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('rubico_topic_type', $rubico_topic_type),
      'extra' => print_r($rubico_topic_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a example_task_type being inserted or updated.
 *
 * This hook is invoked before the example_task_type is saved to the database.
 *
 * @param ExampleTaskType $example_task_type
 *   The example_task_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_rubico_topic_type_presave(RubicoTopicType $rubico_topic_type) {
  $rubico_topic_type->name = 'foo';
}

/**
 * Responds to a example_task_type being updated.
 *
 * This hook is invoked after the example_task_type has been updated in the database.
 *
 * @param ExampleTaskType $example_task_type
 *   The example_task_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_rubico_topic_type_update(RubicoTopicType $rubico_topic_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($rubico_topic_type, TRUE)))
    ->condition('id', entity_id('rubico_topic_type', $rubico_topic_type))
    ->execute();
}

/**
 * Responds to example_task_type deletion.
 *
 * This hook is invoked after the example_task_type has been removed from the database.
 *
 * @param ExampleTaskType $example_task_type
 *   The example_task_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_rubico_topic_type_delete(RubicoTopicType $rubico_topic_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('rubico_topic_type', $rubico_topic_type))
    ->execute();
}

/**
 * Define default example_task_type configurations.
 *
 * @return
 *   An array of default example_task_type, keyed by machine names.
 *
 * @see hook_default_example_task_type_alter()
 */
function hook_default_rubico_topic_type() {
  $defaults['main'] = entity_create('rubico_topic_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default example_task_type configurations.
 *
 * @param array $defaults
 *   An array of default example_task_type, keyed by machine names.
 *
 * @see hook_default_example_task_type()
 */
function hook_default_rubico_topic_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
