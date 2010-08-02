<?php
/**
 * Description of MoTag_pending
 *
 */
function MoTag_show_pending($command = FALSE, $object = FALSE) {
   $output = '';

   $header = array("Program", "Machine", "Reg", "Train count", "Max iter", "Label parts", "Test count", "Commands");
   $rows = array();
   $result = db_query('SELECT * FROM {motag_tests}');
   // prepare data for saving
   foreach ($result as $record) {
      $row = array(
         $record->program,
         $record->machine,
         $record->regularization,
         $record->training_count,
         $record->max_iterations,
         $record->label_columns,
         $record->testing_count,
         _test_options($record->test_id));
      $rows[] = $row;
   }

   $output .= theme('table', array('header' => $header, 'rows' => $rows));

   return $output;
}

/**
 * Utility Function for building links for the product table
 */
function _test_options($id){
        $links = l(t('Finish'), "show_pending/finish/$id");
        return $links;
}