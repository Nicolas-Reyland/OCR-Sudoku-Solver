// model.c
#include "model.h"

static void _nn_printModelLayers(nn_Model *model);
static void _nn_printModelLayersValues(nn_Model *model);
static void _nn_printModelArchitecture(nn_Model *model);

nn_Layer **_nn_nn_createModelLayers(size_t num_layers,
                                    nn_ShapeDescription model_architecture[],
                                    activation activations[]) {
    nn_Layer **const layers = mem_calloc(num_layers, sizeof(nn_Layer));
    // input & output layers
    layers[0] =
        _nn_createInputLayer(model_architecture[0], model_architecture[1]);
    layers[num_layers - 1] = _nn_createOutputLayer(
        model_architecture[num_layers - 1], activations[num_layers - 2]);
    // hidden layers
    for (size_t i = 1; i < num_layers - 1; i++) {
        verbose("Allocating hidden layer ...");
        layers[i] =
            _nn_createLayer(model_architecture[i], model_architecture[i + 1],
                            activations[i - 1]);
    }
    verbose("layers are allocated.");

    return layers;
}

nn_Model *nn_createModel(size_t num_layers,
                         nn_ShapeDescription model_architecture[],
                         activation activations[], losses loss,
                         optimizer optimizer) {
    _nn_assertRandomInitialization();
    nn_Layer **layers =
        _nn_nn_createModelLayers(num_layers, model_architecture, activations);
    // malloc struct
    nn_Model *model = mem_malloc(sizeof(nn_Model));
    model->layers = layers;
    //*(nn_Layer **)&model->layers = layers; // need to cast away the const to
    //initialize the fields of a malloc'ed structure
    model->num_layers = num_layers;
    model->loss = loss;
    model->optimizer = optimizer;
    // add functions to struct
    model->printModelLayers = &_nn_printModelLayers;
    model->printModelLayersValues = &_nn_printModelLayersValues;
    model->printModelArchitecture = &_nn_printModelArchitecture;
    model->use = &_nn_useModel;
    model->saveModel = &_nn_Model_saveModel;
    return model;
}

void nn_freeModel(nn_Model *model) {
    for (size_t i = 0; i < model->num_layers; i++) {
        _nn_freeLayer(model->layers[i]);
    }
    mem_free(model->layers);
    mem_free(model);
}

static void _nn_printModelLayers(nn_Model *model) {
    nn_Layer *layer;
    for (size_t index = 0; index < model->num_layers; index++) {
        layer = model->layers[index];
        printf("index: %ld\n", index);
        nn_Node *node;
        for (size_t i = 0; i < layer->num_nodes; i++) {
            node = layer->nodes[i];
            for (size_t j = 0; j < node->num_weights; j++) {
                printf("layers[%ld]->nodes[%ld]->weights[%ld] = %lf\n", index,
                       i, j, node->weights[j]);
            }
            printf("bias: %lf\n", node->bias);
        }
    }
}

static void _nn_printModelLayersValues(nn_Model *model) {
    nn_Layer *layer;
    for (size_t index = 0; index < model->num_layers; index++) {
        layer = model->layers[index];
        printf("index: %ld\n", index);
        nn_Node *node;
        for (size_t i = 0; i < layer->num_nodes; i++) {
            node = layer->nodes[i];
            printf("layers[%ld]->nodes[%ld]->value = %lf\n", index, i,
                   node->value);
            printf("layers[%ld]->nodes[%ld]->raw_value = %lf\n", index, i,
                   node->raw_value);
        }
    }
}

static void _nn_printModelArchitecture(nn_Model *model) {
    printf("Model architecture:\n * num layers: %lu\n", model->num_layers);
    for (size_t i = 0; i < model->num_layers; i++) {
        nn_Layer *layer = model->layers[i];
        nn_ShapeDescription shape = layer->shape;
        printf(" - layer %lu : dims=%lu, x=%lu, y=%lu, z=%lu (%lu)\n", i + 1,
               shape.dims, shape.x, shape.y, shape.z, shape.range);
        printf("    -> activation=%s\n", activation_str[layer->activation]);
    }
    printf(" * Loss function=%s\n", losses_str[model->loss]);
}
