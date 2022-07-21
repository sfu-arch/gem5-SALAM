//BEGIN GENERATED CODE
//Cluster: SPMV
#include <stdint.h>

//NonCoherentDMA
#define DMA_Flags 0x10020000
#define DMA_RdAddr 0x10020001
#define DMA_WrAddr 0x10020009
#define DMA_CopyLen 0x10020011
//Accelerator: TOP
#define TOP 0x10020040
//Accelerator: SPMV
#define SPMV 0x10020080
#define R 500
#define C 500
#define N 500

typedef enum { taco_mode_dense, taco_mode_sparse } taco_mode_t;
typedef struct {
  int32_t      order;         // tensor order (number of modes)
  int32_t     *dimensions;    // tensor dimensions
  int32_t      csize;         // component size
  int32_t*     mode_ordering; // mode storage ordering
  taco_mode_t* mode_types;    // mode storage types
  int   ***indices;       // tensor index data (per mode)
  double    vals[N];          // tensor values
  int*     fill_value;    // tensor fill value
  int32_t      vals_size;     // values array size
  uint64_t      indices_size = N * C* R;
} taco_tensor_t;
//END GENERATED CODE