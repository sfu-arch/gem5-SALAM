#include "hw_defines.h"

void top() {

	// Define Device MMRs
	volatile uint8_t  * MATFLAG  = (uint8_t *)LOGSUM;
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);

	*MATFLAG = DEV_INIT;
	// //Poll function for finish
	while ((*MATFLAG & DEV_INTR) != DEV_INTR);

	return;
}