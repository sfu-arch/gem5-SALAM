#include "../pathfinder_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

/*************************************************************************
* RISC-V Vectorized Version
* Author: Cristóbal Ramírez Lazo
* email: cristobal.ramirez@bsc.es
* Barcelona Supercomputing Center (2020)
*************************************************************************/

#define NUM_RUNS 1
#define ROWS 100
#define COLS 100
#define MIN(a, b) ((a)<=(b) ? (a) : (b))

extern "C" {
    void top();
}

void run(double *src, double *wall, double *dst)
{
    double min;
    double *temp;
    for (int j=0; j<NUM_RUNS; j++) {
        for (int t = 0; t < ROWS-1; t++) {
            temp = src;
            src = dst;
            dst = temp;
            //#pragma omp parallel for private(min)
            for(int n = 1; n < COLS; n++){
              min = src[t * COLS + n];
              if (n > 0)
                min = MIN(min, src[t * COLS + n-1]);
              if (n < COLS-2)
                min = MIN(min, src[t * COLS + n+1]);
              dst[(t+1)*COLS + n] = wall[(t+1)*COLS + n]*min;
            }
        }   
        //delete src;
    }
}

void top() {
  double *src = (double * )0x80c00000;
  double *src_grad = (double * ) ((uint64_t) src + ROWS * COLS * sizeof(double));
  double *wall = (double * )((uint64_t) src_grad + ROWS * COLS * sizeof(double));
  double *dst = (double * )((uint64_t) wall + ROWS * COLS * sizeof(double));
  
  
  // double *src = (double *) malloc(sizeof(double) * ROWS * COLS);
  // double *src_grad = (double *) malloc(sizeof(double) * ROWS * COLS);
  // double *wall = (double *) malloc(sizeof(double) * ROWS * COLS);
  double *wall_grad = (double *) malloc(sizeof(double) * ROWS * COLS);
  // double *dst = (double *) malloc(sizeof(double) * ROWS * COLS);
  // double *dst_grad = (double *) malloc(sizeof(double) * ROWS * COLS);
  // for (int i = 0; i < ROWS * COLS; i++) {
  //     src[i] = 1.0;
  //     src_grad[i] = 0.0;
  //     wall[i] = 1.0;
  //     dst[i] = 0.0;
  // }
  __enzyme_autodiff<void>(run, src, src_grad, wall, wall_grad, enzyme_const, dst);
}
