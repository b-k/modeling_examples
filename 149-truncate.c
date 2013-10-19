#include <apop.h>

double cutoff; //A global variable, so you know this isn't for production use.

double under_cutoff(double in){ return (in < cutoff); }

long double like(apop_data *d, apop_model *m){
    double any_under_cutoff = apop_map_sum(d, .fn_d=under_cutoff, .part='a');
    if (any_under_cutoff) return -INFINITY;

    //apop_cdf wants an apop_data set; we have a bare double
    gsl_vector_view gv = gsl_vector_view_array(&cutoff, 1);

    return apop_log_likelihood(d, m->more) 
        - (d->matrix ? d->matrix->size1 : d->vector->size) * log(1 - apop_cdf(&(apop_data){.vector=&gv.vector}, m->more));
}

void r(double *out, gsl_rng *r, apop_model *m){
    do apop_draw(out, r, m->more); while (*out < cutoff);
}

void prep(apop_data *d, apop_model *m){
    apop_model *base_model = m->more;
    apop_prep(d, base_model);
    m->vsize = base_model->vsize;
    m->msize1 = base_model->msize1;
    m->msize2 = base_model->msize2;
    m->parameters = base_model->parameters;
}

apop_model *truncated_model = &(apop_model){"A truncated univariate model", .log_likelihood= like, .draw=r, .prep=prep};

apop_model *truncate_model(apop_model *in, double cutoff_in){
    cutoff = cutoff_in;
    apop_model *out = apop_model_copy(truncated_model);
    out->more = in;
    out->dsize= in->dsize;
    out->constraint= in->constraint;
    return out;
}
