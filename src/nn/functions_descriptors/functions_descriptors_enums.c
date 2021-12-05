// functions_descriptors_enums.c

#include "functions_descriptors_enums.h"

const char *const losses_str[] = {
    [NO_LOSS] = "no_loss",
    [CATEGORICALCROSSENTROPY] = "categorical_cross_entropy",
    [BINARYCROSSENTROPY] = "binary_cross_entropy",
    [MEANSQUAREDERROR] = "mean_square_error",
};

const char *const activation_str[] = {
    [NO_ACTIVATION] = "no_activation", [SIGMOID] = "sigmoid", [RELU] = "relu",
    [LEAKY_RELU] = "leaky_relu",       [SOFTMAX] = "softmax", [TANH] = "tanh",
};

const char *const optimizer_str[] = {
    [NO_OPTIMIZER] = "no_optimizer",
    [RMSPROP] = "rmsprop",
    [ADAM] = "adam",
};
