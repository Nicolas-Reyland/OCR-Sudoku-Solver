// model.h

#ifndef NN_MODEL_H
#define NN_MODEL_H

#include "layers/layer.h"
#include "nn/functions_descriptors/functions_descriptors_enums.h"
#include "utils/mem/mem-management.h"
#include <string.h>

typedef struct nn_Model nn_Model;
struct nn_Model {
    size_t num_layers;
    nn_Layer **layers;
    losses loss;
    optimizer optimizer;
    void (*printModelLayers)(struct nn_Model *model);
    void (*printModelLayersValues)(struct nn_Model *model);
    void (*printModelArchitecture)(struct nn_Model *model);
    double *(*use)(struct nn_Model *model, double *);
    void (*saveModel)(struct nn_Model *model, char *);
};

#include "load_model.h"
#include "save_model.h"
#include "use_model.h"

nn_Model *nn_createModel(size_t num_layers,
                         nn_ShapeDescription model_architecture[],
                         activation activations[], losses loss,
                         optimizer optimizer);
void nn_freeModel(nn_Model *model);

#endif
