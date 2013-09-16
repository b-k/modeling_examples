#include <apop.h>
extern apop_model search_sim;

double one_weibull(double d, void *params){
    double lambda = apop_data_get(params, 0);
    double k = apop_data_get(params, 1);
    return logl(k) - logl(lambda)
        + (k-1)*(logl(d) - logl(lambda))
        - powl(d/lambda, k);
}

static long double positive_params(apop_data *data, apop_model *v){
    return apop_linear_constraint(v->parameters->vector);
}

long double weibull_ll(apop_data *d, apop_model *m){
    return apop_map_sum(d, .param = m->parameters, .fn_dp=one_weibull, .part='a');
}

apop_model weibull = {"The Weibull", .vsize=2, .log_likelihood = weibull_ll,
            .constraint=positive_params};

void one_run(int grid_size, int pop_size){
    printf("------ A run with a %i X %i grid and %i agents:\n", grid_size, grid_size, pop_size);
    search_sim.dsize = pop_size;
    double params[2];
    search_sim.more = params;
    params[0] = grid_size;
    params[1] = pop_size;
    apop_model *model_out = apop_estimate(apop_model_draws(&search_sim, 1000), weibull);
    apop_model_show(model_out);
}

apop_model *fuzz(apop_model sim){
    int draws = 100;
    gsl_rng *r = apop_rng_alloc(1);
    apop_model *prior = apop_model_stack(
                            apop_model_set_parameters(apop_normal, 10, 2),
                            apop_model_set_parameters(apop_normal, 10, 2));
    apop_data *outdata = apop_data_alloc(draws, weibull.vsize);
    apop_prep(NULL, &sim);
    double params[2];
    sim.more = params;
    for (int i=0; i< draws; i++){
        do {
        apop_draw(params, r, prior);
        } while (params[1]*2 > pow(params[0], 2));
        sim.dsize=params[1];
        apop_model *est = apop_estimate(apop_model_draws(&sim, 1000), weibull);
        Apop_matrix_row(outdata->matrix, i, onerow);
        gsl_vector_memcpy(onerow, est->parameters->vector);
        apop_model_free(est);
    }
    return apop_estimate(outdata, apop_pmf);
}

int main(){ 
    one_run(10, 10);
    one_run(100, 10);
    one_run(10, 45);

    apop_model *fuzzed = fuzz(search_sim);
    apop_data_print(fuzzed->data,.output_file="outdata");
}
