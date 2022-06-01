#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

template<typename Return, typename... T>
Return __enzyme_autodiff(T...);


#include "fft.h"

void foobar(double* data, unsigned len) {
  fft(data, len);
  ifft(data, len);
}


extern "C" {
  int enzyme_dupnoneed;
}

static double foobar_and_gradient(unsigned len) {
    double *inp = new double[2*len];
    for(int i=0; i<2*len; i++) inp[i] = 2.0;
    double *dinp = new double[2*len];
    for(int i=0; i<2*len; i++) dinp[i] = 1.0;
    __enzyme_autodiff<void>(foobar, enzyme_dupnoneed, inp, dinp, len);
    double res = dinp[0];
    delete[] dinp;
    delete[] inp;
    return res;
}

static void enzyme_sincos(double inp, unsigned len) {
  double res2 = foobar_and_gradient(len);
}

unsigned max(unsigned A, unsigned B){
  if (A>B) return A;
  return B;
}

int main(int argc, char** argv) {

  unsigned N = 32;
  double inp = -2.1;

  for(unsigned iters=max(1, N>>5); iters <= N; iters*=32768) {
    enzyme_sincos(inp, iters);
  }
}
