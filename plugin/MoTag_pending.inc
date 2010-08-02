<?php

/**
 *
 * @param <type> $form
 * @param <type> $form_state
 * @return <type>
 */
function WebNAPTR_create($form, &$form_state) {
   drupal_add_js(drupal_get_path('module', 'WebNAPTR') . '/js/WebNAPTR.js');
   drupal_add_js(drupal_get_path('module', 'WebNAPTR') . '/js/WebNAPTR_module.js');


   $form['order'] = array(
           '#type'           => 'textfield',
           '#title'          => t('Order'),
           '#description'    => t('A 16-bit unsigned integer specifying the order in which the NAPTR records will be processed.'),
           '#default_value'  => 50,
           '#required'       => TRUE,
   );

   $form['preference'] = array(
           '#type'           => 'textfield',
           '#title'          => t('Preference'),
           '#description'    => t('A 16-bit unsigned integer that specifies the order in which NAPTR records with equal "order" values should be processed.'),
           '#default_value'  => 50,
           '#required'       => TRUE,
   );

   $form['flags'] = array(
           '#title' 		=> t('Flags'),
           '#type' 		=> 'fieldset',
           '#prefix'         => '<div id="flags_fs">',
           '#suffix'         => '</div>',
   );

   $form['flags']['s'] = array(
           '#type'           => 'radio',
           '#title'          => 'S',
           '#description'    => t('Next lookup should be for SRV records.'),
           '#parents'        => array('flags'),
           '#return_value'   => 's',
           '#ajax'           => array(
                   'callback'      => 'services_callback',
                   'wrapper'       => 'service_settings',
           ),
   );

   $form['flags']['a'] = array(
           '#type'           => 'radio',
           '#title'          => 'A',
           '#description'    => t('Next lookup should be for either an A, AAAA, or A6 record.'),
           '#parents'        => array('flags'),
           '#return_value'   => 'a',
           '#ajax'           => array(
                   'callback'      => 'services_callback',
                   'wrapper'       => 'service_settings',
           ),
   );

   $form['flags']['u'] = array(
           '#type'           => 'radio',
           '#title'          => 'U',
           '#description'    => t('The "U" flag means that the next step is not a DNS lookup but that the output of the Regexp field is an URI.'),
           '#parents'        => array('flags'),
           '#return_value'   => 'u',
            '#ajax'           => array(
                   'callback'      => 'services_callback',
                   'wrapper'       => 'service_settings',
           ),
   );

   $form['flags']['p'] = array(
           '#type'           => 'radio',
           '#title'          => 'P',
           '#description'    => t('Remainder of the application side algorithm shall be carried out in a Protocol-specific fashion.'),
           '#parents'        => array('flags'),
           '#return_value'   => 'p',
           '#ajax'           => array(
                   'callback'      => 'services_callback',
                   'wrapper'       => 'service_settings',
           ),
   );

  $form['services'] = array(
           '#title' 		=> t('Services'),
           '#type' 		=> 'fieldset',
   );

   $form['services']['e2u_sip'] = array(
           '#type'           => 'radio',
           '#title'          => 'E2U+sip',
           '#description'    => t('ENUM system for translating telephone number to URI.'),
           '#parents'        => array('services'),
           '#return_value'   => 1,
           '#ajax'           => array(
                   'callback'      => 'services_callback',
                   'wrapper'       => 'service_settings',
           ),
   );

   $form['services']['e2u_email'] = array(
           '#type'           => 'radio',
           '#title'          => 'E2U+email',
           '#description'    => t('ENUM system for translating telephone number to email.'),
           '#parents'        => array('services'),
           '#return_value'   => 2,
           '#ajax'           => array(
                   'callback'      => 'services_callback',
                   'wrapper'       => 'service_settings',
           ),
   );

   $form['service_settings'] = array(
           '#title'          => t('Service settings'),
           '#type'           => 'fieldset',
           '#prefix'         => '<div id="service_settings">',
           '#suffix'         => '</div>',
   );

   if(isset($form_state['values']['services']) && ($form_state['values']['services'] == 1 || $form_state['values']['services'] == 2 )) {
      $form['service_settings']['sip_telnum'] = array(
              '#type'           => 'textfield',
              '#title'          => t('Telephone number'),
              '#description'    => t('Write telephone number without +'),
              '#required'       => TRUE,
      );

      $form['service_settings']['sip_uri'] = array(
              '#type'           => 'textfield',
              '#title'          => t('SIP URI'),
              '#description'    => t(''),
              '#required'       => TRUE,
      );

      if(!isset($form_state['values']['flags']) || $form_state['values']['flags'] != 'u')
        $form['service_settings']['replacement'] = array(
              '#type'           => 'textfield',
              '#title'          => t('Replacement domain address'),
              '#description'    => t(''),
              '#required'       => TRUE,
      );
   }

   $form['result'] = array(
           '#type'             => 'textarea',
           '#title'            => t('Final NAPTR record'),
           '#disabled'         => true,
           '#prefix'           => '<div id="result">',
           '#suffix'           => '</div>',
   );

   $form['submit'] = array(
           '#type'           => 'submit',
           '#value'          => t('Submit'),
           '#prefix'         => '<div id="submit">',
           '#suffix'         => '</div>',
   );

   return $form;
}

function services_callback($form, $form_state) {
   //drupal_set_message('<pre>'. print_r($form_state['values'], TRUE) .'</pre>');
   return $form['service_settings'];
}
