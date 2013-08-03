CFLAGS =-g -Wall -O3  `pkg-config --cflags apophenia`
LDLIBS=`pkg-config --libs apophenia` 
CC=c99


all: 149-roundtrip 150-update 153-rng_est 154-demand_curve 156-find 158-fexact 158-fexact2 158-fscaled

149-roundtrip: 149-truncate.o
150-update: 149-truncate.o

156-find: 156-search.o

plot_demand: 154-demand_curve
	./154-demand_curve | sed "1iplot '-'"| sed '$$ae' | /bin/gnuplot --persist
