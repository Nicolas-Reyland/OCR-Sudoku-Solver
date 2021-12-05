#include "load_model.h"

nn_Model *_nn_Model_loadModelArchitecture(char *path) {
    FILE *file = fopen(path, "r+");

    if (file == NULL) {
        printf("Fatal ERROR loadModelArchitecture: could not open file");
        exit(1);
    }

    size_t size = 0;
    losses loss;
    optimizer opti;
    fscanf(file, "%lu %d %d\n\n", &size, (int *)&loss, (int *)&opti);

    nn_ShapeDescription shape_descriptions[size];
    activation activations[size - 1];
    for (size_t i = 0; i < size; i++) {
        size_t x, y, z;
        int acti = 0;
        fscanf(file, "%lu %lu %lu\n", &x, &y, &z);
        shape_descriptions[i] = nn_createShapeDescription(x, y, z);

        fscanf(file, "%d\n", &acti);
        if (acti != 0) // if activation value is not "no_activation":
            activations[i - 1] = (activation)acti;
    }

    fclose(file);
    return nn_createModel(size, shape_descriptions, activations, loss, opti);
}

void _nn_Model_modifyModel(char *path, nn_Model *model) {
    FILE *file = fopen(path, "r+");

    if (file == NULL) {
        printf("Fatal ERROR modifyModel: could not open file");
        exit(1);
    }

    // weights and biases for all except last
    for (size_t i = 0; i < model->num_layers - 1; i++) {
        for (size_t j = 0; j < model->layers[i]->num_nodes; j++) {
            for (size_t k = 0; k < model->layers[i]->nodes[j]->num_weights; k++)
                fscanf(file, "%lf ", &model->layers[i]->nodes[j]->weights[k]);

            fscanf(file, "\n%lf\n", &model->layers[i]->nodes[j]->bias);
        }
    }
    // last layer biases
    nn_Layer *last_layer = model->layers[model->num_layers - 1];
    for (size_t k = 0; k < last_layer->num_nodes; k++) {
        fscanf(file, "\n%lf\n", &last_layer->nodes[k]->bias);
    }
    // close the file
    fclose(file);
}

nn_Model *nn_loadModel(char *dirpath) {
    char arch_file[512];
    char weight_file[512];

    strcpy(arch_file, dirpath);
    strcpy(weight_file, dirpath);

    strcat(arch_file, "architecture.mdl");
    strcat(weight_file, "weightsandbias.mdl");

    nn_Model *model = _nn_Model_loadModelArchitecture(arch_file);
    _nn_Model_modifyModel(weight_file, model);

    return model;
}
