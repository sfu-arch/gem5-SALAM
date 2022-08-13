#include "../defines.h"
#include "data.h"

#define N 2000
#define M 2000

typedef struct
{
    double * mat;
    double * vec;
    double * out;
} mat_struct;

void genData(mat_struct *m) {
    for (int i=0; i<N*M; i++) m->mat[i] = 3*i;
    for (int i=0; i<M; i++) m->vec[i] = 1*i;
}
