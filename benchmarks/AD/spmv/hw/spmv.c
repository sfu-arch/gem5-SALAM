#include "../spmv_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

extern "C" {
    void top();
}

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

#define DIMS N
void compute(taco_tensor_t *y, taco_tensor_t *a, taco_tensor_t *x, taco_tensor_t *z) {
  double* y_vals = (double*)(y->vals);
  int* a2_pos = (int*)(a->indices[1][0]);
  int* a2_crd = (int*)(a->indices[1][1]);
  double* a_vals = (double*)(a->vals);
  double* x_vals = (double*)(x->vals);
  int z1_dimension = (int)(DIMS);
  double* z_vals = (double*)(z->vals);

  int32_t iy = 0;

  for (int32_t i = 0; i < DIMS; i++) {
    double tjy_val = 0.0;
    for (int32_t ja = a2_pos[i]; ja < a2_pos[(i + 1)]; ja++) {
      y_vals[iy] += (a_vals[ja] * x_vals[i]) * z_vals[i];
    }
    iy++;
  }
}

void top() {
    // incode
    taco_tensor_t* A = (taco_tensor_t*)((uint64_t) 0x80c00000);
    A->indices = (int***)malloc(sizeof(int**) * N);
    for (int i = 0; i < N; i++) {
        A->indices[i] = (int**)malloc(sizeof(int*) * C);
        for (int j = 0; j < C; j++) {
            A->indices[i][j] = (int*)malloc(sizeof(int) * R);
        }
    }
    int k = 0;
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++) 
             if ((i + j) % 2 == 0) {
                A->indices[1][0][k] = i;
                A->indices[1][1][k] = j;
                k++;
            }
   
  // taco_tensor_t* A = (taco_tensor_t*)((uint64_t) 0x80c00000 + sizeof(int) * R * C);
  taco_tensor_t* Y = (taco_tensor_t*)((uint64_t) A + sizeof(taco_tensor_t));
  taco_tensor_t* X = (taco_tensor_t*)((uint64_t) Y + sizeof(taco_tensor_t));
  taco_tensor_t* Z = (taco_tensor_t*)((uint64_t) X + sizeof(taco_tensor_t));

  taco_tensor_t* A_grad = (taco_tensor_t*)((uint64_t) Z + sizeof(taco_tensor_t));
  taco_tensor_t* Y_grad = (taco_tensor_t*)((uint64_t) A_grad + sizeof(taco_tensor_t));
  taco_tensor_t* X_grad = (taco_tensor_t*)((uint64_t) Y_grad + sizeof(taco_tensor_t));
  taco_tensor_t* Z_grad = (taco_tensor_t*)((uint64_t) X_grad + sizeof(taco_tensor_t));
  
  __enzyme_autodiff<double>(compute, Y, Y_grad, A, A_grad, X, X_grad, Z, Z_grad) ;
}