#include "save_model.h"

void _nn_Model_saveArchitectureFn(nn_Model *model, char *path) {
    FILE *architecture_file = fopen(path, "w+");

    if (architecture_file == NULL) {
        err_verbose_exit(
            "saveArchitecture: %s, the file does not exist. Exiting...\n",
            path);
    }
    fprintf(architecture_file, "%lu %d %d\n\n", model->num_layers, model->loss,
            model->optimizer);
    for (size_t i = 0; i < model->num_layers; i++) {
        size_t x = model->layers[i]->shape.x, y = model->layers[i]->shape.y,
               z = model->layers[i]->shape.z;

        fprintf(architecture_file, "%lu %lu %lu\n", x, y, z);
        fprintf(architecture_file, "%d\n", model->layers[i]->activation);
    }
    fclose(architecture_file);
}

void _nn_Model_saveWeightsAndBias(nn_Model *model, char *path) {
    FILE *wab_file = fopen(path, "w+");

    if (wab_file == NULL) {
        err_verbose_exit(
            "saveWeightsAndBias: %s, the file does not exist. Exiting...\n",
            path);
    }

    // weights and biases for all except last
    for (size_t i = 0; i < model->num_layers - 1; i++) {
        for (size_t j = 0; j < model->layers[i]->num_nodes; j++) {
            for (size_t k = 0; k < model->layers[i]->nodes[j]->num_weights; k++)
                fprintf(wab_file, "%lf ",
                        model->layers[i]->nodes[j]->weights[k]);

            fprintf(wab_file, "\n%lf\n", model->layers[i]->nodes[j]->bias);
        }
    }
    // last layer biases
    nn_Layer *last_layer = model->layers[model->num_layers - 1];
    for (size_t k = 0; k < last_layer->num_nodes; k++) {
        fprintf(wab_file, "\n%lf\n", last_layer->nodes[k]->bias);
    }
    // close the file
    fclose(wab_file);
}

void _nn_Model_saveModel(nn_Model *model, char *dirpath) {
    char arch_file[512];
    char weight_file[512];

    strcpy(arch_file, dirpath);
    strcpy(weight_file, dirpath);

    strcat(arch_file, "architecture.mdl");
    strcat(weight_file, "weightsandbias.mdl");

    _nn_Model_saveArchitectureFn(model, arch_file);
    _nn_Model_saveWeightsAndBias(model, weight_file);
}