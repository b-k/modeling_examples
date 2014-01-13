CFLAGS =-g -Wall -O3  `pkg-config --cflags apophenia`
LDLIBS=`pkg-config --libs apophenia` 
CC=c99

Executables=149-roundtrip 150-update 153-rng_est 154-demand_curve 156-find 158-fexact 158-fexact2 158-fscaled 160-logit 161-make_matrix 162-pca 163-de_xtab 165-boot-threaded 169-intercoder

all: $(Executables)
clean: 
	rm -f $(Executables)

149-roundtrip: 149-truncate.o
150-update: 149-truncate.o
154-demand_curve: 154a-demand_curve.o
156-find: 156-search.o 156-weibull.o
169-intercoder: 169-kappa_and_pi.o

plot_search: 156-find
	./156-find
	sed -e "1iplot '-'" -e '/Demand/,$$d' -e '$$ae' < outdata | /bin/gnuplot --persist

plot_logits: 160-logit
	./160-logit
	. 160-make_plots
