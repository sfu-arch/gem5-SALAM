#include "../sddmm_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

#define DIM 16
#define N 40

extern "C" {
    void top();
}

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

typedef enum { taco_mode_dense, taco_mode_sparse } taco_mode_t;
typedef struct {
  int32_t      order;         // tensor order (number of modes)
  int32_t     *dimensions;    // tensor dimensions
  int32_t      csize;         // component size
  int32_t*     mode_ordering; // mode storage ordering
  taco_mode_t* mode_types;    // mode storage types
  uint8_t***   indices;       // tensor index data (per mode)
  double    vals[DIM*DIM*DIM];          // tensor values
  uint8_t*     fill_value;    // tensor fill value
  int32_t      vals_size;     // values array size
} taco_tensor_t;

void compute(taco_tensor_t *A, taco_tensor_t *B, taco_tensor_t *C, taco_tensor_t *D) {
  for (int32_t i = 0; i < DIM; i++) {
    for (int32_t k = 0; k < DIM; k++) {
      int32_t kC = i * DIM + k;
      #pragma clang loop unroll(full)
      for (int32_t j = 0; j < DIM; j++) {
        int32_t jA = i * DIM + j;
        int32_t jB = i * DIM + j;
        int32_t jD = k * DIM + j;
        A->vals[jA] = A->vals[jA] + (B->vals[jB] * C->vals[kC]) * D->vals[jD];
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
    taco_tensor_t* C_grad = (taco_tensor_t*)((uint64_t) D_grad + sizeof(taco_tensor_t));

    __enzyme_autodiff<double>(compute, B, B_grad, C, C_grad, D, D_grad, A, A_grad) ;
}