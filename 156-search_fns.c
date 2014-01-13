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

