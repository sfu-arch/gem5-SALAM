BENCHMARKS=\
	bfs \
	fft \
	enzyme\
	fir \
	gemm \
	md-grid \
	md-knn \
	nw \
	spmv \
	stencil2d \
	stencil3d \
	vadd \
	matdescent\

CFLAGS=-O1 -Wall -Wno-unused-label

.PHONY: build clean all

build:
	@( for b in $(BENCHMARKS); do $(MAKE) CFLAGS="$(CFLAGS)" -C $$b; done )
	
clean:
	@( for b in $(BENCHMARKS); do $(MAKE) -C $$b clean || exit ; done )
	
all: clean build
