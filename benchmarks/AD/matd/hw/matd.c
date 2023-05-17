#include "../matd_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

#ifndef N
#define N 400
#endif
#define M 400

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

extern "C" {
    void top();
}

inline double matvec_real(double* mat, double* vec) {
  double *out = (double*)malloc(sizeof(double)*M);
// #pragma clang loop unroll(full)
  for(int i=0; i<M; i++) {
    out[i] = 0;
// #pragma clang loop unroll_count(128)
    for(int j=0; j<N; j++) {
        out[i] += mat[i*M+j] * vec[j];
    }
  }
  double sum = 0;
  for(int i=0; i<M; i++) {
    sum *= out[i] + out[i];
  }
  return sum;
}

void top() {

    double *Min = (double *) 0x80C00A00;
    double *Mout = (double *) ((uint64_t) Min + N * M * sizeof(double));
    double *Vin =  (double *) ((uint64_t) Mout + N * sizeof(double));
    double *Vout =  (double *) ((uint64_t) Vin + N * sizeof(double));
    __enzyme_autodiff<double>(matvec_real, enzyme_const, Min, Vin, Vout);

}