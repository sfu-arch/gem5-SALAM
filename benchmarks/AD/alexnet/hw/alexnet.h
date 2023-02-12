#ifndef ALEXNET_H
#define ALEXNET_H
#include "../alexnet_clstr_hw_defines.h"

///////////// FUNCTION DEFINES ///////////
#define RELU(in) ((in) > 0 ? (in) : 0)
///////////// END DEFINES /////////////

///////////// VAR DEFINES /////////////
#define IMG_WIDTH 224
#define IMG_HEIGHT 224
#define IMG_DEPTH 3

#define LAYER_1_FILTER_COUNT  96
#define LAYER_1_FILTER_STRIDE  4

#define LAYER_1_FILTER_WIDTH  11
#define LAYER_1_FILTER_HEIGHT 11

#define LAYER_1_CONV_OUTPUT_WIDTH  (IMG_WIDTH/LAYER_1_FILTER_STRIDE) + 1 - LAYER_1_FILTER_WIDTH
#define LAYER_1_CONV_OUTPUT_HEIGHT (IMG_HEIGHT/LAYER_1_FILTER_STRIDE) + 1 - LAYER_1_FILTER_HEIGHT
#define LAYER_1_CONV_OUTPUT_DEPTH  LAYER_1_FILTER_COUNT

#define LAYER_1_CONV_OUTPUT_ELEMENT_COUNT (LAYER_1_FILTER_COUNT * LAYER_1_CONV_OUTPUT_HEIGHT * LAYER_1_CONV_OUTPUT_WIDTH * LAYER_1_CONV_OUTPUT_DEPTH)

#define LAYER_1_POOL_COUNT LAYER_1_FILTER_COUNT
#define LAYER_1_POOL_WIDTH  3
#define LAYER_1_POOL_HEIGHT 3
#define LAYER_1_POOL_DEPTH 3

#define LAYER_1_OUTPUT_WIDTH  LAYER_1_CONV_OUTPUT_WIDTH/2 + 1 - LAYER_1_POOL_WIDTH
#define LAYER_1_OUTPUT_HEIGHT LAYER_1_CONV_OUTPUT_HEIGHT/2 + 1 - LAYER_1_POOL_HEIGHT

#define LAYER_1_OUTPUT_ELEMENT_COUNT (LAYER_1_POOL_COUNT * LAYER_1_CONV_OUTPUT_HEIGHT * LAYER_1_CONV_OUTPUT_WIDTH * LAYER_1_POOL_DEPTH)
///////////// LAYER 2 ///////////// 

#define LAYER_2_INPUT_WIDTH  LAYER_1_OUTPUT_WIDTH
#define LAYER_2_INPUT_HEIGHT LAYER_1_OUTPUT_HEIGHT
#define LAYER_2_INPUT_DEPTH 3

#define LAYER_2_FILTER_COUNT  256
// #define LAYER_2_FILTER_COUNT  2

#define LAYER_2_FILTER_WIDTH  5
#define LAYER_2_FILTER_HEIGHT 5
#define LAYER_2_FILTER_DEPTH 3

#define LAYER_2_CONV_OUTPUT_WIDTH LAYER_2_INPUT_HEIGHT
#define LAYER_2_CONV_OUTPUT_HEIGHT LAYER_2_INPUT_HEIGHT
#define LAYER_2_CONV_OUTPUT_DEPTH 3

#define LAYER_2_CONV_OUTPUT_ELEMENT_COUNT (LAYER_2_FILTER_COUNT * LAYER_2_CONV_OUTPUT_HEIGHT * LAYER_2_CONV_OUTPUT_WIDTH * LAYER_2_CONV_OUTPUT_DEPTH)

#define LAYER_2_POOL_COUNT  LAYER_2_FILTER_COUNT
#define LAYER_2_POOL_WIDTH  3
#define LAYER_2_POOL_HEIGHT 3
#define LAYER_2_POOL_DEPTH 3
#define LAYER_2_FILTER_STRIDE  2

#define LAYER_2_OUTPUT_WIDTH  LAYER_2_CONV_OUTPUT_HEIGHT/2 + 1 - LAYER_2_POOL_WIDTH
#define LAYER_2_OUTPUT_HEIGHT LAYER_2_CONV_OUTPUT_HEIGHT/2 + 1 - LAYER_2_POOL_HEIGHT

#define LAYER_2_OUTPUT_ELEMENT_COUNT (LAYER_2_POOL_COUNT * LAYER_2_OUTPUT_HEIGHT * LAYER_2_OUTPUT_WIDTH * LAYER_2_POOL_DEPTH)

///////////// LAYER 3 /////////////
#define LAYER_3_INPUT_WIDTH  LAYER_2_OUTPUT_WIDTH
#define LAYER_3_INPUT_HEIGHT LAYER_2_OUTPUT_HEIGHT
#define LAYER_3_INPUT_DEPTH  LAYER_2_POOL_COUNT

#define LAYER_3_FILTER_COUNT  384
// #define LAYER_3_FILTER_COUNT  10

#define LAYER_3_FILTER_WIDTH  3
#define LAYER_3_FILTER_HEIGHT 3
#define LAYER_3_FILTER_DEPTH  3
#define LAYER_3_CONV_OUTPUT_ELEMENT_COUNT (LAYER_3_FILTER_COUNT * LAYER_3_CONV_OUTPUT_HEIGHT * LAYER_3_CONV_OUTPUT_WIDTH * LAYER_3_CONV_OUTPUT_DEPTH)

#define LAYER_3_CONV_OUTPUT_WIDTH  LAYER_3_INPUT_WIDTH 
#define LAYER_3_CONV_OUTPUT_HEIGHT LAYER_3_INPUT_HEIGHT
#define LAYER_3_CONV_OUTPUT_DEPTH  3

#define LAYER_3_OUTPUT_ELEMENT_COUNT LAYER_3_CONV_OUTPUT_WIDTH * LAYER_3_CONV_OUTPUT_HEIGHT * LAYER_3_CONV_OUTPUT_DEPTH

#define LAYER_3_POOL_COUNT  LAYER_3_FILTER_COUNT
#define LAYER_3_POOL_WIDTH  3
#define LAYER_3_POOL_HEIGHT 3
#define LAYER_3_POOL_DEPTH 3
#define LAYER_3_FILTER_STRIDE  2

#define LAYER_3_OUTPUT_WIDTH  LAYER_3_CONV_OUTPUT_HEIGHT/2 + 1 - LAYER_3_POOL_WIDTH
#define LAYER_3_OUTPUT_HEIGHT LAYER_3_CONV_OUTPUT_HEIGHT/2 + 1 - LAYER_3_POOL_HEIGHT

///////////// LAYER 4 /////////////
#define LAYER_4_INPUT_WIDTH  LAYER_3_OUTPUT_WIDTH
#define LAYER_4_INPUT_HEIGHT LAYER_3_OUTPUT_HEIGHT
#define LAYER_4_INPUT_DEPTH  3

#define LAYER_4_FILTER_COUNT  384
#define LAYER_4_CONV_OUTPUT_ELEMENT_COUNT (LAYER_4_FILTER_COUNT * LAYER_4_CONV_OUTPUT_HEIGHT * LAYER_4_CONV_OUTPUT_WIDTH * LAYER_4_CONV_OUTPUT_DEPTH)

// #define LAYER_4_FILTER_COUNT  10

#define LAYER_4_FILTER_WIDTH  3
#define LAYER_4_FILTER_HEIGHT 3

#define LAYER_4_CONV_OUTPUT_WIDTH  LAYER_4_INPUT_WIDTH 
#define LAYER_4_CONV_OUTPUT_HEIGHT LAYER_4_INPUT_HEIGHT
#define LAYER_4_CONV_OUTPUT_DEPTH  3

#define LAYER_4_OUTPUT_ELEMENT_COUNT LAYER_4_CONV_OUTPUT_WIDTH * LAYER_4_CONV_OUTPUT_HEIGHT * LAYER_4_CONV_OUTPUT_DEPTH

#define LAYER_4_POOL_COUNT  LAYER_4_FILTER_COUNT
#define LAYER_4_POOL_WIDTH  3
#define LAYER_4_POOL_HEIGHT 3
#define LAYER_4_POOL_DEPTH 3
#define LAYER_4_FILTER_STRIDE  2

#define LAYER_4_OUTPUT_WIDTH  LAYER_4_CONV_OUTPUT_HEIGHT/2 + 1 - LAYER_4_POOL_WIDTH
#define LAYER_4_OUTPUT_HEIGHT LAYER_4_CONV_OUTPUT_HEIGHT/2 + 1 - LAYER_4_POOL_HEIGHT

///////////// LAYER 5 /////////////
#define LAYER_5_INPUT_WIDTH  LAYER_4_OUTPUT_WIDTH
#define LAYER_5_INPUT_HEIGHT LAYER_4_OUTPUT_HEIGHT
#define LAYER_5_INPUT_DEPTH  3

#define LAYER_5_FILTER_COUNT  256
// #define LAYER_5_FILTER_COUNT  10
#define LAYER_5_CONV_OUTPUT_ELEMENT_COUNT (LAYER_5_FILTER_COUNT * LAYER_5_CONV_OUTPUT_HEIGHT * LAYER_5_CONV_OUTPUT_WIDTH * LAYER_5_CONV_OUTPUT_DEPTH)

#define LAYER_5_FILTER_WIDTH  3
#define LAYER_5_FILTER_HEIGHT 3

#define LAYER_5_CONV_OUTPUT_WIDTH  LAYER_5_INPUT_WIDTH 
#define LAYER_5_CONV_OUTPUT_HEIGHT LAYER_5_INPUT_HEIGHT
#define LAYER_5_CONV_OUTPUT_DEPTH  LAYER_5_FILTER_COUNT


#define LAYER_5_POOL_COUNT  LAYER_5_FILTER_COUNT
#define LAYER_5_POOL_WIDTH  3
#define LAYER_5_POOL_HEIGHT 3
#define LAYER_5_POOL_DEPTH 3
#define LAYER_5_FILTER_STRIDE  2

#define LAYER_5_OUTPUT_WIDTH  LAYER_5_CONV_OUTPUT_HEIGHT/2 + 1 - LAYER_5_POOL_WIDTH
#define LAYER_5_OUTPUT_HEIGHT LAYER_5_CONV_OUTPUT_HEIGHT/2 + 1 - LAYER_5_POOL_HEIGHT

#define LAYER_5_OUTPUT_ELEMENT_COUNT (LAYER_5_POOL_COUNT * LAYER_5_OUTPUT_HEIGHT * LAYER_5_OUTPUT_WIDTH * LAYER_5_POOL_DEPTH)

///////////// LAYER 6 /////////////
#define LAYER_6_OUTPUT_WIDTH  4096

///////////// LAYER 7 /////////////
#define LAYER_7_OUTPUT_WIDTH  4096

///////////// LAYER 8 /////////////
#define LAYER_8_OUTPUT_WIDTH  1000

///////////// END VAR DEFINES /////////////

__attribute__((always_inline))
static void conv(double* mat1, int mat_width1, double* mat2, int mat_width2, int stride, double* out) {
    int i, j, k;
    for (i = 0; i < mat_width1; i+=stride) {
        for (j = 0; j < mat_width2; j+=stride) {
            out[i * mat_width2 + j] = 0;
            #pragma clang loop unroll(full)
            for (k = 0; k < mat_width1; k++) {
                out[i * mat_width2 + j] += mat1[i * mat_width1 + k] * mat2[k * mat_width2 + j];
            }
        }
    }
}

__attribute__((always_inline))
static void pool(double* mat, int mat_width, int pool_width, double* out) {
    int i, j, k;
    for (i = 0; i < mat_width; i+=1) {
        // #pragma clang loop unroll_count(16)
        #pragma clang loop unroll(full)
        for (j = 0; j < mat_width; j+=1) {
            out[i * mat_width + j] = 0;
            for (k = 0; k < pool_width; k++) {
                int mat_row = i * pool_width + k;
                for (int l = 0; l < pool_width; l++) {
                    int mat_col = j * pool_width + l;
                    out[i * mat_width + j] += mat[mat_row * mat_width + mat_col];
                }
            }
        }
    }
}

__attribute__((always_inline))
void dense(double *mat1, int mat1_width, double *weight, int weight_count, double *out) {
    int i, j;
    for (i = 0; i < weight_count; i++) {
        double tmp = 0;
        // #pragma clang loop unroll_count(8)
        #pragma clang loop unroll(full)
        for (int j = 0; j < mat1_width; j++) {
            tmp += mat1[j] * weight[i];
        }
        out[i] = tmp;

    }
}

#endif // ALEXNET_H