#include "../somier_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

#define N 10
extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

void force_contribution(int n, double *X, double *F,
                   int i, int j, int k, int neig_i, int neig_j, int neig_k)
{
    double spring_K=10.0;

    double dx, dy, dz, dl, spring_F, FX, FY,FZ;
    int N2 = N*N;
    int N3 = N*N*N;
    dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
    dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
    dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];

    // dx=X[0][neig_i][neig_j][neig_k]-X[0][i][j][k];
    // dy=X[1][neig_i][neig_j][neig_k]-X[1][i][j][k];
    // dz=X[2][neig_i][neig_j][neig_k]-X[2][i][j][k];
    //    dl = sqrt(dx*dx + dy*dy + dz*dz);
    dl = (dx*dx + dy*dy + dz*dz);
    spring_F = 0.25 * spring_K*(dl-1);
    FX = spring_F * dx/dl; 
    FY = spring_F * dy/dl;
    FZ = spring_F * dz/dl; 
    F[0*N3+neig_i * N2 + neig_j*N+neig_k] += FX;
    F[1*N3+neig_i * N2 + neig_j*N+neig_k] += FY;
    F[2*N3+neig_i * N2 + neig_j*N+neig_k] += FZ;
    // F[0][i][j][k] += FX;
    // F[1][i][j][k] += FY;
    // F[2][i][j][k] += FZ;
}

void compute_forces(int n, double *X, double *F)
{
   int N2 = N*N;
   int N3 = N*N*N;
   double spring_K=10.0;

   for (int i=1; i<N-1; i++) {
      for (int j=1; j<N-1; j++) {
         for (int k=1; k<N-1; k++) {
            {
               // force_contribution (n, X, F, i, j, k, i-1, j,   k);

               double dx, dy, dz, dl, spring_F, FX, FY,FZ;
               int neig_i = i-1;
               int neig_j = j;
               int neig_k = k;
               dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
               dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
               dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];
               //    dl = sqrt(dx*dx + dy*dy + dz*dz);
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
               // force_contribution (n, X, F, i, j, k, i+1, j,   k);
               double dx, dy, dz, dl, spring_F, FX, FY,FZ;
               int neig_i = i+1;
               int neig_j = j;
               int neig_k = k;
               dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
               dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
               dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];
               //    dl = sqrt(dx*dx + dy*dy + dz*dz);
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
               // force_contribution (n, X, F, i, j, k, i,   j-1, k);
               double dx, dy, dz, dl, spring_F, FX, FY,FZ;
               int neig_i = i;
               int neig_j = j-1;
               int neig_k = k;
               dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
               dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
               dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];
               //    dl = sqrt(dx*dx + dy*dy + dz*dz);
               dl = (dx*dx + dy*dy + dz*dz);
               spring_F = 0.25 * spring_K*(dl-1);
               FX = spring_F * dx/dl; 
               FY = spring_F * dy/dl;
               FZ = spring_F * dz/dl; 
               F[0*N3+neig_i * N2 + neig_j*N+neig_k] += FX;
               F[1*N3+neig_i * N2 + neig_j*N+neig_k] += FY;
               F[2*N3+neig_i * N2 + neig_j*N+neig_k] += FZ;
            }
            // {
            //    // force_contribution (n, X, F, i, j, k, i,   j+1, k);

            //    double dx, dy, dz, dl, spring_F, FX, FY,FZ;
            //    int neig_i = i;
            //    int neig_j = j+1;
            //    int neig_k = k;
            //    dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
            //    dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
            //    dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];
            //    //    dl = sqrt(dx*dx + dy*dy + dz*dz);
            //    dl = (dx*dx + dy*dy + dz*dz);
            //    spring_F = 0.25 * spring_K*(dl-1);
            //    FX = spring_F * dx/dl; 
            //    FY = spring_F * dy/dl;
            //    FZ = spring_F * dz/dl; 
            //    F[0*N3+neig_i * N2 + neig_j*N+neig_k] += FX;
            //    F[1*N3+neig_i * N2 + neig_j*N+neig_k] += FY;
            //    F[2*N3+neig_i * N2 + neig_j*N+neig_k] += FZ;
            // }
            // {
            //    // force_contribution (n, X, F, i, j, k, i,   j,   k-1);

            //    double dx, dy, dz, dl, spring_F, FX, FY,FZ;
            //    int neig_i = i;
            //    int neig_j = j;
            //    int neig_k = k-1;
            //    dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
            //    dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
            //    dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];
            //    //    dl = sqrt(dx*dx + dy*dy + dz*dz);
            //    dl = (dx*dx + dy*dy + dz*dz);
            //    spring_F = 0.25 * spring_K*(dl-1);
            //    FX = spring_F * dx/dl; 
            //    FY = spring_F * dy/dl;
            //    FZ = spring_F * dz/dl; 
            //    F[0*N3+neig_i * N2 + neig_j*N+neig_k] += FX;
            //    F[1*N3+neig_i * N2 + neig_j*N+neig_k] += FY;
            //    F[2*N3+neig_i * N2 + neig_j*N+neig_k] += FZ;
            // }
            // {
            //    // force_contribution (n, X, F, i, j, k, i,   j,   k+1);
            //    double dx, dy, dz, dl, spring_F, FX, FY,FZ;
            //    int neig_i = i;
            //    int neig_j = j;
            //    int neig_k = k+1;
            //    dx = X[0*N3+neig_i * N2 + neig_j*N+neig_k] - X[0*N3+i * N2 + j*N+k];
            //    dy = X[1*N3+neig_i * N2 + neig_j*N+neig_k] - X[1*N3+i * N2 + j*N+k];
            //    dz = X[2*N3+neig_i * N2 + neig_j*N+neig_k] - X[2*N3+i * N2 + j*N+k];
            //    //    dl = sqrt(dx*dx + dy*dy + dz*dz);
            //    dl = (dx*dx + dy*dy + dz*dz);
            //    spring_F = 0.25 * spring_K*(dl-1);
            //    FX = spring_F * dx/dl; 
            //    FY = spring_F * dy/dl;
            //    FZ = spring_F * dz/dl; 
            //    F[0*N3+neig_i * N2 + neig_j*N+neig_k] += FX;
            //    F[1*N3+neig_i * N2 + neig_j*N+neig_k] += FY;
            //    F[2*N3+neig_i * N2 + neig_j*N+neig_k] += FZ;
            // }
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
//    double ****F;
//    double ****X_grad;
// //    double ****F_grad;
//    X = (double ****)malloc(3*(N-1)*(N-1)*(N-1)*sizeof(double));
//    F = (double ****)malloc(3*(N-1)*(N-1)*(N-1)*sizeof(double));
//    X_grad = (double ****)malloc(3*(N-1)*(N-1)*(N-1)*sizeof(double));
//    F_grad = (double ****)malloc(3*(N-1)*(N-1)*(N-1)*sizeof(double));
   
  __enzyme_autodiff<void>(compute_forces, N, X, X_grad, enzyme_const, F);
  return;
}