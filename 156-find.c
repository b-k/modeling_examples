#include <apop.h>
extern apop_model search_sim;
extern apop_model *weibull;

void one_run(int grid_size, int pop_size){
    printf("------ A run with a %i X %i grid and %i agents:\n", grid_size, grid_size, pop_size);
    search_sim.dsize = pop_size;
    apop_data_set(search_sim.parameters, 0, .val=grid_size);
    apop_data_set(search_sim.parameters, 1, .val=pop_size);
    apop_model *model_out = apop_estimate(apop_model_draws(&search_sim, 1000), weibull);
    apop_model_show(model_out);
}

apop_model *fuzz(apop_model sim){
    int draws = 100;
    gsl_rng *r = apop_rng_alloc(1);
    apop_model *prior = apop_model_stack(
                            apop_model_set_parameters(apop_normal, 10, 2),
                            apop_model_set_parameters(apop_normal, 10, 2));
    apop_data *outdata = apop_data_alloc(draws, weibull->vsize);
    double *params = sim.parameters->vector->data;
    for (int i=0; i< draws; i++){
        do {
            apop_draw(params, r, prior);
        } while (params[1]*2 > pow(params[0], 2));
        sim.dsize=params[1];
        apop_model *est = apop_estimate(apop_model_draws(&sim, 1000), weibull);
        Apop_row_v(outdata, i, onerow);
        gsl_vector_memcpy(onerow, est->parameters->vector);
        apop_model_free(est);
    }
    return apop_estimate(outdata, apop_pmf);
}

int main(){ 
    apop_prep(NULL, &search_sim);
    one_run(10, 10);
    one_run(100, 10);
    one_run(10, 45);
    apop_model *fuzzed = fuzz(search_sim);
    apop_data_print(fuzzed->data, .output_name="outdata");
}
