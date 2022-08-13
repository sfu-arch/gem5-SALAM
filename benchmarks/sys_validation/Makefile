BENCHMARKS= \
	fft \
	gemm \
	md-knn \
	nw \
	stencil2d \
	stencil3d \
	matdescent \
	matd2 \

CFLAGS=-O1 -Wall -Wno-unused-label

.PHONY: build clean all

build:
	@( for b in $(BENCHMARKS); do $(MAKE) CFLAGS="$(CFLAGS)" -C $$b; done )
	
clean:
	@( for b in $(BENCHMARKS); do $(MAKE) -C $$b clean || exit ; done )
	
all: clean build
