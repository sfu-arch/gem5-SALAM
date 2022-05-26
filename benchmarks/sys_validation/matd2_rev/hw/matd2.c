#include "../matd2_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

#define MNIST_LABEL_MAGIC 0x00000801
#define MNIST_IMAGE_MAGIC 0x00000803
#define MNIST_IMAGE_WIDTH 3
#define MNIST_IMAGE_HEIGHT 3
#define MNIST_IMAGE_SIZE MNIST_IMAGE_WIDTH * MNIST_IMAGE_HEIGHT
#define MNIST_LABELS 3
#define PIXEL_SCALE(x) (((float) (x)) / 255.0f)

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

extern "C" {
    void top();
}
typedef struct mnist_image_t_ {
    uint8_t pixels[MNIST_IMAGE_SIZE];
} __attribute__((packed)) mnist_image_t;

typedef struct neural_network_t_ {
    float b[MNIST_LABELS];
    float W[MNIST_LABELS][MNIST_IMAGE_SIZE];
} neural_network_t;


static void neural_network_softmax_v2(const float * activations, float* outp, int length)
{
    int i;
    float sum, max;
#pragma clang loop unroll(full)
    for (i = 1, max = activations[0]; i < length; i++) {
        if (activations[i] > max) {
            max = activations[i];
        }
    }
#pragma clang loop unroll(full)
    for (i = 0, sum = 0; i < length; i++) {
        sum += exp(activations[i] - max);
    }

#pragma clang loop unroll(full)
    for (i = 0; i < length; i++) {
        outp[i] = exp(activations[i] - max) / sum;
    }
#if 0
    float max = maxval(activations, length);
    float exps[length];
    makeexps(exps,activations, length, max);
    float sum = sumval(exps,length);
    /*
    for (int i = 0; i < length; i++) {
        double tmp = exps[i];//exp(activations[i] - max);
        sum += tmp;
    }
    */

#pragma clang loop unroll(full)
    for (int i = 0; i < length; i++) {
        outp[i] = exps[i] / sum;
    }
#endif
}

static float neural_network_hypothesis_v2(const mnist_image_t * image, const neural_network_t * network, uint8_t label)
{
    // float activations[MNIST_LABELS] = {0};
    float *activations = (float *) 0x100200c0;

    int i, j;
#pragma clang loop unroll(full)
    for (i = 0; i < MNIST_LABELS; i++) {
        activations[i] = network->b[i];
#pragma clang loop unroll(full)
        for (j = 0; j < MNIST_IMAGE_SIZE; j++) {
            activations[i] += network->W[i][j] * PIXEL_SCALE(image->pixels[j]);
        }
    }
    float *activations2 = (float *) 0x100200c0;
    // float activations2[MNIST_LABELS] = { 0 };
    neural_network_softmax_v2(activations, activations2, MNIST_LABELS);
    return -log(activations2[label]);
}

#define N 10

double dt = 0.001;

double ComputeU(double *x) {
    double r;
    double u = 1.0;
// #pragma clang loop unroll(full)
    for (int i = 0; i < N; i++) {
// #pragma clang loop unroll(full)
        for (int j=i+1; j < N; j++) {
            r = x[i] - x[j];
            u *= -1.0 / (r + 0.001);
        }
    }
    return u;
}

void top() {

    // mnist_image_t *x = (mnist_image_t *) 0x100200c0;
    // neural_network_t *v =  (neural_network_t *) 0x10033980;
    // mnist_image_t *x_grad = (mnist_image_t *) 0x10033cc0;
    // uint8_t label = 3;
    // __enzyme_autodiff<void>(neural_network_hypothesis_v2, enzyme_const, x, v, x_grad, label);

    volatile double * x = (double *)MAT;
    // double *v = new double[N]
    double *x_grad = (double *)(VEC);

    // merge(tempBase, arrBase, tempBase);
    // arrBase[0] = compute_U(1);
    __enzyme_autodiff<double>(ComputeU, x, x_grad) ;
}