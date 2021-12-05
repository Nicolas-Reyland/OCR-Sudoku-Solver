// functions_descriptor_enums.h

#ifndef NN_FUNCTIONS_DESCRIPTOR_ENUMS_H
#define NN_FUNCTIONS_DESCRIPTOR_ENUMS_H

typedef enum losses losses;
enum losses {
    NO_LOSS, // for safety
    CATEGORICALCROSSENTROPY,
    BINARYCROSSENTROPY,
    MEANSQUAREDERROR
};

extern const char *const losses_str[];

typedef enum activation activation;
enum activation {
    NO_ACTIVATION, // for safety
    SIGMOID,
    RELU,
    LEAKY_RELU,
    SOFTMAX, // output layer only
    TANH
};

extern const char *const activation_str[];

typedef enum optimizer optimizer;
enum optimizer {
    NO_OPTIMIZER, // for safety
    RMSPROP,
    ADAM
};

extern const char *const optimizer_str[];

#endif
