// node.c

#include "node.h"

nn_Node *_nn_createNode(size_t num_weights, bool init_bias) {
    double *weights, *d_weights;
    // malloc weights
    if (num_weights) {
        weights = mem_calloc(num_weights, sizeof(double));
        d_weights = mem_calloc(num_weights, sizeof(double));
        for (size_t i = 0; i < num_weights; i++) {
            weights[i] = getNormalizedRandomDouble();
            d_weights[i] = NAN;
        }
    } else {
        weights = NULL;
        d_weights = NULL;
    }
    // init bias
    double bias;
    if (init_bias) {
        bias = getNormalizedRandomDouble(); // / 10; // start with a small bias
                                            // value (-.1 < x < .1)
    } else {
        bias = NAN;
    }
    // malloc struct
    nn_Node *node = mem_malloc(sizeof(nn_Node));
    // weights
    node->num_weights = num_weights;
    node->weights = weights;
    node->d_weights = d_weights;
    // bias
    node->bias = bias;
    node->d_bias = NAN;
    // values
    node->value = NAN;
    node->d_value = NAN;
    // raw values
    node->raw_value = NAN;
    node->d_raw_value = NAN;
    return node;
}

void _nn_freeNode(nn_Node *node) {
    if (node->weights != NULL) {
        // explicitly set to NULL for the output payer
        mem_free(node->weights);
    }
    if (node->d_weights != NULL) {
        // explicitly set to NULL for the output payer
        mem_free(node->d_weights);
    }
    mem_free(node);
}

//
