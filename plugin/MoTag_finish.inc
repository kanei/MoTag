<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
*/
function MoTag_finish($form, &$form_state, $id = FALSE) {
   $result = db_query('SELECT test_id,machine FROM {motag_tests}  WHERE pending = 1');

   $tests = array();
   foreach ($result as $item) {
      $tests[$item->test_id] = $item->test_id . ' - ' . $item->machine;
   }

   if(!$tests) {
      drupal_set_message("There are no tests running at the moment.");
      return $form;
   }

   $default_test = $id;

    $form['selection'] = array(
           '#title' 		=> t("Test selection"),
           '#type' 		=> 'fieldset',
   );

   $form['selection']['test_id'] = array(
           '#type' => 'select',
           '#title' => t('Running tests'),
           '#description' => t('Please select test you want to finish'),
           '#default_value' => $default_test,
           '#options' => $tests,
           '#required' => TRUE,
   );

   $form['stats'] = array(
           '#title' 		=> t("Test statistics"),
           '#type' 		=> 'fieldset',
   );

   $form['stats']['word_count'] = array(
           '#type' => 'textfield',
           '#title' => t('Number of words in training'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $form['stats']['label_count'] = array(
           '#type' => 'textfield',
           '#title' => t('Number of labels in training'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $form['stats']['iterations'] = array(
           '#type' => 'textfield',
           '#title' => t('Number of iterations'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $form['results'] = array(
           '#title' 		=> t("Test results"),
           '#type' 		=> 'fieldset',
   );

    $form['results']['training_begin'] = array(
           '#type' => 'textfield',
           '#title' => t('Date and time of start of training process'),
           '#default_value' => date('Y-m-d g:i'),
           '#size' => 20,
           '#maxlength' => 20,
           '#required' => TRUE,
   );

   $form['results']['training_finish'] = array(
           '#type' => 'textfield',
           '#title' => t('Date and time of end of training process'),
           '#default_value' => date('Y-m-d g:i'),
           '#size' => 20,
           '#maxlength' => 20,
           '#required' => TRUE,
   );

   $form['results']['features_start'] = array(
           '#type' => 'textfield',
           '#title' => t('Number of features in the beginning'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $form['results']['features_end'] = array(
           '#type' => 'textfield',
           '#title' => t('Number of features in the end'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $form['results']['testing_time'] = array(
           '#type' => 'textfield',
           '#title' => t('Time of testing'),
           '#description' => t('Fill in value in seconds.'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $form['results']['label_success_rate'] = array(
           '#type' => 'textfield',
           '#title' => t('Label success rate'),
           '#description' => t('Fill in float number separated by \'.\'.'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $form['results']['sentence_success_rate'] = array(
           '#type' => 'textfield',
           '#title' => t('Sentence success rate'),
           '#description' => t('Fill in float number separated by \'.\'.'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $form['results']['model_size'] = array(
           '#type' => 'textfield',
           '#title' => t('Size of model'),
           '#description' => t('Fill in value in bytes.'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 20,
           '#required' => TRUE,
   );


    $form['submit'] = array(
           '#type' 		=> 'submit',
           '#value' 		=> t('Finish'),
   );

   return $form;
}

function MoTag_finish_validate($form, &$form_state) {
   $word_count = $form_state['values']['word_count'];
   $label_count = $form_state['values']['label_count'];
   $iterations = $form_state['values']['iterations'];
   $training_begin = $form_state['values']['training_begin'];
   $training_finish = $form_state['values']['training_finish'];
   $testing_time = $form_state['values']['testing_time'];
   $features_start = $form_state['values']['features_start'];
   $features_end = $form_state['values']['features_end'];
   $model_size = $form_state['values']['model_size'];
   $label_success_rate = $form_state['values']['label_success_rate'];
   $sentence_success_rate = $form_state['values']['sentence_success_rate'];

   if(!$word_count || !is_numeric($word_count))
      form_set_error('word_count', t('Please fill number of words, which were processed in training.'));

   if(!$label_count || !is_numeric($label_count))
      form_set_error('word_count', t('Please fill number of labels, which were processed in training.'));

   if(!$iterations || !is_numeric($iterations))
      form_set_error('iterations', t('Please fill number of iterations it took to finish the training.'));

   if(!$training_begin || !strtotime($training_begin))
      form_set_error('training_begin', t('Please fill date and time of start of the training process in correct format.'));

   if(!$training_finish || !strtotime($training_finish))
      form_set_error('training_finish', t('Please fill date and time of the end of the training process in correct format.'));

   if(!$testing_time || !is_numeric($testing_time))
      form_set_error('testing_time', t('Please fill number of seconds it took to finish the tagging.'));

   if(!$features_start || !is_numeric($features_start))
      form_set_error('features_start', t('Please fill number of features present int the beginning of training process.'));

   if(!$features_end || !is_numeric($features_end))
      form_set_error('features_end', t('Please fill number of features present int the end of training process.'));

   if(!$model_size || !is_numeric($model_size))
      form_set_error('model_size', t('Please fill size of model in bytes.'));

   if(!$label_success_rate || !is_numeric($label_success_rate))
      form_set_error('label_success_rate', t('Please fill success rate of labels.'));

   if(!$sentence_success_rate || !is_numeric($sentence_success_rate))
      form_set_error('sentence_success_rate', t('Please fill success rate of sentences.'));
}

function MoTag_finish_submit($form, &$form_state) {
   $output = '';
   
   db_update('motag_tests')
          ->condition('test_id', $form_state['values']['test_id'])
          ->fields(array(
             'word_count' => $form_state['values']['word_count'],
             'label_count' => $form_state['values']['label_count'],
             'iterations' => $form_state['values']['iterations'],
             'training_begin' => $form_state['values']['training_begin'],
             'training_finish' => $form_state['values']['training_finish'],
             'testing_time' => $form_state['values']['testing_time'],
             'features_start' => $form_state['values']['features_start'],
             'features_end' => $form_state['values']['features_end'],
             'model_size' => $form_state['values']['model_size'],
             'label_success_rate' => $form_state['values']['label_success_rate'],
             'sentence_success_rate' => $form_state['values']['sentence_success_rate'],
             'pending' => 0,
          ))
          ->execute();

   $output = t('Test has been succesfully finished.');
   drupal_set_message($output);
}