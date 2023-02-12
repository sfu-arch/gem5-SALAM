#include "../gravity_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

#ifndef N
#define N 128
#endif
#define HALFN N/2
extern "C" {
    void top();
}

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

double dt = 0.001;

double ComputeU(double *x) {
    double r;
    double u = 1.0;
    // #pragma clang loop unroll_count(4)
    for (int i = 0; i < HALFN; i++) {
        #pragma clang loop unroll_count(32)
        for (int j=0; j < HALFN; j++) {
            r = x[i*(HALFN)+j ] - x[j*HALFN+i];
            u *= -1.0 / (r + 0.001);
        }
    }
    // for (int i = 0; i < N/2; i++) {
    //     for (int j=i+1; j < N/2; j++) {
    //         u *= -u * x[i];
    //     }
    // }
    return u;
}

void top() {

    double * x = (double *)0x80c00000;
    double *x_grad = (double *)((uint64_t) x + N * N * sizeof(double));
    __enzyme_autodiff<double>(ComputeU, x, x_grad) ;
    // ComputeU(x);
    x[0] = x[1];
}