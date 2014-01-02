#include "169-kappa_and_pi.h"

apop_data *kappa_and_pi(apop_data const *tab_in){
    apop_data *out = apop_data_alloc();
    Apop_stopif(!tab_in, out->error='n'; return out, 0, "NULL input. Returning output with 'n' error code.");
    Apop_stopif(!tab_in->matrix, out->error='m'; return out, 0, "NULL input matrix. Returning output with 'm' error code.");
    Apop_stopif(tab_in->matrix->size1 != tab_in->matrix->size2, out->error='s'; return out, 0, "Input rows=%zu; input cols=%zu; "
                    "these need to be equal. Returning output with error code 's'.", tab_in->matrix->size1, tab_in->matrix->size2);

    apop_data *tab = apop_data_copy(tab_in);
    double total = apop_matrix_sum(tab->matrix);
    gsl_matrix_scale(tab->matrix, 1./total);
    double p_o = 0, p_e = 0, scott_pe = 0, ia = 0, row_ent = 0, col_ent = 0;
    for (int c=0; c< tab->matrix->size1; c++){
        double this_obs = apop_data_get(tab, c, c);
        p_o += this_obs;

        Apop_row_v(tab, c, row);
        Apop_col_v(tab, c, col);
        double rsum = apop_sum(row);
        double csum = apop_sum(col);
        p_e += rsum * csum;
        scott_pe += pow((rsum+csum)/2, 2);

        ia += this_obs * log2(this_obs/(rsum * csum));
        row_ent -= rsum * log2(rsum);
        col_ent -= csum * log2(csum);
    }
    apop_data_free(tab);

    asprintf(&out->names->title, "Scott's π and Cohen's κ");
    apop_data_add_named_elmt(out, "total count", total);
    apop_data_add_named_elmt(out, "percent agreement", p_o);
    apop_data_add_named_elmt(out, "κ", ((p_e==1)? 0: (p_o - p_e) / (1-p_e) ));
    apop_data_add_named_elmt(out, "π", ((p_e==1)? 0: (p_o - scott_pe) / (1-scott_pe)));
    apop_data_add_named_elmt(out, "P_I", ia/((row_ent+col_ent)/2));
    apop_data_add_named_elmt(out, "Cohen's p_e", p_e);
    apop_data_add_named_elmt(out, "Scott's p_e", scott_pe);
    apop_data_add_named_elmt(out, "information in agreement", ia);
    apop_data_add_named_elmt(out, "row entropy", row_ent);
    apop_data_add_named_elmt(out, "column entropy", col_ent);
    return out;
}
