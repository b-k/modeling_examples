#include <apop.h>

//Binomial draws. Input parameter gives the odds of a one; we always make 1,000 draws.
int rng(double *out, gsl_rng *r, apop_model *m){
    double prob = apop_data_get(m->parameters);
    int draws = 1000;
    *out = 0;
    for (int i=0; i< draws; i++) if (gsl_rng_uniform(r) < prob) *out += 1./draws;
    return 0;
}

//For centering a uniform distribution around a point. 
//Cut/pasted from the Apophenia documentation, but narrowed range to ±.0.8. This also took some tweaking.
//The uniform is not a good choice (and ruins the covariance estimate), but the premise
//was that we don't know the formula for a Normal distribution.
void set_midpoint(apop_data * in, apop_model *m){
    apop_data_set(m->parameters, 0, -1, apop_data_get(in)-0.07);
    apop_data_set(m->parameters, 1, -1, apop_data_get(in)+0.07);
}

//The probability: draw from the rng, smooth with a kernel density, calculate p.
long double p(apop_data *d, apop_model *m){
    int draw_ct = 100;
    apop_data *draws = apop_model_draws(m, draw_ct);
    apop_model *smoothed = apop_model_copy_set(apop_kernel_density, apop_kernel_density,
            .base_data =draws, .kernel=apop_uniform, .set_fn=set_midpoint);
    double out = apop_p(d, smoothed);
    apop_data_free(draws);
    apop_model_free(smoothed);
    return out;
}

apop_model *binom = &(apop_model){"Binomial draws (n=1000) via random draws", .vsize=1, .dsize=1, .draw=rng, .p=p};

//Now let's use the model: make five draws from it, find the probability of those 
//draws given various paramter values; find the optimal parameter given the input data.
int main(){
    apop_data *five_draws= apop_data_alloc(5,1);
    asprintf(&five_draws->names->title, "five draws");
    apop_model_draws(.model=apop_model_set_parameters(binom, 0.3), 
                     .draws=five_draws);
    apop_data_print(five_draws);
    printf("\n\n");

    #define showprob(p) printf("PDF(five draws|param=" #p ") = %g\n", \
            apop_p(five_draws, apop_model_set_parameters(binom, p)));

    showprob(0.2)
    showprob(0.25)
    showprob(0.3)
    showprob(0.35)
    showprob(0.5)

    printf("\n\n");
    Apop_model_add_group(binom, apop_mle, .step_size=0.1, /*.method="NM simplex",*/
        .tolerance=1e-7, /*.verbose='y',*/ .starting_pt=(double[]){0.4});
    apop_model_print(apop_estimate(five_draws, binom), NULL);

}
