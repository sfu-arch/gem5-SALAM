#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"
#include "../gravity_clstr_hw_defines.h"


volatile uint8_t  * top   = (uint8_t  *)(TOP + 0x00);
volatile uint32_t * val_a = (uint32_t *)(TOP + 0x01);
volatile uint32_t * val_b = (uint32_t *)(TOP + 0x09);
volatile uint32_t * val_c = (uint32_t *)(TOP + 0x11);

int __attribute__ ((optimize("0"))) main(void) {
	m5_reset_stats();
    uint32_t base = 0x80c00000;
	TYPE *m1 = (TYPE *)base;
	TYPE *m2 = (TYPE *)(base+MAT_SIZE);
	TYPE *m3 = (TYPE *)(base+MAT_SIZE+VEC_SIZE);
	TYPE *check = (TYPE *)(base+MAT_SIZE+VEC_SIZE+VEC_SIZE);
    volatile int count = 0;
	stage = 0;

    *val_a = (uint32_t)(void *)m1;
    *val_b = (uint32_t)(void *)m2;
    *val_c = (uint32_t)(void *)m3;
    // printf("%d\n", *top);
    *top = 0x01;
    while (stage < 1) count++;

    printf("Job complete\n");
	m5_dump_stats();
	m5_exit();
}
