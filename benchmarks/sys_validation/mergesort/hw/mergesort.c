#include "hw_defines.h"

void merge(int start, int m, int stop){
    uint8_t * baseAddr = (uint8_t *)SPM;
    uint8_t * tempAddr = (uint8_t *)TEMP;
    TYPE    * a     = (TYPE    *)baseAddr;
    TYPE    * temp     = (TYPE    *)tempAddr;
    int i, j, k;
    merge_label1 : 
    #pragma clang loop unroll(disable)
    for(i=start; i<=m; i++){
        temp[i] = a[i];
    }
    merge_label2 : 
    #pragma clang loop unroll(disable)
    for(j=m+1; j<=stop; j++){
        temp[m+1+stop-j] = a[j];
    }

    i = start;
    j = stop;
    merge_label3 :
    #pragma clang loop unroll(disable)
    for(k=start; k<=stop; k++){
        TYPE tmp_j = temp[j];
        TYPE tmp_i = temp[i];
        if(tmp_j < tmp_i) {
            a[k] = tmp_j;
            j--;
        } else {
            a[k] = tmp_i;
            i++;
        }
    }
}

void mergesort() {
    uint8_t * baseAddr = (uint8_t *)SPM;
    TYPE    * a     = (TYPE    *)baseAddr;
    int start, stop;
    int i, m, from, mid, to;

    start = 0;
    stop = SIZE;
    mergesort_label1 : 
    #pragma clang loop unroll(disable)
    for(m=1; m<stop-start; m+=m) {
        mergesort_label2 : 
        #pragma clang loop unroll(disable)
        for(i=start; i<stop; i+=m+m) {
            from = i;
            mid = i+m-1;
            to = i+m+m-1;
            if(to < stop){
                merge(from, mid, to);
            }
            else{
                merge(from, mid, stop);
            }
        }
    }
}