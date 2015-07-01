#include <apop.h>

typedef struct {
    double b, alpha, q1, q2;
} an_agent;

int draw(double *qs, gsl_rng *r, apop_model *m){
    double m1 = apop_data_get(m->parameters, 0);
    double m2 = apop_data_get(m->parameters, 1);
    double p1 = apop_data_get(m->parameters, 2);

    apop_model *ba_model = apop_model_cross(
            apop_model_set_parameters(apop_normal, m1, 1),
            apop_model_set_parameters(apop_normal, m2, 1)
            );

    //set up agents by drawing from the above model
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
    }
    qs[0] /= agent_count;
    qs[1] /= agent_count;
    apop_model_free(ba_model);
    return 0;
}
