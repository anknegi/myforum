<?php

/**
 * Generates the topic type editing form.
 */
function rubico_topic_type_form($form, &$form_state, $topic_type, $op = 'edit') {

  

  if ($op == 'clone') {
    $topic_type->label .= '(cloned)';
    $topic_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Topic Type Title'),
    '#type' => 'textfield',
    '#default_value' => $topic_type->label,
    '#description' => t('The human-readable name of this topic type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($topic_type->type) ? $topic_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $topic_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'rubico_topic_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this topic type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => isset($topic_type->description) ? $topic_type->description : '',
    '#description' => t('Description about the topic type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save topic type'),
    '#weight' => 40,
  );

  if (!$topic_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete topic type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('rubico_topic_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing topic_type.
 */
function rubico_topic_type_form_submit(&$form, &$form_state) {
  $topic_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  rubico_topic_type_save($topic_type);

  // Redirect user back to list of topic types.
  $form_state['redirect'] = 'admin/structure/topic-types';
}

function rubico_topic_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/topic-types/' . $form_state['rubico_topic_type']->type . '/delete';
}

/**
 * Topic type delete form.
 */
function rubico_topic_type_form_delete_confirm($form, &$form_state, $topic_type) {
  $form_state['topic_type'] = $topic_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['topic_type_id'] = array('#type' => 'value', '#value' => entity_id('rubico_topic_type' ,$topic_type));
  return confirm_form($form,
    t('Are you sure you want to delete topic type %title?', array('%title' => entity_label('topic_type', $topic_type))),
    'topic/' . entity_id('rubico_topic_type' ,$topic_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Task type delete form submit handler.
 */
function rubico_topic_type_form_delete_confirm_submit($form, &$form_state) {
  $topic_type = $form_state['topic_type'];
  rubico_topic_type_delete($topic_type);

  watchdog('rubico_topic_type', '@type: deleted %title.', array('@type' => $topic_type->type, '%title' => $topic_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $topic_type->type, '%title' => $topic_type->label)));

  $form_state['redirect'] = 'admin/structure/topic-types';
}

/**
 * Page to select task Type to add new task.
 */
function rubico_topic_admin_add_page() {
  $items = array();
  foreach (rubico_topic_types() as $topic_type_key => $topic_type) {
    $items[] = l(entity_label('rubico_topic_type', $topic_type), 'topic/add/' . $topic_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of topic to create.')));
}

/**
 * Add new task page callback.
 */
function rubico_topic_add($type) {
  $topic_type = rubico_topic_types($type);

  $topic = entity_create('rubico_topic', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('topic_type', $topic_type))));

  $output = drupal_get_form('rubico_topic_form', $topic);

  return $output;
}

/**
 * Topic Form.
 */
function rubico_topic_form($form, &$form_state, $topic) {
  $form_state['topic'] = $topic;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $topic->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $topic->description,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $topic->uid,
  );

  field_attach_form('rubico_topic', $topic, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save topic'),
    '#submit' => $submit + array('rubico_topic_form_submit'),
  );

  // Show Delete button if we edit task.
  $topic_id = entity_id('rubico_topic' ,$topic);
  if (!empty($topic_id) && rubico_topic_access('edit', $topic)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('rubico_topic_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'rubico_topic_form_validate';

  return $form;
}

function rubico_topic_form_validate($form, &$form_state) {

}



/**
 * Topic submit handler.
 */
function rubico_topic_form_submit($form, &$form_state) {
  $topic = $form_state['topic'];

  entity_form_submit_build_entity('rubico_topic', $topic, $form, $form_state);

  rubico_topic_save($topic);

  $topic_uri = entity_uri('rubico_topic', $topic);

  $form_state['redirect'] = $topic_uri['path'];

  drupal_set_message(t('Topic %title saved.', array('%title' => entity_label('rubico_topic', $topic))));
}

function rubico_topic_form_submit_delete($form, &$form_state) {
  $topic = $form_state['topic'];
  $topic_uri = entity_uri('rubico_topic', $topic);
  $form_state['redirect'] = $topic_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function rubico_topic_delete_form($form, &$form_state, $topic) {
  $form_state['topic'] = $topic;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['topic_type_id'] = array('#type' => 'value', '#value' => entity_id('rubico_topic' ,$topic));
  $topic_uri = entity_uri('rubico_topic', $topic);
  return confirm_form($form,
    t('Are you sure you want to delete topic %title?', array('%title' => entity_label('rubico_topic', $topic))),
    $topic_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function rubico_topic_delete_form_submit($form, &$form_state) {
  $topic = $form_state['topic'];
  rubico_topic_delete($topic);

  drupal_set_message(t('Topic %title deleted.', array('%title' => entity_label('rubico_topic', $topic))));

  $form_state['redirect'] = '<front>';
}
