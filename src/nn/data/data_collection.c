// data_collection.c

#include "data_collection.h"

nn_DataCollection _nn_loadDataCollection(nn_InOutTuple *iot_array,
                                         size_t num_tuples, double *in_ptr,
                                         double *out_ptr) {
    return (nn_DataCollection){
        num_tuples,
        iot_array,
        in_ptr,
        out_ptr,
    };
}

void _nn_freeDataCollection(nn_DataCollection collection) {
    mem_free(collection.in_ptr);
    mem_free(collection.out_ptr);
}
