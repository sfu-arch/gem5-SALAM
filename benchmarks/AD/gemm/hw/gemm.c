#include "gemm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

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
    double pixels[IMG_WIDTH*IMG_WIDTH];
} mnist_image_t;

typedef struct neural_network_t_ {
    double W[LAYER_1_FILTER_HEIGHT*LAYER_1_FILTER_WIDTH];
} neural_network_t;

typedef struct neural_network_weights_ {
    double layer1[LAYER_1_FILTER_COUNT*LAYER_1_FILTER_HEIGHT*LAYER_1_FILTER_WIDTH];
    double layer2[LAYER_2_FILTER_COUNT*LAYER_2_FILTER_HEIGHT*LAYER_2_FILTER_WIDTH];
    double layer3[LAYER_3_FILTER_COUNT*LAYER_3_FILTER_HEIGHT*LAYER_3_FILTER_WIDTH];
    double layer4[LAYER_4_OUTPUT_WIDTH];
    double layer5[LAYER_5_OUTPUT_WIDTH];
} neural_network_weights_t;


static void neural_network_hypothesis_v2(mnist_image_t *image, neural_network_weights_t * network, int num_layers, uint8_t label)
{
    
    double *input = (double *)image;
    ////////// layer1 //////////
    int FILTER_ELEM_SIZE = LAYER_1_FILTER_HEIGHT * LAYER_1_FILTER_WIDTH;
    double *layer1_weights = (double *) (network->layer1);
    double *layer2_weights = (double *) (network->layer2);
    double *layer3_weights = (double *) (network->layer3);
    double *layer4_weights = (double *) (network->layer4);
    double *layer5_weights = (double *) (network->layer5);
    // double *conv_out = (double*) malloc(sizeof(double) * LAYER_1_CONV_OUTPUT_WIDTH * LAYER_1_CONV_OUTPUT_WIDTH);
    // double *layer1_out = (double*) malloc(sizeof(double) * LAYER_1_OUTPUT_WIDTH * LAYER_1_OUTPUT_WIDTH);
    // double *conv2_out = (double*) malloc(sizeof(double) * LAYER_2_CONV_OUTPUT_WIDTH * LAYER_2_CONV_OUTPUT_WIDTH);
    // double *layer2_out = (double*) malloc(sizeof(double) * LAYER_2_OUTPUT_WIDTH * LAYER_2_OUTPUT_WIDTH);
    
    double *conv_out = (double *) ((uint64_t) layer1_weights + sizeof(neural_network_weights_t));
    double *layer1_out = (double *) ((uint64_t) conv_out + sizeof(double) * LAYER_1_CONV_OUTPUT_ELEMENT_COUNT);
    double *conv2_out = (double*) ((uint64_t) layer1_out + sizeof(double) * LAYER_1_OUTPUT_ELEMENT_COUNT);
    double *layer2_out = (double*) ((uint64_t) conv2_out + sizeof(double) * LAYER_2_CONV_OUTPUT_ELEMENT_COUNT);
    double *layer3_out = (double*) ((uint64_t) layer2_out + sizeof(double) * LAYER_2_OUTPUT_ELEMENT_COUNT);
    double *layer4_out = (double*) ((uint64_t) layer3_out + sizeof(double) * LAYER_3_OUTPUT_ELEMENT_COUNT);
    double *layer5_out = (double*) ((uint64_t) layer4_out + sizeof(double) * LAYER_4_OUTPUT_WIDTH);

    /////////// layer1 //////////
    for (int i = 0; i < 2; i++) {
        double *current_filter = (double *) (layer1_weights + i * LAYER_1_FILTER_HEIGHT * LAYER_1_FILTER_WIDTH);
        double *current_filter_conv_out = (double *) (conv_out + i * LAYER_1_CONV_OUTPUT_WIDTH * LAYER_1_CONV_OUTPUT_WIDTH);
        double *current_layer_output = (double *) (current_layer_output + i * LAYER_1_OUTPUT_WIDTH * LAYER_1_OUTPUT_WIDTH);
        conv(input, IMG_WIDTH, current_filter, LAYER_1_FILTER_WIDTH, current_filter_conv_out);
        pool(current_filter_conv_out, LAYER_1_CONV_OUTPUT_WIDTH, LAYER_1_POOL_WIDTH, current_layer_output);
    }
    /////////// layer2 //////////
    // conv(layer1_out, LAYER_2_INPUT_WIDTH, layer2_weights, LAYER_2_FILTER_WIDTH, conv2_out);
    // pool(conv2_out, LAYER_2_CONV_OUTPUT_WIDTH, LAYER_2_POOL_WIDTH, layer2_out);

    // // /////////// layer3 //////////
    // conv(layer2_out, LAYER_3_INPUT_WIDTH, layer3_weights, LAYER_3_FILTER_WIDTH, layer3_out);
    // // /////////// layer4 //////////
    // dense(layer3_out, LAYER_3_OUTPUT_ELEMENT_COUNT, layer4_weights, LAYER_4_OUTPUT_WIDTH, layer4_out);
    // // /////////// layer5 //////////
    // dense(layer4_out, LAYER_4_OUTPUT_WIDTH, layer5_weights, LAYER_5_OUTPUT_WIDTH, layer5_out);
}

void top() {
    mnist_image_t *x = (mnist_image_t *) 0x80c00000;

    neural_network_weights_t *v =  (neural_network_weights_t *) ((uint64_t) x + sizeof(neural_network_weights_t));
    neural_network_weights_t *v_grad = (neural_network_weights_t *) ((uint64_t) v + sizeof(neural_network_weights_t));
    uint8_t label = 1;
    int num_layers = 20;
    __enzyme_autodiff<void>(neural_network_hypothesis_v2, enzyme_const, x, v, v_grad, num_layers, label);
}