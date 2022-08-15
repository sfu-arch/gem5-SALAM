#include "../nn_clstr_hw_defines.h"
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
#define MNIST_IMAGE_WIDTH 28
#define MNIST_IMAGE_HEIGHT 28
#define MNIST_IMAGE_SIZE MNIST_IMAGE_WIDTH * MNIST_IMAGE_HEIGHT
#define MNIST_LABELS 10
#define PIXEL_SCALE(x) (((double) (x)) / 255.0f)

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

extern "C" {
    void top();
}
typedef struct mnist_image_t_ {
    double pixels[MNIST_IMAGE_SIZE];
} mnist_image_t;

typedef struct neural_network_t_ {
    double b[MNIST_LABELS];
    double W[MNIST_LABELS][MNIST_IMAGE_SIZE];
} neural_network_t;


static void neural_network_softmax_v2(const double * activations, double* outp, int length)
{
    int i;
    double sum, max;
// #pragma clang loop unroll(full)
    for (i = 1, max = activations[0]; i < length; i++) {
        if (activations[i] > max) {
            max = activations[i];
        }
    }
// #pragma clang loop unroll(full)
    for (i = 0, sum = 0; i < length; i++) {
        sum += exp(activations[i] - max);
    }

// #pragma clang loop unroll(full)
    for (i = 0; i < length; i++) {
        outp[i] = exp(activations[i] - max) / sum;
    }
#if 0
    double max = maxval(activations, length);
    double exps[length];
    makeexps(exps,activations, length, max);
    double sum = sumval(exps,length);
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

static double neural_network_hypothesis_v2(mnist_image_t * image, neural_network_t * network, uint8_t label)
{
    // double activations[MNIST_LABELS] = {0};
    double *activations = (double *) malloc(MNIST_LABELS * sizeof(double));
    // double *activations = (double *) 0x100200c0;

    int i, j;
// #pragma clang loop unroll(full)
    for (i = 0; i < MNIST_LABELS; i++) {
        activations[i] = network->b[i];
#pragma clang loop unroll_count(8)
        for (j = 0; j < MNIST_IMAGE_SIZE; j++) {
            activations[i] += network->W[i][j] * PIXEL_SCALE(image->pixels[j]);
        }
    }
 
    double *activations2 = (double *) 0x80caa120;
    // double *activations2 = (double *) malloc(MNIST_LABELS * sizeof(double));

    // double activations2[MNIST_LABELS] = { 0 };
    neural_network_softmax_v2(activations, activations2, MNIST_LABELS);
    return -log(activations2[label]);
}

void top() {
    mnist_image_t *x = (mnist_image_t *) 0x80c00000;

    neural_network_t *v =  (neural_network_t *) ((uint64_t) x + sizeof(mnist_image_t));
    mnist_image_t *v_grad = (mnist_image_t *) ((uint64_t) v + sizeof(neural_network_t));
    uint8_t label = 1;
    __enzyme_autodiff<void>(neural_network_hypothesis_v2, enzyme_const, x, v, v_grad, label);
}