<?php
/**
 * Description of MoTag_add
 *
 */
function MoTag_test_start($form, &$form_state) {
   drupal_add_css(drupal_get_path('module', 'MoTag') . '/css/general.css');
   drupal_add_js(drupal_get_path('module', 'MoTag') . '/js/start_test.js');

   $form['general'] = array(
           '#title' 		=> t("General informations"),
           '#prefix' 		=> '<div id="general">',
           '#suffix' 		=> '</div>',
           '#type' 		=> 'fieldset',
   );

   $form['general']['program'] = array(
           '#type' 		=> 'select',
           '#title' 		=> t('Program'),
           '#default_value' => 'crfsuite',
           '#options' => array(
                   'crfsuite' => t('CRFSuite'),
                   'crfpp' => t('CRF++'),
           ),
           '#required'		=> TRUE,
   );

   $form['general']['machine'] = array(
           '#type' 		=> 'select',
           '#title' 		=> t('Computer name'),
           '#options' => array(
                   'pcnlp3' => t('PC Nlp 3'),
                   'pcnlp4' => t('PC Nlp 4'),
                   'pcnlp5' => t('PC Nlp 5'),
                   'pcnlp6' => t('PC Nlp 6'),
                   'pcnlp7' => t('PC Nlp 7'),
                   'athena2' => t('Athena 2'),
                   'athena3' => t('Athena 5'),
                   'athena4' => t('Athena 6'),
           ),
           '#required'		=> TRUE,
   );

   $form['general']['description'] = array(
           '#type' => 'textarea',
           '#title' => t('Description'),
           '#required' => FALSE,
   );

   $form['training'] = array(
           '#title' 		=> t("Training settings"),
           '#prefix' 		=> '<div id="training">',
           '#suffix' 		=> '</div>',
           '#type' 		=> 'fieldset',
   );

   $form['training']['regularization'] = array(
           '#type' => 'select',
           '#title' => t('Regularization'),
           '#options' => array(
                   'l1' => t('L1'),
                   'l2' => t('L2'),
           ),
           '#required'		=> TRUE,
   );

   $form['training']['training_count'] = array(
           '#type' => 'textfield',
           '#title' => t('Sentence count'),
           '#default_value' => '10000',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $form['training']['max_iterations'] = array(
           '#type' => 'textfield',
           '#title' => t('Maximum iterations'),
           '#default_value' => '',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => FALSE,
   );

   $form['testing'] = array(
           '#title' 		=> t("Testing settings"),
           '#prefix' 		=> '<div id="testing">',
           '#suffix' 		=> '</div>',
           '#type' 		=> 'fieldset',
   );

   $form['testing']['testing_count'] = array(
           '#type' => 'textfield',
           '#title' => t('Sentence count'),
           '#default_value' => '7000',
           '#size' => 10,
           '#maxlength' => 10,
           '#required' => TRUE,
   );

   $result = db_query('SELECT * FROM {motag_features}');

   $features = array();
   foreach ($result as $item) {
      $features[$item->feature_id] = $item->caption;
   }

   $form['training']['features'] = array(
           '#type' => 'checkboxes',
           '#title' => t('Choose used features'),
           '#prefix' 		=> '<div id="features">',
           '#suffix' 		=> '</div><div id="features_buttons"><button type="button" id="features_all">Check all</button><button type="button" id="features_none">Uncheck all</button></div>',
           '#options' => $features,
   );

   $label = array();
   for($i = 1; $i<=16; $i++) {
      $label[$i] = $i;
   }

   $form['training']['label'] = array(
           '#type' => 'checkboxes',
           '#title' => t('Parts of label to use'),
           '#prefix' 		=> '<div id="label">',
           '#suffix' 		=> '</div><div id="label_buttons"><button type="button" id="label_all">Check all</button><button type="button" id="label_none">Uncheck all</button></div>',
           '#options' => $label,
   );


   $form['submit'] = array(
           '#type' 		=> 'submit',
           '#value' 		=> t('Start test'),
   );

   return $form;
}

function MoTag_test_start_validate($form, &$form_state) {
   $training_count = $form_state['values']['training_count'];
   $testing_count = $form_state['values']['testing_count'];

   if(!$training_count || !is_numeric($training_count))
      form_set_error('training_count', t('Please fill number of sentences to be processed in training procedure.'));

   if(!$testing_count || !is_numeric($testing_count))
      form_set_error('testing_count', t('Please fill number of sentences to be processed in testing procedure.'));

}

function MoTag_test_start_submit($form, &$form_state) {
   $output = '';

   /* Generate string for label 'x' for checked, '-' for not checked */
   $label_pieces = $form_state['values']['label'];
   $label = '';

   for($i = 1; $i <= 16; $i++) {
      if($label_pieces[$i])
         $label .= 'x';
      else
         $label .= '-';
   }

   //drupal_set_message('<pre>'. print_r($form_state['values']['features'], TRUE) .'</pre>');

   /* Prepare maximum number of iterations */
   $max_iterations = $form_state['values']['max_iterations'] ? $form_state['values']['max_iterations'] : 0;

   $test_id = db_insert('motag_tests')
           ->fields(array(
           'program' 		=> $form_state['values']['program'],
           'description' 	=> $form_state['values']['description'],
           'machine' 	=> $form_state['values']['machine'],
           'training_count' 	=> $form_state['values']['training_count'],
           'testing_count' 	=> $form_state['values']['testing_count'],
           'max_iterations' => $max_iterations,
           'regularization' => $form_state['values']['regularization'],
           'label_columns' => $label,
           'pending'  => true,
           ))
           ->execute();

   $output .= "Test was successfully added.<br />";

   $fcount = 0;

   /* Connect features with test */
   foreach ($form_state['values']['features'] as $item) {
      if(!$item)
         break;
      
      db_insert('motag_tests_features')
              ->fields(array(
              'test_id'   => $test_id,
              'feature_id'   => $item,
              ))
              ->execute();
      
      $fcount++;
   }

   $output .= $fcount . " features has been used.";

   drupal_set_message($output);
}