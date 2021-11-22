//BEGIN GENERATED CODE
//Cluster: BODY_PWCONV1
//NonCoherentDMA
#define TAIL_DMA_Flags 0x10078580
#define TAIL_DMA_RdAddr 0x10078581
#define TAIL_DMA_WrAddr 0x10078589
#define TAIL_DMA_CopyLen 0x10078591
//StreamDMA
#define TAIL_STREAM_DMA0_Flags 0x100785c0
#define TAIL_STREAM_DMA0_RdAddr 0x100785c4
#define TAIL_STREAM_DMA0_WrAddr 0x100785cc
#define TAIL_STREAM_DMA0_RdFrameSize 0x100785d4
#define TAIL_STREAM_DMA0_NumRdFrames 0x100785d8
#define TAIL_STREAM_DMA0_RdFrameBufSize 0x100785d9
#define TAIL_STREAM_DMA0_WrFrameSize 0x100785da
#define TAIL_STREAM_DMA0_NumWrFrames 0x100785de
#define TAIL_STREAM_DMA0_WrFrameBufSize 0x100785df
#define TAIL_STREAM_DMA0_Stream 0x100785e0
//Accelerator: TAIL_TOP
#define TAIL_TOP 0x10078780
//Accelerator: TAIL_PWCONV
#define TAIL_PWCONV 0x10078800
#define tail_PWConvLocalFeats 0x10078840
#define tail_PWConvWeights 0x10078940
#define tail_PWBias 0x100c3980
#define tail_PWIMultBias 0x100c3ec0
#define tail_PWNShiftBias 0x100c4400
#define tail_PWIMultOut 0x100c4940
#define tail_PWNShiftOut 0x100c4e80
#define tail_PWWeightZP 0x100c53c0
#define tail_PWConvOut 0x100c5900
//Accelerator: TAIL_RESHAPE
#define TAIL_RESHAPE 0x100c5940
#define tail_ReshapeOut 0x100c5980
//Accelerator: TAIL_AVGPOOL
#define TAIL_AVGPOOL 0x100cd540
//END GENERATED CODE