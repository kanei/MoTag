/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
(function($) {

   $(document).ready(function () {
      $('#features_all').click(function() {
         $('#features input').attr('checked', true);
      });

      $('#features_none').click(function() {
         $('#features input').attr('checked', false);
      });

      $('#label_all').click(function() {
         $('#label input').attr('checked', true);
      });

      $('#label_none').click(function() {
         $('#label input').attr('checked', false);
      });
   });

  // $('input[name=features[U00]]').attr('checked', true);

   

})(jQuery);