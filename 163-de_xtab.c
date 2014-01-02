#include <apop.h>

int main(){
    apop_data *d = apop_text_to_data("163-xtab",.delimiters=" ",.has_row_names='y', .has_col_names='y');
    apop_data_print(d);

    //de-crosstab into the database. Arguments are names for the db.
    apop_crosstab_to_db(d, "wbdata", "country", "year", "expectancy");

    //pull from the database; write to file.
    sprintf(apop_opts.db_name_column, "country");
    apop_data_print(apop_query_to_data("select * from wbdata"), .output_file="unxtabbed");
    
/*   oops: getting ahead of myself. Apop_matrix_row_t will be added shortly.
    printf("\nWhile we're here, a moving average for Algeria:\n");
    Apop_row_tv(d, "Algeria", af);
    apop_vector_show(apop_vector_moving_average(af,2));
    */
}
