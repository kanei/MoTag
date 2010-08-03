<?php
/**
 * Description of MoTag_pending
 *
 */
function MoTag_show($pending = FALSE) {
   $output = '';

   if($pending == '1') {
      $pending = 1;
   }
   else
      $pending = 0;

   $header = array(
      "ID",
      "Program",
      "Machine",
      "Reg",
      "Train count",
      "Max iter",
      "Label parts",
      "Test count",
   );


   if($pending) {
      $header[] = "Commands";
   }
   else {
      $header = array_merge($header, array(
         "Words",
         "Labels",
         "Iterations",
         "Train t.",
         "Tag t.",
         "Model s.",
         "Succ. lab.",
         "Succ. sen.",
      ));
   }

   $rows = array();
   $result = db_query('SELECT * FROM {motag_tests} WHERE pending = '.$pending);

   // prepare data for saving
   foreach ($result as $record) {
      $row = array(
         $record->test_id,
         $record->program,
         $record->machine,
         $record->regularization,
         $record->training_count,
         $record->max_iterations,
         $record->label_columns,
         $record->testing_count,
      );

      if($pending) {
         $row[] = _test_options($record->test_id);
      }
      else {
         // prepare time
         $time = strtotime($record->training_finish) - strtotime($record->training_begin);
         $times = floor($time / 3600) . 'h ' . ($time%3600)/60 . 'm';

         $row = array_merge($row, array(
            $record->word_count,
            $record->label_count,
            $record->iterations,
            $times,
            $record->testing_time . 's',
            $record->model_size . 'B',
            $record->label_success_rate . '%',
            $record->sentence_success_rate . '%',
         ));
      }
       
      $rows[] = $row;
   }

   $output .= theme('table', array('header' => $header, 'rows' => $rows));

   return $output;
}

/**
 * Utility Function for building links for the product table
 */
function _test_options($id){
        $links = l(t('Finish'), "finish/$id");
        return $links;
}