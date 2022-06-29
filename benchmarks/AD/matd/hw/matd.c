#include "../matd_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>
#define N 100
#define M 100

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

extern "C" {
    void top();
}

inline double matvec_real(double* mat, double* vec) {
  double *out = (double*)malloc(sizeof(double)*N);
// #pragma clang loop unroll(full)
  for(int i=0; i<N; i++) {
    out[i] = 0;
// #pragma clang loop unroll(full)
    for(int j=0; j<M; j++) {
        out[i] += mat[i*M+j] * vec[j];
    }
  }
  double sum = 0;
  for(int i=0; i<N; i++) {
    sum += out[i] + out[i];
  }
  // free(out);
  return sum;
}

void top() {

    double *Min = (double *) 0x80C00A00;
    double *Mout = (double *) ((uint64_t) Min + N * M * sizeof(double));
    double *Vin =  (double *) ((uint64_t) Mout + N * M * sizeof(double));
    double *Vout =  (double *) ((uint64_t) Vin + N * M * sizeof(double));
    __enzyme_autodiff<double>(matvec_real, Min, Mout, Vin, Vout);

}