#include <apop.h>
#include <stdbool.h>

typedef struct{
    gsl_rng *rng;
    int x, y;
    char type;
    bool done;
} agent_s;

#define gridpt(x, y) grid[(x)*grid_size + (y)]

#define xoff_yoff_loop(...)                     \
    for (int xoff=-1; xoff <=1; xoff++)         \
        for (int yoff=-1; yoff <=1; yoff++) {   \
            if (a->x + xoff >= grid_size || a->x + xoff < 0 || \
                a->y + yoff >= grid_size || a->y + yoff < 0 || (!xoff & !yoff) ) \
                continue;                       \
            __VA_ARGS__                         \
        }

agent_s *search_for_mate(agent_s *a, agent_s **grid, int grid_size){
    xoff_yoff_loop (
        agent_s *b = gridpt(a->x+xoff, a->y+yoff);
        if (b && b->type!=a->type) return b;
    )
    return NULL;
}

void step(agent_s *a, agent_s **grid, int grid_size){
    int open_ct = 0;
    xoff_yoff_loop (
        if (!gridpt(a->x+xoff, a->y+yoff)) open_ct++;
    )
    if (!open_ct) return; //landlocked, can't move
    int move = gsl_rng_uniform(a->rng) * open_ct;
    xoff_yoff_loop (
        if (!move--) {
            gridpt(a->x, a->y) = NULL;
            a->x += xoff;
            a->y += yoff;
            gridpt(a->x, a->y) = a;
            return;
        }
    )
}

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

void run_sim(double *durations, gsl_rng *r, apop_model *m){
    int grid_size = ((double*)m->more)[0];
    int pop_size = ((double*)m->more)[1];
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
                durations[done_ctr++] =period;
            }
            step(a, grid, grid_size);
        }
        period ++;
    } while (done_ctr < pop_size/2);
}

apop_model search_sim = {"A search on a grid", .draw=run_sim};
