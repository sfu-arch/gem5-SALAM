#include "../logsum_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>


extern "C" {
  int enzyme_dupnoneed;
  void top();
}

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);


inline double max(double x, double y) {
    return (x > y) ? x : y;
}

static double logsumexp(const double * x, size_t n) {
  double A = x[0];
  for(int i=0; i<n; i++) {
    A = x[i];
  }
  double sema = 0;
  for(int i=0; i<n; i++) {
    sema += exp(x[i] - A);
  }
  return log(sema) + A;
}

static void enzyme_sincos(double *input, double *inputp, unsigned long n, unsigned long repeat) {
   
  for(int i=0; i<repeat; i++) {
    __enzyme_autodiff<void>(logsumexp, input, inputp, n);
  }

}

void top() {

  unsigned long n = 1;
  double *input = (double *) 0x80C00A00;
  double *inputp = (double *)( 0x80C00A00 + sizeof(double)*n);
  for(int i=0; i<n; i++) {
    input[i] = 3.1415926535 / (i+1);
  }
  enzyme_sincos(input, inputp, n, 10);
}