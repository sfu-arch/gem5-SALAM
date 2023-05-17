#include "hw_defines.h"

extern "C" {
	void top();
}

void top() {

	// Define Device MMRs
	volatile uint8_t  * MATFLAG  = (uint8_t *)SDDMM;

	// //Start the accelerated function
	*MATFLAG = DEV_INIT;
	// //Poll function for finish
	while ((*MATFLAG & DEV_INTR) != DEV_INTR);

	return;
}