#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "fft.h"

fft_struct ffts;

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#define REAL_OFFSET     0
#define IMG_OFFSET      8*FFT_SIZE
#define RTWID_OFFSET    16*FFT_SIZE
#define ITWID_OFFSET    20*FFT_SIZE
#define RCHK_OFFSET     24*FFT_SIZE
#define ICHK_OFFSET     32*FFT_SIZE

int main(void) {
	double *real       = (double *)(BASE+REAL_OFFSET);
	double *img        = (double *)(BASE+IMG_OFFSET);
	double *real_twid  = (double *)(BASE+RTWID_OFFSET);
	double *img_twid   = (double *)(BASE+ITWID_OFFSET);
	double *real_check = (double *)(BASE+RCHK_OFFSET);
	double *img_check  = (double *)(BASE+ICHK_OFFSET);

	common_val      = 0;
    ffts.real       = real;
    ffts.img        = img;
    ffts.real_twid  = real_twid;
    ffts.img_twid   = img_twid;
    ffts.real_check = real_check;
    ffts.img_check  = img_check;

    printf("Generating data\n");
    genData(&ffts);
    printf("Data generated\n");

#ifndef SPM
    loc_real       = (uint64_t)(BASE+REAL_OFFSET);
	loc_img        = (uint64_t)(BASE+IMG_OFFSET);
	loc_real_twid  = (uint64_t)(BASE+RTWID_OFFSET);
	loc_img_twid   = (uint64_t)(BASE+ITWID_OFFSET);
#else
    loc_real       = (uint64_t)(SPM_BASE+REAL_OFFSET);
	loc_img        = (uint64_t)(SPM_BASE+IMG_OFFSET);
	loc_real_twid  = (uint64_t)(SPM_BASE+RTWID_OFFSET);
	loc_img_twid   = (uint64_t)(SPM_BASE+ITWID_OFFSET);

    std::memcpy((void *)(SPM_BASE+REAL_OFFSET),  (void *)real,      sizeof(double)*FFT_SIZE);
    std::memcpy((void *)(SPM_BASE+IMG_OFFSET),   (void *)img,       sizeof(double)*FFT_SIZE);
    std::memcpy((void *)(SPM_BASE+RTWID_OFFSET), (void *)real_twid, sizeof(double)*FFT_SIZE/2);
    std::memcpy((void *)(SPM_BASE+ITWID_OFFSET), (void *)img_twid,  sizeof(double)*FFT_SIZE/2);
#endif
    int i;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    std::memcpy((void *)real, (void *)(SPM_BASE+REAL_OFFSET), sizeof(double)*FFT_SIZE);
    std::memcpy((void *)img,  (void *)(SPM_BASE+IMG_OFFSET),  sizeof(double)*FFT_SIZE);
#endif
    acc = 0x00;
	if(!checkData(&ffts)) {
	    for (i = 0; i < FFT_SIZE; i++) {
	        if(((real[i]-real_check[i]) > EPSILON) || ((real[i]-real_check[i]) < -EPSILON))
	            printf("real[%2d]=%f expected[%d]=%f\n", i, real[i], i, real_check[i]);
	    }
	    for (i = 0; i < FFT_SIZE; i++) {
	        if(((img[i]-img_check[i]) > EPSILON) || ((img[i]-img_check[i]) < -EPSILON))
	            printf("img[%2d]=%f expected[%d]=%f\n", i, img[i], i, img_check[i]);
	    }
	}
	*(char *)0x7fffffff = 1; //Kill the simulation
}