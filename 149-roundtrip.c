#include <apop.h>

apop_model truncated_model; //these two will be defined in the next file.
apop_model *truncate_model(apop_model *in, double cutoff_in);

void round_trip(apop_model *m){
    apop_data *draws = apop_model_draws(m, 2e5);
    //Uncomment these two lines if you want to look at the data and have Gnuplot installed:
    //Apop_col(draws, 0, v);
    //apop_plot_histogram(v, .bin_count=100, .output_pipe=popen("gnuplot --persist", "w"));

    //this copying and NULLifying is unnecessary; it's just so you know I'm not cheating.
    apop_model *clean_copy = apop_model_copy(m);
    clean_copy->parameters = NULL;                

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
    m = apop_model_set_parameters(apop_beta, .7, 1.7);
    round_trip(m);

    printf("\n Truncated Beta(.7, 1.7)\n"); 
    mt = truncate_model(m, .2);
    round_trip(mt);
}
