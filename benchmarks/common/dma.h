#ifndef __DMA_H__
#define __DMA_H__

#define MMR_ADDR    0x2ff00000

volatile char * FLAGS = (char *)(MMR_ADDR);
volatile size_t * SRC = (size_t *)(MMR_ADDR+1);
volatile size_t * DST = (size_t *)(MMR_ADDR+9);
volatile int * LEN = (int *)(MMR_ADDR+17);
volatile unsigned flag;

#define DEVINIT	0x01
#define DEVINTR	0x04
extern "C" {
void DmaCopy(uint32_t *dst, uint32_t *src, int len) {

    volatile uint8_t  * DmaFlags   = (uint8_t  *)(0x10020000);
    volatile uint64_t * DmaRdAddr  = (uint64_t *)(0x10020001);
    volatile uint64_t * DmaWrAddr  = (uint64_t *)(0x10020009);
    volatile uint32_t * DmaCopyLen = (uint32_t *)(0x10020011);
	
    while (*DmaFlags == DEVINIT || *DmaFlags == DEVINTR);
	
    //Transfer Input Features
    *DmaRdAddr  = (uint64_t)src;
	*DmaWrAddr  = (uint64_t)dst;
	*DmaCopyLen = len;
	*DmaFlags   = DEVINIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEVINTR) != DEVINTR);
    *DmaFlags = 0x0;
}

int pollDma() {
    return ((*FLAGS&0x04)==0x04);
}
void resetDma() {
    *FLAGS = 0;
}
}

#endif //__DMA_H__
