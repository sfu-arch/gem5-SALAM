#include "../mass_spring_clstr_hw_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

extern "C" {
    void top();
}

extern int enzyme_const;
template<typename Return, typename... T>
Return __enzyme_autodiff(T...);

#define N_SPRINGS 16
#define N_OBJECTS 2*N_SPRINGS
#ifndef N
constexpr int n_hidden = 16;
#else
constexpr int n_hidden = N;
#endif
constexpr int n_sin_waves = 16;

__attribute__((always_inline))
void nn1(double *x, double *v, double *bias1, double *weights1, double *hidden) {
    for (size_t i = 0; i < n_hidden; i++) {
        hidden[i] = 0;
        #pragma clang loop unroll_count(UNROLL_COUNT)
        for (size_t j = 0; j < n_sin_waves; ++j) {
            double weights_i_j = weights1[j];
            hidden[i] += weights_i_j * sin(1/(j+1));
        }
        #pragma clang loop unroll_count(UNROLL_COUNT)
        for (size_t j = 0; j < N_OBJECTS; ++j) {
            hidden[i] += weights1[i * n_sin_waves + j * 4 + n_sin_waves] * x[j] * 0.05;
            hidden[i] += weights1[i * n_sin_waves + j * 4 + n_sin_waves +1] * x[j+1] * 0.05;
            hidden[i] += weights1[i * n_sin_waves + j * 4 + n_sin_waves +2] * v[j*N_OBJECTS] * 0.05;
            hidden[i] += weights1[i * n_sin_waves + j * 4 + n_sin_waves +3] * v[j*N_OBJECTS+1] * 0.05;
        }
        hidden[i] += weights1[i * n_sin_waves + N_OBJECTS * 4 + n_sin_waves];
        hidden[i] += weights1[i * n_sin_waves + N_OBJECTS * 4 + n_sin_waves+1];
        hidden[i] += bias1[i];
        hidden[i] = tanh(hidden[i]);
    }
}

__attribute__((always_inline))
void nn2(double *weights2, double* bias2, double *hidden, double *act) {
    for (size_t i = 0; i < N_SPRINGS; ++i) {
        double actuation = 0;
        #pragma clang loop unroll_count(UNROLL_COUNT)
        for (size_t j = 0; j < n_hidden; ++j) {
            actuation += weights2[i * n_hidden + j] * hidden[j];
        }
        actuation += bias2[i];
        actuation = tanh(actuation);
        act[i] = actuation;
    }
}

__attribute__((always_inline))
void ApplyForce(double *x, double *act, double *spring_length, double *spring_actuation, double *v_inc) {
    // #pragma clang loop unroll_count(unroll_count/2)
    for (size_t i = 0; i < N_SPRINGS; i++) {
        int a = i;
        int b = 2*i;
        double dist = x[b] - x[a];
        double length = sqrt(x[a] * x[b] + x[a] * x[b]) + 0.0001;
        double target_length = spring_length[i] * (1 + act[i] * spring_actuation[i]);
        double impulse = (length - target_length) / length * dist;
        v_inc[a] -= impulse;
        v_inc[b] += impulse;
    }
} 

static void mass_spring(double *x, double *v, double *bias1, double *hidden, double *act, double *weights1, double *weights2, double *bias2, double *spring_lengths, double *spring_actuation, double *v_inc) {
    nn1(x, v, bias1, weights1, hidden);
    nn2(weights2, bias2, hidden, act);
    ApplyForce(x, act, spring_lengths, spring_actuation, v_inc);
}

void top() {
    double *x = (double *) malloc(sizeof(double) * N_OBJECTS);
    double *x_grad = (double *) malloc(sizeof(double) * N_OBJECTS);

    double *v = (double *) malloc(sizeof(double) * N_OBJECTS);
    double *v_grad = (double *) malloc(sizeof(double) * N_OBJECTS);

    double *bias1 = (double *) malloc(sizeof(double) * n_hidden);
    double *bias1_grad = (double *) malloc(sizeof(double) * n_hidden);

    double *weights1 = (double *) malloc(sizeof(double) * n_hidden * (n_sin_waves + N_OBJECTS * 4 + 2));
    double *weights1_grad = (double *) malloc(sizeof(double) * n_hidden * (n_sin_waves + N_OBJECTS * 4 + 2));

    double *hidden = (double *) malloc(sizeof(double) * n_hidden);
    double *hidden_grad = (double *) malloc(sizeof(double) * n_hidden);

    double *weights2 = (double *) malloc(sizeof(double) * N_SPRINGS * n_hidden);
    double *weights2_grad = (double *) malloc(sizeof(double) * N_SPRINGS * n_hidden);

    double *bias2 = (double *) malloc(sizeof(double) * N_SPRINGS);
    double *bias2_grad = (double *) malloc(sizeof(double) * N_SPRINGS);

    double *act = (double *) malloc(sizeof(double) * N_SPRINGS);
    double *act_grad = (double *) malloc(sizeof(double) * N_SPRINGS);

    double *spring_lengths = (double *) malloc(sizeof(double) * N_SPRINGS);
    double *spring_lengths_grad = (double *) malloc(sizeof(double) * N_SPRINGS);

    double *spring_actuation = (double *) malloc(sizeof(double) * N_SPRINGS);
    double *spring_actuation_grad = (double *) malloc(sizeof(double) * N_SPRINGS);

    double *v_inc = (double *) malloc(sizeof(double) * N_OBJECTS);
    double *v_inc_grad = (double *) malloc(sizeof(double) * N_OBJECTS);

    __enzyme_autodiff<double>(mass_spring, x, x_grad, v, v_grad, bias1, bias1_grad, hidden, hidden_grad, act, act_grad, weights1, weights1_grad, weights2, weights2_grad, bias2, bias2_grad, spring_lengths, spring_lengths_grad, spring_actuation, spring_actuation_grad, v_inc, v_inc_grad);
}