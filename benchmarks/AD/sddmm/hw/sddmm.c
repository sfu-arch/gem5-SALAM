#include "../sddmm_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>
// #include "../autogen.h"

extern "C" {
    void top();
}

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

#define DIMS N
void compute(taco_tensor_t *y, taco_tensor_t *a, taco_tensor_t *x, taco_tensor_t *z) {
  double* y_vals = (double*)(y->vals);
  uint64_t* a2_pos = (uint64_t*)(a->indptrs);
  double* a_vals = (double*)(a->vals);
  double* x_vals = (double*)(x->vals);
  double* z_vals = (double*)(z->vals);

  uint64_t *x_ptrs = (uint64_t*)(x->indptrs);
  uint64_t *y_ptrs = (uint64_t*)(y->indptrs);
  uint64_t *z_ptrs = (uint64_t*)(z->indptrs);
  int32_t iy = 0;
  for (int32_t i = 0; i < N; i++) {
    // #pragma omp clang loop unroll_count(8)
    for (int64_t ja = a2_pos[i]; ja < a2_pos[(i+1)]; ja++) {
      // for (int32_t k = 0; k < DIMS; k++) {
        int64_t kC = i * N + x->indices[ja];
        int64_t jD = i * N + a->indices[ja];
        y_vals[iy] += ((-1 * a_vals[ja]) * (-1*x_vals[kC])) * (-1*z_vals[jD]);
        iy++;
      // }
    }
  }
}

void top() {
    // incode
  taco_tensor_t* A = (taco_tensor_t*)((uint64_t) 0x80c00000);
  taco_tensor_t* Y = (taco_tensor_t*)((uint64_t) A + sizeof(taco_tensor_t));
  taco_tensor_t* X = (taco_tensor_t*)((uint64_t) Y + sizeof(taco_tensor_t));
  taco_tensor_t* Z = (taco_tensor_t*)((uint64_t) X + sizeof(taco_tensor_t));
  // ptr_assigner(A->ptrs);
  // ptr_assigner(Y->ptrs);
  // ptr_assigner(X->ptrs);
  // ptr_assigner(Z->ptrs);
  // outcode
  uint64_t* a2_pos = (uint64_t*)(A->indptrs);

  // for (int i = 0; i < N; i++) {
  //   for (int64_t ja = a2_pos[i]; ja < a2_pos[(i+1)]; ja++) {
  //     A->indices[ja] = (((i * ja) ^ N)  * 161241) % DATA_SIZE;
  //     Z->indices[ja] = (((i * ja * 13) ^ N)  * 1331) % DATA_SIZE;
  //     X->indices[ja] = (((i * ja * 17) ^ N)  * 1242141) % DATA_SIZE;
  //   }
  // }
  taco_tensor_t* A_grad = (taco_tensor_t*)((uint64_t) Z + sizeof(taco_tensor_t));
  taco_tensor_t* Y_grad = (taco_tensor_t*)((uint64_t) A_grad + sizeof(taco_tensor_t));
  taco_tensor_t* X_grad = (taco_tensor_t*)((uint64_t) Y_grad + sizeof(taco_tensor_t));
  taco_tensor_t* Z_grad = (taco_tensor_t*)((uint64_t) X_grad + sizeof(taco_tensor_t));

  // // taco_tensor_t* Y = (taco_tensor_t*)malloc(sizeof(taco_tensor_t));
  // taco_tensor_t* X = (taco_tensor_t*)malloc(sizeof(taco_tensor_t));
  // taco_tensor_t* Z = (taco_tensor_t*)malloc(sizeof(taco_tensor_t));

  // taco_tensor_t* A_grad = (taco_tensor_t*)malloc(sizeof(taco_tensor_t));
  // taco_tensor_t* Y_grad = (taco_tensor_t*)malloc(sizeof(taco_tensor_t));
  // taco_tensor_t* X_grad = (taco_tensor_t*)malloc(sizeof(taco_tensor_t));
  // taco_tensor_t* Z_grad = (taco_tensor_t*)malloc(sizeof(taco_tensor_t));
  
  __enzyme_autodiff<void>(compute, Y, Y_grad, A, A_grad, X, X_grad, Z, Z_grad) ;
}