#include <apop.h>
#include <stdbool.h>
#include "156-search_fns.c" //previous listing

void generate_agents(agent_s **grid, int grid_size, int pop_size, agent_s *out){
    for(int i=0; i< pop_size; i++){
        agent_s *a = out+i;
        *a = (agent_s){.rng = apop_rng_alloc(apop_opts.rng_seed++),
                       .type = (i % 2) ? 'A' : 'B'};
        do{ //find a vacant spot
            a->x = gsl_rng_uniform(a->rng) * grid_size;
            a->y = gsl_rng_uniform(a->rng) * grid_size;
        } while (gridpt(a->x, a->y));
        gridpt(a->x, a->y) = a;
    }
    out[pop_size] = (agent_s){}; //empty stopper.
}

int run_sim(double *durations, gsl_rng *r, apop_model *m){
    int grid_size = apop_data_get(m->parameters, 0);
    int pop_size = apop_data_get(m->parameters, 1);
    int done_ctr = 0, period = 1;
    pop_size *=2; //guarantee evenness.
    assert(pop_size <= pow(grid_size,2));

    agent_s alist[pop_size+1];
    agent_s *grid[grid_size * grid_size];
    memset(grid, 0, grid_size*grid_size*sizeof(agent_s*));
    generate_agents(grid, grid_size, pop_size, alist);
    
    do {
        for (agent_s *a=alist; a->rng; a++){
            if (a->done) continue;
            agent_s *b;
            if (a->type=='A' && (b=search_for_mate(a, grid, grid_size))){
                gridpt(a->x, a->y) = gridpt(b->x, b->y) = NULL;
                a->done = b->done = true;
                durations[done_ctr++] = period;
            }
            step(a, grid, grid_size);
        }
        period ++;
    } while (done_ctr < pop_size/2);
    return 0;
}

apop_model search_sim = {"A search on a grid", .vsize=2, .draw=run_sim};
