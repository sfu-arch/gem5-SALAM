#include "../gravity_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

#define N 15

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
// #pragma clang loop unroll(full)
    for (int i = 0; i < N; i++) {
// #pragma clang loop unroll(full)
        for (int j=i+1; j < N; j++) {
            r = x[i] - x[j];
            u *= -1.0 / (r + 0.001);
        }
    }
    return u;
}

void top() {

    volatile double * x = (double *)MAT;
    // volatile double * x = (double *)0x80C007D0;

    double *x_grad = (double *)(VEC);
    // double *x_grad = (double *)(0x80C003E8);

    // arrBase[0] = compute_U(1);
    __enzyme_autodiff<double>(ComputeU, x, x_grad) ;
}