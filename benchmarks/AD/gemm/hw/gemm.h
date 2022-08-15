#ifndef GEMM_H
#define GEMM_H
#include "../gemm_clstr_hw_defines.h"

///////////// FUNCTION DEFINES ///////////
#define RELU(in) ((in) > 0 ? (in) : 0)
///////////// END DEFINES /////////////

///////////// VAR DEFINES /////////////
#define MAT1_WIDTH 32
#define MAT1_HEIGHT 32

#define MAT2_WIDTH  32
#define MAT2_HEIGHT MAT1_WIDTH

#define LAYER_1_CONV_OUTPUT_WIDTH  MAT2_WIDTH
#define LAYER_1_CONV_OUTPUT_HEIGHT MAT1_HEIGHT

__attribute__((always_inline))
static void mult(double* mat1, double* mat2, double* out) {
    int i, j, k;
    for (i = 0; i < MAT1_HEIGHT; i++) {
        // #pragma clang loop unroll_count(32)
        for (j = 0; j < MAT1_WIDTH; j++) {
            out[i * MAT1_WIDTH + j] = 0;
            #pragma clang loop unroll(full)
            for (k = 0; k < MAT2_WIDTH; k++) {
                out[i * mat_width2 + j] += mat1[i * mat_width1 + k] * mat2[k * mat_width2 + j];
            }
        }
    }
}


#endif // GEMM_H