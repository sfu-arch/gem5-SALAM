#include "../somier_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>


#ifndef N
#define N 16
#endif
extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

void compute_forces(int n, double *X, double *F)
{
   int N2 = N*N;
   int N3 = N*N*N;
   double spring_K=10.0;

   for (int i=1; i<N-1; i++) {
      for (int j=1; j<N-1; j++) {
         #ifdef UNROLL_COUNT
         #pragma clang loop unroll_count(UNROLL_COUNT)
         #endif
         for (int k=1; k<N-1; k++) {
            {
               double dx, dy, dz, dl, spring_F, FX, FY,FZ;
               int neig_i = i-1;
               int neig_j = j;
               int neig_k = k;
               dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
               dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
               dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];
               dl = (dx*dx + dy*dy + dz*dz);
               spring_F = 0.25 * spring_K*(dl-1);
               FX = spring_F * dx/dl; 
               FY = spring_F * dy/dl;
               FZ = spring_F * dz/dl; 
               F[0*N3+neig_i * N2 + neig_j*N+neig_k] += FX;
               F[1*N3+neig_i * N2 + neig_j*N+neig_k] += FY;
               F[2*N3+neig_i * N2 + neig_j*N+neig_k] += FZ;
            }
            {
               double dx, dy, dz, dl, spring_F, FX, FY,FZ;
               int neig_i = i+1;
               int neig_j = j;
               int neig_k = k;
               dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
               dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
               dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];
               dl = (dx*dx + dy*dy + dz*dz);
               spring_F = 0.25 * spring_K*(dl-1);
               FX = spring_F * dx/dl; 
               FY = spring_F * dy/dl;
               FZ = spring_F * dz/dl; 
               F[0*N3+neig_i * N2 + neig_j*N+neig_k] += FX;
               F[1*N3+neig_i * N2 + neig_j*N+neig_k] += FY;
               F[2*N3+neig_i * N2 + neig_j*N+neig_k] += FZ;
            }
            {
               double dx, dy, dz, dl, spring_F, FX, FY,FZ;
               int neig_i = i;
               int neig_j = j-1;
               int neig_k = k;
               dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
               dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
               dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];
               dl = (dx*dx + dy*dy + dz*dz);
               spring_F = 0.25 * spring_K*(dl-1);
               FX = spring_F * dx/dl; 
               FY = spring_F * dy/dl;
               FZ = spring_F * dz/dl; 
               F[0*N3+neig_i * N2 + neig_j*N+neig_k] += FX;
               F[1*N3+neig_i * N2 + neig_j*N+neig_k] += FY;
               F[2*N3+neig_i * N2 + neig_j*N+neig_k] += FZ;
            }
         }
      }
   }
}
extern "C" {
    void top();
}

void top() {

    double *X = (double * )0x80c00000;
    double *F = (double * )((uint64_t) X + 3 * N * N * N * sizeof(double));
    double *X_grad = (double * )((uint64_t) F + 3 * N * N * N * sizeof(double));
    double *F_grad = (double * )((uint64_t) X_grad + 3 * N * N * N * sizeof(double));

  __enzyme_autodiff<void>(compute_forces, N, X, X_grad, enzyme_const, F);
  return;
}