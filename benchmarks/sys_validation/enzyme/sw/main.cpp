#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../../../common/m5ops.h"
#include "../fft_clstr_hw_defines.h"
#include "bench.h"

mat_struct mat_inputs;

#define BASE            0x80c00000

#define MAT_OFFSET     0
#define VEC_OFFSET      8*M*N
#define OUT_OFFSET      VEC_OFFSET + 8*M


volatile uint8_t  * top           = (uint8_t  *)(TOP);
volatile uint32_t * loc_mat      = (uint32_t *)(TOP+1);
volatile uint32_t * loc_vec       = (uint32_t *)(TOP+9);
volatile uint32_t * loc_out = (uint32_t *)(TOP+17);

int __attribute__ ((optimize("0"))) main(void) {
        double *mat       	= (double *)(BASE+MAT_OFFSET);
        double *vec        	= (double *)(BASE+VEC_OFFSET);
        double *out  	= (double *)(BASE+OUT_OFFSET);

    volatile int count = 0;
    int stage = 0;

    mat_inputs.mat       = mat;
    mat_inputs.vec        = vec;
    mat_inputs.out  = out;

    printf("Generating data\n");
    genData(&mat_inputs);
    printf("Data generated\n");

    *loc_mat       = (uint32_t)(void *)mat;
        *loc_vec        = (uint32_t)(void *)vec;
        *loc_out  = (uint32_t)(void *)out;

    *top = 0x01;
    while (stage < 1) count++;

    printf("Job complete\n");

// #ifdef CHECK
//     bool fail = false;
// 	double creal, cimg;
//     for (int i = 0; i < FFT_SIZE; i++) {
//         creal = real[i] - real_check[i];
//         cimg = img[i] - img_check[i];
// 		// printf("[%i] = Real: %.12f, Img: %.12f \n", i, creal, cimg);

//         if ((creal > EPSILON) || (creal < -EPSILON)) {
//             fail = true;
//         }
//         if ((cimg > EPSILON) || (cimg < -EPSILON)) {
//             fail = true;
//         }
//         if (fail) {
//             printf("Diff[%i] = Real: %f, Img: %f \n", i, real[i], img[i]);
//             break;
//         }
//     }
//     if (fail)
//         printf("Check Failed\n");
//     else
//         printf("Check Passed\n");
// #endif
    m5_dump_stats();
    m5_exit();
}
