#include "../fft_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>
#include "fft.h"

#define N 1
#define LEN 1>>5
extern "C" {
  int enzyme_dupnoneed;
  void top();
}

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);


inline void foobar(double* data, unsigned len) {
  fft(data, len);
  ifft(data, len);
}


inline static double foobar_and_gradient(unsigned len) {
    // double *inp = new double[2*len];
    // double *inp = (double*) malloc (2*(LEN)*sizeof(double));
    // double *inp = (double*) 0x80C00A00;
    double *inp = (double*) ((uint64_t) 0x80C0AB80 + (2*(LEN)*sizeof(double)));

    // for(int i=0; i<2*LEN; i++) inp[i] = 2.0;
    // double *dinp = new double[2*len];
    // double *dinp = (double*) malloc (2*len*sizeof(double));
    double *dinp = (double*) ((uint64_t) 0x80C0AB80 + (2*(LEN)*sizeof(double)));

    // for(int i=0; i<2*len; i++) dinp[i] = 1.0;
    __enzyme_autodiff<void>(foobar, enzyme_dupnoneed, inp, dinp, len);
    double res = dinp[0];
    // delete[] dinp;
    // delete[] inp;
    return res;
}

unsigned max(unsigned A, unsigned B){
  if (A>B) return A;
  return B;
}

void top() {
  // for(unsigned iters=max(1, N>>5); iters <= N; iters*=32768) {
  foobar_and_gradient(LEN);
  //     break;
  // }
}