$(function () {
    $('[data-toggle="tooltip"]').tooltip();
});

$(document).ready(function () {
    $('#lang_menu').onchange(function () {
        if ($('#lang_menu').val() == 'other') {
            this.form['other'].style.visibility='visible'
        }
        else {
            this.form['other'].style.visibility='hidden'
        }
    });
});

$("input.datepicker").datepicker({
    format: "yyyy",
    viewMode: "years",
    minViewMode: "years",
    width: '100%'
});

$(document).ready(function () {
    $('#film_table').DataTable(
        {
            "pageLength": 5,
            "lengthMenu": [5, 10, 15, 20, 50, 100, 500],
        }
    );
  });

  $(document).ready(function () {
    $('#staff_table').DataTable(
        {
            "pageLength": 5,
            "lengthMenu": [5, 10, 15, 20, 50, 100, 500],
        }
    );
  });