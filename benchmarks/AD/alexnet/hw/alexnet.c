#include "alexnet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

extern "C" {
    void top();
}
typedef struct mnist_image_t_ {
    double pixels[IMG_WIDTH*IMG_WIDTH*IMG_DEPTH];
} mnist_image_t;

typedef struct neural_network_weights_ {
    double layer1[LAYER_1_FILTER_COUNT*LAYER_1_FILTER_HEIGHT*LAYER_1_FILTER_WIDTH*LAYER_1_CONV_OUTPUT_DEPTH];
    double layer2[LAYER_2_FILTER_COUNT*LAYER_2_FILTER_HEIGHT*LAYER_2_FILTER_WIDTH*LAYER_2_CONV_OUTPUT_DEPTH];
    double layer3[LAYER_3_FILTER_COUNT*LAYER_3_FILTER_HEIGHT*LAYER_3_FILTER_WIDTH*LAYER_3_CONV_OUTPUT_DEPTH];
    double layer4[LAYER_4_FILTER_COUNT*LAYER_4_FILTER_HEIGHT*LAYER_4_FILTER_WIDTH*LAYER_4_CONV_OUTPUT_DEPTH];
    double layer5[LAYER_5_FILTER_COUNT*LAYER_5_FILTER_HEIGHT*LAYER_5_FILTER_WIDTH*LAYER_5_CONV_OUTPUT_DEPTH];
    double layer6[LAYER_6_OUTPUT_WIDTH];
    double layer7[LAYER_7_OUTPUT_WIDTH];
    double layer8[LAYER_8_OUTPUT_WIDTH];
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
    double *layer6_weights = (double *) (network->layer6);
    double *layer7_weights = (double *) (network->layer7);
    double *layer8_weights = (double *) (network->layer8);
    // double *conv_out = (double*) malloc(sizeof(double) * LAYER_1_CONV_OUTPUT_WIDTH * LAYER_1_CONV_OUTPUT_WIDTH);
    // double *layer1_out = (double*) malloc(sizeof(double) * LAYER_1_OUTPUT_WIDTH * LAYER_1_OUTPUT_WIDTH);
    // double *conv2_out = (double*) malloc(sizeof(double) * LAYER_2_CONV_OUTPUT_WIDTH * LAYER_2_CONV_OUTPUT_WIDTH);
    // double *layer2_out = (double*) malloc(sizeof(double) * LAYER_2_OUTPUT_WIDTH * LAYER_2_OUTPUT_WIDTH);
    
    double *conv_out = (double *) ((uint64_t) layer1_weights + sizeof(neural_network_weights_t));
    double *layer1_out = (double *) ((uint64_t) conv_out + sizeof(double) * LAYER_1_CONV_OUTPUT_ELEMENT_COUNT);
    double *conv2_out = (double*) ((uint64_t) layer1_out + sizeof(double) * LAYER_1_OUTPUT_ELEMENT_COUNT);
    double *layer2_out = (double*) ((uint64_t) conv2_out + sizeof(double) * LAYER_2_CONV_OUTPUT_ELEMENT_COUNT);
    double *conv3_out = (double*) ((uint64_t) layer2_out + sizeof(double) * LAYER_2_OUTPUT_ELEMENT_COUNT);
    double *layer3_out = (double*) ((uint64_t) layer2_out + sizeof(double) * LAYER_3_CONV_OUTPUT_ELEMENT_COUNT);
    double *conv4_out = (double*) ((uint64_t) layer3_out + sizeof(double) * LAYER_3_OUTPUT_ELEMENT_COUNT);
    double *layer4_out = (double*) ((uint64_t) conv4_out + sizeof(double) * LAYER_4_CONV_OUTPUT_ELEMENT_COUNT);
    double *conv5_out = (double*) ((uint64_t) layer4_out + sizeof(double) * LAYER_4_OUTPUT_ELEMENT_COUNT);
    double *layer5_out = (double*) ((uint64_t) conv5_out + sizeof(double) * LAYER_5_CONV_OUTPUT_ELEMENT_COUNT);
    // // double *layer4_out = (double*) ((uint64_t) layer3_out + sizeof(double) * LAYER_3_OUTPUT_ELEMENT_COUNT);
    // // double *layer5_out = (double*) ((uint64_t) layer4_out + sizeof(double) * LAYER_4_OUTPUT_ELEMENT_COUNT);
    double *layer6_out = (double*) ((uint64_t) layer5_out + sizeof(double) * LAYER_5_OUTPUT_ELEMENT_COUNT);
    double *layer7_out = (double*) ((uint64_t) layer6_out + sizeof(double) * LAYER_6_OUTPUT_WIDTH);
    double *layer8_out = (double*) ((uint64_t) layer7_out + sizeof(double) * LAYER_7_OUTPUT_WIDTH);
    /////////// layer1 //////////
    int d = 0;

    for (int i = 0; i < 1; i++) {
        // for (int d = 0; d < 3; d++) {
            double *current_filter = (double *) (layer1_weights + d * LAYER_1_FILTER_HEIGHT * LAYER_1_FILTER_WIDTH * LAYER_1_FILTER_COUNT + i * LAYER_1_FILTER_HEIGHT * LAYER_1_FILTER_WIDTH);
            double *current_filter_conv_out = (double *) (conv_out + d * LAYER_1_FILTER_HEIGHT * LAYER_1_CONV_OUTPUT_WIDTH * LAYER_1_CONV_OUTPUT_WIDTH + i * LAYER_1_CONV_OUTPUT_WIDTH * LAYER_1_CONV_OUTPUT_WIDTH);
            double *current_layer_output = (double *) (layer1_out + d * LAYER_1_FILTER_HEIGHT * LAYER_1_CONV_OUTPUT_WIDTH * LAYER_1_CONV_OUTPUT_WIDTH + i * LAYER_1_OUTPUT_WIDTH * LAYER_1_OUTPUT_WIDTH);
            conv(input + d * IMG_HEIGHT * IMG_DEPTH, IMG_WIDTH, current_filter, LAYER_1_FILTER_WIDTH, LAYER_1_FILTER_STRIDE, current_filter_conv_out);
            pool(current_filter_conv_out, LAYER_1_CONV_OUTPUT_WIDTH, LAYER_1_POOL_WIDTH, current_layer_output);
        // }
    }
    ///////// layer2 //////////
    for (int i = 0; i < 1; i++) {
        // for (int d = 0; d < 3; d++) {
            double *current_input = &layer1_out[d * LAYER_1_OUTPUT_WIDTH * LAYER_1_OUTPUT_WIDTH + i * LAYER_1_OUTPUT_WIDTH];
            double *current_filter = (double *) (layer2_weights + d * LAYER_2_FILTER_HEIGHT * LAYER_2_FILTER_WIDTH * LAYER_2_FILTER_COUNT + i * LAYER_2_FILTER_HEIGHT * LAYER_2_FILTER_WIDTH);
            double *current_filter_conv_out = (double *) (conv2_out + d * LAYER_2_FILTER_HEIGHT * LAYER_2_CONV_OUTPUT_WIDTH * LAYER_2_CONV_OUTPUT_WIDTH + i * LAYER_2_CONV_OUTPUT_WIDTH * LAYER_2_CONV_OUTPUT_WIDTH);
            double *current_layer_output = (double *) (layer2_out + d * LAYER_2_FILTER_HEIGHT * LAYER_2_CONV_OUTPUT_WIDTH * LAYER_2_CONV_OUTPUT_WIDTH + i * LAYER_2_OUTPUT_WIDTH * LAYER_2_OUTPUT_WIDTH);
            conv(current_input, LAYER_1_OUTPUT_WIDTH, current_filter, LAYER_2_FILTER_WIDTH, LAYER_2_FILTER_STRIDE, current_filter_conv_out);
            pool(current_filter_conv_out, LAYER_2_CONV_OUTPUT_WIDTH, LAYER_2_POOL_WIDTH, current_layer_output);
        // }
    }

    // // /////////// layer3 //////////
    for (int i = 0; i < 1; i++) {
        // for (int d = 0; d < 3; d++) {
            double *current_input = &layer2_out[d * LAYER_2_OUTPUT_WIDTH * LAYER_2_OUTPUT_WIDTH + i * LAYER_2_OUTPUT_WIDTH];
            double *current_filter = (double *) (layer3_weights + d * LAYER_3_FILTER_HEIGHT * LAYER_3_FILTER_WIDTH * LAYER_3_FILTER_COUNT + i * LAYER_3_FILTER_HEIGHT * LAYER_3_FILTER_WIDTH);
            double *current_filter_conv_out = (double *) (conv3_out + d * LAYER_3_FILTER_HEIGHT * LAYER_3_CONV_OUTPUT_WIDTH * LAYER_3_CONV_OUTPUT_WIDTH + i * LAYER_3_CONV_OUTPUT_WIDTH * LAYER_3_CONV_OUTPUT_WIDTH);
            double *current_layer_output = (double *) (layer3_out + d * LAYER_3_FILTER_HEIGHT * LAYER_3_CONV_OUTPUT_WIDTH * LAYER_3_CONV_OUTPUT_WIDTH + i * LAYER_3_OUTPUT_WIDTH * LAYER_3_OUTPUT_WIDTH);
            conv(current_input, LAYER_2_OUTPUT_WIDTH, current_filter, LAYER_3_FILTER_WIDTH, LAYER_2_FILTER_STRIDE, current_filter_conv_out);
            pool(current_filter_conv_out, LAYER_3_CONV_OUTPUT_WIDTH, LAYER_3_POOL_WIDTH, current_layer_output);
        // }
    }
    // // // /////////// layer4 //////////
    for (int i = 0; i < 1; i++) {
        // for (int d = 0; d < 3; d++) {
            double *current_input = &layer3_out[d * LAYER_3_OUTPUT_WIDTH * LAYER_3_OUTPUT_WIDTH + i * LAYER_3_OUTPUT_WIDTH];
            double *current_filter = (double *) (layer4_weights + d * LAYER_4_FILTER_HEIGHT * LAYER_4_FILTER_WIDTH * LAYER_4_FILTER_COUNT + i * LAYER_4_FILTER_HEIGHT * LAYER_4_FILTER_WIDTH);
            double *current_filter_conv_out = (double *) (conv4_out + d * LAYER_4_FILTER_HEIGHT * LAYER_4_CONV_OUTPUT_WIDTH * LAYER_4_CONV_OUTPUT_WIDTH + i * LAYER_4_CONV_OUTPUT_WIDTH * LAYER_4_CONV_OUTPUT_WIDTH);
            double *current_layer_output = (double *) (layer4_out + d * LAYER_4_FILTER_HEIGHT * LAYER_4_CONV_OUTPUT_WIDTH * LAYER_4_CONV_OUTPUT_WIDTH + i * LAYER_4_OUTPUT_WIDTH * LAYER_4_OUTPUT_WIDTH);
            conv(current_input, LAYER_3_OUTPUT_WIDTH, current_filter, LAYER_4_FILTER_WIDTH, LAYER_4_FILTER_STRIDE, current_filter_conv_out);
            pool(current_filter_conv_out, LAYER_4_CONV_OUTPUT_WIDTH, LAYER_4_POOL_WIDTH, current_layer_output);
        // }
    }
    // // // /////////// layer5 //////////
    for (int i = 0; i < 1; i++) {
        // for (int d = 0; d < 3; d++) {
            double *current_input = &layer4_out[d * LAYER_4_OUTPUT_WIDTH * LAYER_4_OUTPUT_WIDTH + i * LAYER_4_OUTPUT_WIDTH];
            double *current_filter = (double *) (layer5_weights + d * LAYER_5_FILTER_HEIGHT * LAYER_5_FILTER_WIDTH * LAYER_5_FILTER_COUNT + i * LAYER_5_FILTER_HEIGHT * LAYER_5_FILTER_WIDTH);
            double *current_filter_conv_out = (double *) (conv5_out + d * LAYER_5_FILTER_HEIGHT * LAYER_5_CONV_OUTPUT_WIDTH * LAYER_5_CONV_OUTPUT_WIDTH + i * LAYER_5_CONV_OUTPUT_WIDTH * LAYER_5_CONV_OUTPUT_WIDTH);
            double *current_layer_output = (double *) (layer5_out + d * LAYER_5_FILTER_HEIGHT * LAYER_5_CONV_OUTPUT_WIDTH * LAYER_5_CONV_OUTPUT_WIDTH + i * LAYER_5_OUTPUT_WIDTH * LAYER_5_OUTPUT_WIDTH);
            conv(current_input, LAYER_4_OUTPUT_WIDTH, current_filter, LAYER_4_FILTER_WIDTH, LAYER_5_FILTER_STRIDE, current_filter_conv_out);
            pool(current_filter_conv_out, LAYER_5_CONV_OUTPUT_WIDTH, LAYER_5_POOL_WIDTH, current_layer_output);
        // }
    }
    // /////////// layer6 //////////
    dense(layer5_out, LAYER_5_OUTPUT_ELEMENT_COUNT, layer6_weights, LAYER_6_OUTPUT_WIDTH, layer6_out);
    // // // // /////////// layer7 //////////
    // dense(layer6_out, LAYER_6_OUTPUT_WIDTH, layer7_weights, LAYER_7_OUTPUT_WIDTH, layer7_out);
    // // // // /////////// layer8 //////////
    // dense(layer7_out, LAYER_7_OUTPUT_WIDTH, layer8_weights, LAYER_8_OUTPUT_WIDTH, layer8_out);

}

void top() {
    mnist_image_t *x = (mnist_image_t *) 0x80c00000;

    neural_network_weights_t *v =  (neural_network_weights_t *) ((uint64_t) x + sizeof(neural_network_weights_t));
    neural_network_weights_t *v_grad = (neural_network_weights_t *) ((uint64_t) v + sizeof(neural_network_weights_t));
    uint8_t label = 1;
    int num_layers = 20;
    __enzyme_autodiff<void>(neural_network_hypothesis_v2, enzyme_const, x, v, v_grad, num_layers, label);
}