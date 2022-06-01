#ifndef _fft_h_
#define _fft_h_


/*
  A classy FFT and Inverse FFT C++ class library

  Author: Tim Molteno, tim@physics.otago.ac.nz

  Based on the article "A Simple and Efficient FFT Implementation in C++" by Volodymyr Myrnyy
  with just a simple Inverse FFT modification.

  Licensed under the GPL v3.
*/


// #include <math.h>

double sin(double x) {
  int i = 1;
  double cur = x;
  double acc = 1;
  double fact= 1;
  double pow = x;
  while (fabs(acc) > .00000001 &&   i < 100){
      fact *= ((2*i)*(2*i+1));
      pow *= -1 * x*x; 
      acc =  pow / fact;
      cur += acc;
      i++;
  }
  return cur; 
}
inline void swap(double* a, double* b) {
  double temp=*a;
  *a = *b;
  *b = temp;
}

static void recursiveApply(double* data, int iSign, unsigned N) {
  if (N == 1) return;
  recursiveApply(data, iSign, N/2);
  recursiveApply(data+N, iSign, N/2);

  double wtemp = iSign*sin(M_PI/N);
  double wpi = -iSign*sin(2*M_PI/N);
  double wpr = -2.0*wtemp*wtemp;
  double wr = 1.0;
  double wi = 0.0;
// #pragma clang loop unroll (full)
  for (unsigned i=0; i<N; i+=2) {
    int iN = i+N;

    double tempr = data[iN]*wr - data[iN+1]*wi;
    double tempi = data[iN]*wi + data[iN+1]*wr;

    // data[iN] = data[i]-tempr;
    // data[iN+1] = data[i+1]-tempi;
    // data[i] += tempr;
    // data[i+1] += tempi;

    wtemp = wr;
    wr += wr*wpr - wi*wpi;
    wi += wi*wpr + wtemp*wpi;
  }
}

inline static void scramble(double* data, unsigned N) {
  int j=1;
// #pragma clang loop unroll (full)
  for (int i=1; i<2*N; i+=2) {
    if (j>i) {
      // swap(&data[j-1], &data[i-1]);
      // swap(&data[j], &data[i]);
    }
    int m = N;
// #pragma clang loop unroll (full)
    while (m>=2 && j>m) {
      j -= m;
      m >>= 1;
    }
    j += m;
  }
}

inline static void rescale(double* data, unsigned N) {
  double scale = ((double)1)/N;
// #pragma clang loop unroll (full)
  for (unsigned i=0; i<2*N; i++) {
    data[i] *= scale;
  }
}

inline static void fft(double* data, unsigned N) {
  scramble(data, N);
  recursiveApply(data,1, N);
}

inline static void ifft(double* data, unsigned N) {
  scramble(data, N);
  recursiveApply(data,-1, N);
  rescale(data, N);
}


#endif /* _fft_h_ */
