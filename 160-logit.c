#include <apop.h>

//The part about producing the ratios of effects from inputs.

int check_vote(apop_data *d, void *keep){return *(int*)keep != apop_data_get(d, .col=-1);}

double scale(gsl_vector *in){apop_vector_normalize(in); return 0;}

void print_effects(apop_model *logit_out, char *outfile, int keep){
    apop_data *effect = apop_data_copy(logit_out->data);
    for (int i=0; i< logit_out->parameters->matrix->size1; i++){
        Apop_col_v(effect, i, onecol);
        gsl_vector_scale(onecol, apop_data_get(logit_out->parameters, i));
    //printf("Mu for %s, %s: %g\n", outfile, effect->names->column[i], apop_mean(onecol));
    }

    apop_data_rm_rows(effect, .do_drop=check_vote, .drop_parameter=&keep);
    apop_data_prune_columns(effect, "contribs", "ideology");
    apop_map(effect, .fn_v=scale, .part='r');
    apop_data_sort(effect);
    printf(outfile);
    apop_data_print(apop_data_summarize(effect));
    apop_matrix_print(effect->matrix, .output_file=outfile);
}

//Elasticities

double observation_elasticity(apop_data *in){
    double this_xb = apop_data_get(in, .col=-1);
    double xb = apop_data_get(in, .col=0);
    return  this_xb/(1+exp(xb));
}

apop_data * elasticity(apop_data *d, apop_model *m, char *varname){
    apop_data *xbeta = apop_dot(d, m->parameters);
    //let us put the desired column in xbeta's ->vector.

    Apop_col_tv(d, varname, xcol);
    double this_beta=apop_data_get(m->parameters, .rowname=varname);
    gsl_vector_scale(xcol, this_beta);
    xbeta->vector = xcol;
    return apop_map(xbeta, .fn_r=observation_elasticity);
}

void elastic(apop_model *logit_out, char *outfile, int keep){
    apop_data *d = apop_data_copy(logit_out->data);
    apop_data_rm_rows(d, .do_drop=check_vote, .drop_parameter=&keep);
    apop_data *contribs_e = (elasticity(d, logit_out, "contribs"));
    apop_data *ideology_e = (elasticity(d, logit_out, "ideology"));
    contribs_e->names=NULL; //so they don't print.
    apop_data_print(apop_data_stack(contribs_e, ideology_e, .posn='c', .inplace='y')
                     , .output_file=outfile);
}

int main(){
    apop_text_to_db("amash_vote_analysis.csv", .tabname="amash");
    apop_data *d = apop_query_to_mixed_data("mmmt", "select 0, ideology,log(contribs+10) as contribs, vote from amash");
    apop_data_to_factors(d);
    Apop_model_add_group(apop_logit, apop_mle, /*.verbose='y',*/ .method="PR CG", .tolerance=1e-7);
    apop_model *logit_out = apop_estimate(d, apop_logit);
    apop_model_show(logit_out);

    print_effects(logit_out, "nnn", 1);
    print_effects(logit_out, "yyy", 0);
    elastic(logit_out, "elast.n", 1);
    elastic(logit_out, "elast.y", 0);
}
