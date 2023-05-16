#include "../mttkrp_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>
#include "../../../common/dma.h"

extern "C" {
    void top();
}

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

#ifndef N
#define N 12
#endif

#define DIMS N

typedef enum { taco_mode_dense, taco_mode_sparse } taco_mode_t;
typedef struct {
  int32_t      order;         // tensor order (number of modes)
  int32_t     *dimensions;    // tensor dimensions
  int32_t      csize;         // component size
  int32_t*     mode_ordering; // mode storage ordering
  taco_mode_t* mode_types;    // mode storage types
  uint8_t***   indices;       // tensor index data (per mode)
  double    vals[N];          // tensor values
  uint8_t*     fill_value;    // tensor fill value
  int32_t      vals_size;     // values array size
} taco_tensor_t;

void compute(taco_tensor_t *A, taco_tensor_t *B, taco_tensor_t *D, taco_tensor_t *C) {
  int A1_dimension = (int)DIMS;
  int A2_dimension = (int)DIMS;
  double* A_vals = (double*)(A->vals);
  int B1_dimension = (int)DIMS;
  int B2_dimension = (int)DIMS;
  int B3_dimension = (int)DIMS;
  double* B_vals = (double*)(B->vals);
  int D1_dimension = (int)DIMS;
  int D2_dimension = (int)DIMS;
  double* D_vals = (double*)(D->vals);
  int C1_dimension = (int)DIMS;
  int C2_dimension = (int)DIMS;
  double* C_vals = (double*)(C->vals);

  for (int32_t i = 0; i < B1_dimension; i++) {
    for (int32_t k = 0; k < C1_dimension; k++) {
      int32_t kB = i * B2_dimension + k;
      for (int32_t l = 0; l < D1_dimension; l++) {
        int32_t lB = kB * B3_dimension + l;
        #pragma clang loop unroll_count(16)
        for (int32_t j = 0; j < C2_dimension; j++) {
          int32_t jA = i * A2_dimension + j;
          int32_t jD = l * D2_dimension + j;
          int32_t jC = k * C2_dimension + j;
          A_vals[jA] = A_vals[jA] + (B_vals[lB] * D_vals[jD]) * C_vals[jC];
        }
      }
    }
  }
}

void top() {
    taco_tensor_t* A = (taco_tensor_t*)0x80c00000;
    taco_tensor_t* B = (taco_tensor_t*)((uint64_t) A + sizeof(taco_tensor_t));
    taco_tensor_t* D = (taco_tensor_t*)((uint64_t) B + sizeof(taco_tensor_t));
    taco_tensor_t* C = (taco_tensor_t*)((uint64_t) D + sizeof(taco_tensor_t));

    taco_tensor_t* A_grad = (taco_tensor_t*)((uint64_t) C + sizeof(taco_tensor_t));
    taco_tensor_t* B_grad = (taco_tensor_t*)((uint64_t) A_grad + sizeof(taco_tensor_t));
    taco_tensor_t* D_grad = (taco_tensor_t*)((uint64_t) B_grad + sizeof(taco_tensor_t));
    taco_tensor_t* C_grad = (taco_tensor_t*)((uint64_t) D_grad + sizeof(taco_tensor_t*));

    __enzyme_autodiff<double>(compute, B, B_grad, C, C_grad, D, D_grad, A, A_grad) ;
}