#include <apop.h>

apop_model *truncated_model; //The model and function defined in its own listing.
apop_model *truncate_model(apop_model *in, double cutoff_in);

void round_trip(apop_model *m){
    //this copying and NULLifying is unnecessary; it's here so you know I'm not cheating.
    apop_model *clean_copy = apop_model_copy(m);
    clean_copy->parameters = NULL;                

    apop_data *draws = apop_model_draws(m, 2e5);
    apop_model_show(apop_estimate(draws, clean_copy));
}

int main(){
    printf(" N(1, 1)\n"); 
    apop_model *m = apop_model_set_parameters(apop_normal, 1, 1);
    round_trip(m);

    printf("\n truncated N(1, 1)\n"); 
    apop_model *mt = truncate_model(m, 0);
    round_trip(mt);

    printf("\n Beta(.7, 1.7)\n"); 
    apop_model *mb = apop_model_set_parameters(apop_beta, .7, 1.7);
    round_trip(mb);

    printf("\n Truncated Beta(.7, 1.7)\n"); 
    apop_model *mbt = truncate_model(mb, .2);
    round_trip(mbt);
}
