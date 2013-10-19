#include <gsl/gsl_permute_vector.h>
#include <apop.h>

typedef struct {
    gsl_rng *r;
    apop_model *model;
} rowset_s;

double set_a_row(apop_data *in, void *in_struct, int index){
    rowset_s *s = in_struct;
    double draw;
    apop_draw(&draw, s->r, s->model);
    apop_data_fill(in, 2*index + draw, index, draw);
    return 0;
}


typedef struct {
    apop_data *data;
    gsl_vector *resid, *yhat;
} rep_s;

double one_replication(apop_data *ignore, void *in, int index){
    gsl_rng *r = apop_rng_alloc(index);
    rep_s *rs = in;
    gsl_permutation * p = gsl_permutation_alloc (rs->resid->size);
    gsl_permutation_init(p);
    gsl_ran_shuffle(r, p->data, rs->resid->size, sizeof(size_t));
    
    //Can't write to the input, so build a new one.
    //point to the original matrix of independents,
    //but generate a new dependent var in alt_data->vector.
    apop_data *alt_data = apop_data_alloc();
    alt_data->matrix = rs->data->matrix;
    alt_data->vector = apop_vector_copy(rs->resid);
    gsl_permute_vector(p, alt_data->vector);
    gsl_permutation_free(p);
    gsl_vector_add(alt_data->vector, rs->yhat);

    apop_model *mb = apop_estimate(alt_data, apop_ols);
    double out = apop_data_get(mb->parameters, .row=1, .col=-1);

    //Clean up. Even at this scale, this is optional.
    alt_data->matrix = NULL;
    apop_data_free(alt_data);
    apop_model_free(mb);
    gsl_rng_free(r);
    return out;
}

int main(){
    apop_opts.thread_count = 4;
    gsl_rng *r = apop_rng_alloc(123);
    apop_data *d = apop_data_alloc(101, 3);
    apop_map(d, .fn_rpi=set_a_row, 
                .param=&((rowset_s){.model=apop_model_set_parameters(apop_normal, 0, 10), .r=r}));
    apop_model *m = apop_estimate(d, apop_ols);
    apop_model_show(m);

    apop_data *rpage = apop_data_get_page(m->info, "<Predicted>");
    Apop_col_t(rpage, "Predicted", yhat)
    Apop_col_t(rpage, "Residual", resid)

    int runs = 10000;
    Apop_model_add_group(apop_ols, apop_parts_wanted);
    apop_data *out = apop_data_alloc(runs);
    apop_map(out, .fn_rpi=one_replication, .inplace='y',
                  .param = &((rep_s){.data=d, .yhat=yhat, .resid=resid}));

    printf("mean replicated coefficient = %g\n", apop_mean(out->vector));
}
