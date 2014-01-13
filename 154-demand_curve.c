#include <apop.h>
void draw(double *qs, gsl_rng *r, apop_model *m); //from previous listing.

//for the Kernel density: center a Uniform distribution around a datum
void set_fn(apop_data *d, apop_model *m){
    Apop_row_v(d, 0, onerow);
    gsl_vector_memcpy(m->parameters->vector, onerow);
}

long double p(apop_data *d, apop_model *m){
    apop_multivariate_normal->vsize =
    apop_multivariate_normal->msize1=
    apop_multivariate_normal->msize2= 2;
    apop_model *kernel = apop_model_set_parameters(apop_multivariate_normal,
            0,   1, 0,
            0,   0, 1);

    apop_model *smoothed = apop_model_copy_set(apop_kernel_density, apop_kernel_density,
            .base_data = apop_model_draws(m, 500), .kernel=kernel, .set_fn=set_fn);
    double out = apop_p(d, smoothed);
    apop_data_free(smoothed->data);
    apop_model_free(smoothed);
    return out;
}

apop_model *demandside = &(apop_model){"Demand given price", .vsize=3, .dsize=2, .draw=draw, .p=p};

int main(){
    apop_data *draws = apop_model_draws(apop_model_set_parameters(demandside, 2.6, 1.6, 1.2), .count=20);
    apop_data_show(draws);

    Apop_settings_add_group(demandside, apop_mle, .tolerance=1e-5, .dim_cycle_tolerance=1e-3);
    apop_model_show(apop_estimate(draws, demandside));
}
