#include <apop.h>

typedef struct {
    double b, alpha, q1, q2;
} an_agent;

void draw(double *qs, gsl_rng *r, apop_model *m){
    double m1 = apop_data_get(m->parameters, 0);
    double m2 = apop_data_get(m->parameters, 1);
    double p1 = apop_data_get(m->parameters, 2);

    apop_model *ba_model = apop_model_stack(
            apop_model_set_parameters(apop_normal, m1, 1),
            apop_model_set_parameters(apop_normal, m2, 1)
            ); //leaks; don't care.

    //set up agents
    int agent_count=1000;
    an_agent a_list[agent_count];
    for(int i=0; i< agent_count; i++){
        double out[2];
        do {
            apop_draw(out, r, ba_model);
            a_list[i] = (an_agent){.b=out[0], .alpha=out[1]};
        } while (a_list[i].alpha <=0);
    }

    //agents decide
    qs[0]=qs[1]=0;
    for (int i=0; i< agent_count; i++){
        qs[0] += a_list[i].q1 = GSL_MIN(
                        pow(p1/a_list[i].alpha, 1./(1-a_list[i].alpha)),
                        a_list[i].b/p1);
        qs[1] += a_list[i].q2 = (a_list[i].b - p1*a_list[i].q1);
        //printf("%g %g\n", a_list[i].q1,a_list[i].q2);
    }
    qs[0] /= agent_count;
    qs[1] /= agent_count;
    apop_model_free(ba_model);
}

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
