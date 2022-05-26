#include "hw_defines.h"

void matvec_real() {
  volatile double  * x    = (double  *)MAT;
double r;
double u = 1.0;
#pragma clang loop unroll(full)
    for (int i = 0; i < N; i++) {
#pragma clang loop unroll(full)
        for (int j=i+1; j < N; j++) {
            r = x[i] - x[j];
            u *= -1.0 / (r + 0.001);
        }
    }
  x[0] = u;
}