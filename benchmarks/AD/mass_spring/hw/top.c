#include "hw_defines.h"

void top() {

	// Define Device MMRs
	volatile uint8_t  * MATFLAG  = (uint8_t *)MASS_SPRING;

	// //Start the accelerated function
	*MATFLAG = DEV_INIT;
	// //Poll function for finish
	while ((*MATFLAG & DEV_INTR) != DEV_INTR);
	return;
}