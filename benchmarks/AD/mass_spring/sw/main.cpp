#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"
#include "../mass_spring_clstr_hw_defines.h"


volatile uint8_t  * top   = (uint8_t  *)(TOP + 0x00);

int __attribute__ ((optimize("0"))) main(void) {
	m5_reset_stats();
    uint32_t base = 0x80c00000;
	
    volatile int count = 0;
	stage = 0;

    *top = 0x01;
    while (stage < 1) count++;

    printf("Job complete\n");
	m5_dump_stats();
	m5_exit();
}
