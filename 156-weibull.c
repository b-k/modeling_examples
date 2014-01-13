#include <apop.h>

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

apop_model *weibull = &(apop_model){"The Weibull", .vsize=2, .log_likelihood = weibull_ll,
            .constraint=positive_params};
