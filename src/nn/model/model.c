// model.c
#include "model.h"

void _nn_printModelLayers(nn_Model* model)
{
	nn_Layer* layer;
	for (size_t index = 0; index < model->num_layers; index++) {
		layer = model->layers[index];
		printf("index: %ld\n", index);
		nn_Node* node;
		for (size_t i = 0; i < layer->num_nodes; i++) {
			node = layer->nodes[i];
			for (size_t j = 0; j < node->num_weights; j++) {
				printf("layers[%ld]->nodes[%ld]->weights[%ld] = %lf\n", index, i, j, node->weights[j]);
			}
			printf("bias: %lf\n", node->bias);
		}
	}
}

void _nn_printModelLayersValues(nn_Model* model)
{
	nn_Layer* layer;
	for (size_t index = 0; index < model->num_layers; index++) {
		layer = model->layers[index];
		printf("index: %ld\n", index);
		nn_Node* node;
		for (size_t i = 0; i < layer->num_nodes; i++) {
			node = layer->nodes[i];
			printf("layers[%ld]->nodes[%ld]->value = %lf\n", index, i, node->value);
			printf("layers[%ld]->nodes[%ld]->raw_value = %lf\n", index, i, node->raw_value);
		}
	}
}

void _nn_Model_saveArchitectureFn(nn_Model* model, char* path)
{
  FILE* architecture_file = fopen(path, "w+");

  if (architecture_file == NULL)
  {
    fprintf(stderr, "saveArchitecture: %s, the file does not exist. Exiting...\n",path);
    exit(EXIT_FAILURE);
  }
  fprintf(architecture_file,"%lu\n\n",model->num_layers);
  for (size_t i = 0; i < model->num_layers; i++)
  {
    fprintf(architecture_file,"%lu\n",model->layers[i]->num_nodes);
  }
  fclose(architecture_file);
}

void _nn_Model_saveWeightsAndBias(nn_Model* model, char* path)
{
  FILE* wab_file = fopen(path,"w+");

  if (wab_file == NULL)
  {
    fprintf(stderr, "saveWeightsAndBias: %s, the file does not exist. Exiting...\n",path);
    exit(EXIT_FAILURE);
  }


  for(size_t i = 0; i < model->num_layers-1; i++)
  {
    
    for(size_t j = 0; j < model->layers[i]->num_nodes; j++)
    {
      for(size_t k = 0; k < model->layers[i]->nodes[j]->num_weights; k++)
      {
        fprintf(wab_file,"%f ",model->layers[i]->nodes[j]->weights[k]);
      }
      fprintf(wab_file,"\n%f\n",model->layers[i]->nodes[j]->bias);
    }
  }
  fclose(wab_file);
}

void _nn_Model_saveModel(nn_Model* model, char* dirpath)
{
  char* arch_file = mem_malloc(sizeof(double)*512);
  char* weight_file = mem_malloc(sizeof(double)*512);

  strcat(arch_file,dirpath);
  strcat(weight_file,dirpath);

  strcat(arch_file,"architecture.mdl");
  strcat(weight_file,"weightsandbias.mdl");
  
  _nn_Model_saveArchitectureFn(model, arch_file);
  _nn_Model_saveWeightsAndBias(model, weight_file);

  mem_free(arch_file);
  mem_free(weight_file);  
}

nn_Layer** _nn_createModelLayers(size_t num_layers, nn_ShapeDescription model_architecture[], activation activations[])
{
	nn_Layer** const layers = mem_calloc(num_layers, sizeof(nn_Layer));
  // input & output layers
  layers[0] = _nn_createInputLayer(model_architecture[0], model_architecture[1]);
  layers[num_layers - 1] = _nn_createOutputLayer(model_architecture[num_layers - 1], activations[num_layers - 2]);
  // hidden layers
  for (size_t i = 1; i < num_layers - 1; i++) {
    verbose("Allocating hidden layer ...");
    layers[i] = _nn_createLayer(model_architecture[i], model_architecture[i + 1], activations[i - 1]);
  }
  verbose("layers are allocated.");

  return layers;
}

nn_Model* createModel(size_t num_layers, nn_ShapeDescription model_architecture[], activation activations[], losses loss, optimizer optimizer)
{
  _nn_assertRandomInitialization();
  nn_Layer** layers = _nn_createModelLayers(num_layers, model_architecture, activations);
  // malloc struct
  nn_Model* model = mem_malloc(sizeof(nn_Model));
  model->layers = layers;
	//*(nn_Layer **)&model->layers = layers; // need to cast away the const to initialize the fields of a malloc'ed structure
	model->num_layers = num_layers;
  model->loss = loss;
  model->optimizer = optimizer;
  // add functions to struct
  model->printModelLayers = &_nn_printModelLayers;
  model->printModelLayersValues = &_nn_printModelLayersValues;
  model->saveArchitecture = &_nn_Model_saveArchitectureFn;
  model->saveWeightsAndBias = &_nn_Model_saveWeightsAndBias;
  model->saveModel = &_nn_Model_saveModel;
  return model;
}

void freeModel(nn_Model* model)
{
  for (size_t i = 0; i < model->num_layers; i++) {
    verbose("freeing layer %ld", i);
    _nn_freeLayer(model->layers[i]);
  }
  mem_free(model->layers);
  verbose("freed model layers");
  mem_free(model);
}

// nn_Model* loadModel(char* path);
