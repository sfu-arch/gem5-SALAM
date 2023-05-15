#include "../bilateral_clstr_hw_defines.h"
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

#ifndef N
#define N 12
#endif

#define DIMS N
#define H 30
#define W 30
#define GD 10
#define GH 10
#define GW 10
#define INPUT_CHANS 10

inline double
diff_abs(double x)
{
        double eps = 1e-8;
        return sqrt(x*x+eps);
}

inline double
weight_z(double x)
{
        double abx = diff_abs(x);
        return fmax(1.0f - abx, 0.0f);
}

void
bilateral_slice_cpu_forward_kernel(double * output,
                                   double * bilateral_grid,
                                   double * guide,
                                   double * input,
                                   bool has_offset,
                                   int total_count,
                                   int output_chans)
{
        int h = H;
        int w = W;
        int gd = GD;
        int gh = GH;
        int gw = GW;
        int input_chans = INPUT_CHANS;
        int coeff_stride = input_chans;
        int grid_chans = input_chans*output_chans;

        if (has_offset) {
                grid_chans += output_chans;
                coeff_stride += 1;
        }

        for (int idx = 0;
             idx < total_count;
             ++idx) {
                int x = idx % w;
                int y = (idx / w) % h;
                int out_c = (idx / (h*w)) % output_chans;
                int b = (idx / (output_chans*w*h));

                double gx = (x + 0.5f)*gw/(1.0f*w);
                double gy = (y + 0.5f)*gh/(1.0f*h);
                double gz = guide[x + w*(y + h*b)]*gd;

                int fx = static_cast<int>(floor(gx - 0.5f));
                int fy = static_cast<int>(floor(gy - 0.5f));
                int fz = static_cast<int>(floor(gz - 0.5f));


                // Grid strides
                int sy = gw;
                int sz = gw*gh;
                int sc = gd*gw*gh;
                int sb = grid_chans*gd*gw*gh;

                double value = 0.0f;
                #pragma clang loop unroll_count(16)

                for (int in_c = 0;
                     in_c < coeff_stride;
                     ++in_c) {
                        double coeff_sample = 0.0f;

                        #pragma clang loop unroll(full)
                        for (int xx = fx; xx < fx + 2; ++xx) {
                                int x_ = fmax(fmin(xx, gw - 1), 0);
                                double wx = fmax(1.0f - abs(xx + 0.5 - gx), 0.0f);
                                #pragma clang loop unroll(full)
                                for (int yy = fy; yy < fy + 2; ++yy) {
                                        int y_ = fmax(fmin(yy, gh - 1), 0);
                                        double wy = fmax(1.0f - abs(yy + 0.5 - gy), 0.0f);
                                        #pragma clang loop unroll(full)
                                        for (int zz = fz; zz < fz + 2; ++zz) {
                                                int z_ = fmax(fmin(zz, gd - 1), 0);
                                                double wz = sqrt(zz + 0.5 - gz);
                                                int c_ = coeff_stride*out_c + in_c;
                                                int grid_idx = x_ + sy*y_ + sz*z_ + sc*c_ + sb*b;

                                                coeff_sample += bilateral_grid[grid_idx]*wx*wy;
                                        }
                                }
                        } // Grid trilinear interpolation
                        if (in_c < input_chans) {
                                int input_idx = x + w*(y + h*(in_c + input_chans*b));
                                value += coeff_sample*input[input_idx];
                        } else { // Offset term
                                value += coeff_sample;
                        }
                }

                output[idx] = value;
        }

}

void top() {
  int size = W * H * GD * GH * GW * INPUT_CHANS;
  double* A = (double*)0x80c00000;
  double* B = (double*)((uint64_t) A + sizeof(double) * size);
  double* C = (double*)((uint64_t) B + sizeof(double) * size);
  double* out = (double*)((uint64_t) C + sizeof(double) * size);

  double* A_grad = (double*)((uint64_t) C + sizeof(double));
  double* B_grad = (double*)((uint64_t) A_grad + sizeof(double));
  double* C_grad = (double*)((uint64_t) B_grad + sizeof(double));
  double* out_grad = (double*)((uint64_t) C_grad + sizeof(double*));

    __enzyme_autodiff<double>(bilateral_slice_cpu_forward_kernel, out, out_grad, B, B_grad, C, C_grad, A, A_grad, false, W * H, 1) ;
}