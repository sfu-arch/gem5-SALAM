#include "hw_defines.h"

#define N 2000
#define M 2000
#define ITERS 1000
#define RATE 0.00000001

void matdecent() {
//   double *out = (double*)malloc(sizeof(double)*N);
  //double *out = new double[N];
// volatile double  * real         = (double  *)REAL;
volatile double  * mat    = (double  *)MAT;
volatile double  * vec    = (double  *)VEC;
volatile double  * out     = (double  *)OUT;

  for (int i=0; i<N; i++) {
    out[i] = 0;
    for (int j=0; j<M; j++) {
        out[i] += mat[i*M+j] * vec[j];
    }
  }
  double sum = 1;
  for (int i=0; i<N; i++) {
    sum *= out[i] * out[i];
  }
//   free(out);
  //delete[] out;
}

// void fft() {

//     // volatile uint8_t * realbase     = (uint8_t *)REALADDR;
//     // volatile uint8_t * imgbase      = (uint8_t *)IMGADDR;
//     // volatile uint8_t * realtwidbase = (uint8_t *)REALTWIDADDR;
//     // volatile uint8_t * imgtwidbase  = (uint8_t *)IMGTWIDADDR;
//     volatile double  * real         = (double  *)REAL;
//     volatile double  * img          = (double  *)IMG;
//     volatile double  * real_twid    = (double  *)REALTWID;
//     volatile double  * img_twid     = (double  *)IMGTWID;

//     int even, odd, span, log, rootindex;
//     double temp;

//     log = 0;

//     outer:
//     for (span=FFT_SIZE>>1; span; span>>=1, log++){
//         inner:
//         for (odd=span; odd<FFT_SIZE; odd++){
//             odd |= span;
//             even = odd ^ span;

//             temp = real[even] + real[odd];
//             real[odd] = real[even] - real[odd];
//             real[even] = temp;

//             temp = img[even] + img[odd];
//             img[odd] = img[even] - img[odd];
//             img[even] = temp;

//             rootindex = (even<<log) & (FFT_SIZE - 1);
//             if (rootindex){
//                 temp = real_twid[rootindex] * real[odd] -
//                     img_twid[rootindex]  * img[odd];
//                 img[odd] = real_twid[rootindex]*img[odd] +
//                     img_twid[rootindex]*real[odd];
//                 real[odd] = temp;
//             }
//         }
//     }
// }
