#include <apop.h>

apop_model truncated_model; //these are from last episode.
apop_model *truncate_model(apop_model *in, double cutoff_in);

int main(){
    apop_model_print (
        apop_estimate(
             apop_update(
                apop_model_draws(
                    apop_model_mixture(
                        apop_model_set_parameters(apop_poisson, 2.8),
                        apop_model_set_parameters(apop_poisson, 2.0),
                        apop_model_set_parameters(apop_poisson, 1.3)
                    ), 
                    1e4
                ),
                truncate_model(
                    apop_model_set_parameters(apop_normal, 2, 1), 
                    0
                ), 
                apop_poisson
            )->data,
            apop_normal
        )
    , NULL);
}
