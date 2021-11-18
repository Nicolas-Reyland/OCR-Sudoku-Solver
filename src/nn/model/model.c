// model.c
#include "model.h"


static void _nn_printModelLayers(nn_Model* model);
static void _nn_printModelLayersValues(nn_Model* model);
static void _nn_printModelArchitecture(nn_Model* model);

void _nn_Model_saveArchitectureFn(nn_Model* model, char* path)
{
  FILE* architecture_file = fopen(path, "w+");

  if (architecture_file == NULL)
  {
    fprintf(stderr, "saveArchitecture: %s, the file does not exist. Exiting...\n",path);
    exit(EXIT_FAILURE);
  }
  fprintf(architecture_file,"%lu %d %d\n\n",model->num_layers, model->loss, model->optimizer);
  for (size_t i = 0; i < model->num_layers; i++)
  {
    size_t x = model->layers[i]->shape.x,
           y = model->layers[i]->shape.y,
           z = model->layers[i]->shape.z;

    fprintf(architecture_file,"%lu %lu %lu\n", x, y, z);
    fprintf(architecture_file,"%d\n",model->layers[i]->activation);
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

  // weights and biases for all except last
  for(size_t i = 0; i < model->num_layers-1; i++)
  {
    for(size_t j = 0; j < model->layers[i]->num_nodes; j++)
    {
      for(size_t k = 0; k < model->layers[i]->nodes[j]->num_weights; k++)
      {
        fprintf(wab_file,"%lf ",model->layers[i]->nodes[j]->weights[k]);
      }
      fprintf(wab_file,"\n%lf\n",model->layers[i]->nodes[j]->bias);
    }
  }
  // last layer biases
  nn_Layer* last_layer = model->layers[model->num_layers - 1];
  for (size_t k = 0; k < last_layer->num_nodes; k++) {
	  fprintf(wab_file,"\n%lf\n", last_layer->nodes[k]->bias);
  }
  // close the file
  fclose(wab_file);
}

void _nn_Model_saveModel(nn_Model* model, char* dirpath)
{
  char arch_file[512];
  char weight_file[512];

  strcpy(arch_file, dirpath);
  strcpy(weight_file, dirpath);

  strcat(arch_file, "architecture.mdl");
  strcat(weight_file, "weightsandbias.mdl");

  _nn_Model_saveArchitectureFn(model, arch_file);
  _nn_Model_saveWeightsAndBias(model, weight_file);
}

nn_Layer** _nn_createModelLayers(size_t num_layers, nn_ShapeDescription model_architecture[], activation activations[])
{
  for(size_t i = 0; i < num_layers - 1; i++)
    verbose("activations[%d] = %d", i, activations[i]);
  verbose("Value of activations[num_layers - 2] = activations[%d] = %d", num_layers - 2, activations[num_layers - 2]);
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
  model->printModelArchitecture = &_nn_printModelArchitecture;
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

nn_Model* _nn_Model_loadModelArchitecture(char* path)
{
  FILE* file = fopen(path,"r+");

  if (file == NULL)
  {
    printf("Fatal ERROR loadModelArchitecture: could not open file");
    exit(1);
  }

  size_t size = 0;
  losses loss;
  optimizer opti;
  fscanf(file,"%lu %d %d\n\n", &size, (int*)&loss, (int*)&opti);


  nn_ShapeDescription shape_descriptions [size];
  activation activations [size-1];
  for(size_t i = 0; i < size; i++)
  {
    size_t x,y,z;
    int acti = 0;
    fscanf(file,"%lu %lu %lu\n", &x, &y, &z);
    shape_descriptions[i] = createShapeDescription(x,y,z);

    fscanf(file,"%d\n", &acti);
    if(acti != 0) //if activation value is not "no_activation":
        activations[i-1] = (activation)acti;
  }

  fclose(file);
  for(size_t i = 0; i < size - 1; i++)
    verbose("In Architecture Loading: activations[%d] = %d", i, activations[i]);
  return createModel(size, shape_descriptions, activations, loss, opti);

}

void _nn_Model_modifyModel(char* path, nn_Model* model)
{
  FILE* file = fopen(path,"r+");

  if (file == NULL)
  {
    printf("Fatal ERROR modifyModel: could not open file");
    exit(1);
  }

  // weights and biases for all except last
  for (size_t i = 0; i < model->num_layers - 1; i++)
  {
    for(size_t j = 0; j < model->layers[i]->num_nodes;j++)
    {
      for(size_t k = 0; k < model->layers[i]->nodes[j]->num_weights; k++)
        fscanf(file,"%lf ", &model->layers[i]->nodes[j]->weights[k]);

      fscanf(file,"\n%lf\n", &model->layers[i]->nodes[j]->bias);
    }
  }
  // last layer biases
  nn_Layer* last_layer = model->layers[model->num_layers - 1];
  for (size_t k = 0; k < last_layer->num_nodes; k++) {
	  fscanf(file,"\n%lf\n", &last_layer->nodes[k]->bias);
  }
  // close the file
  fclose(file);
}

nn_Model* nn_loadModel(char* dirpath)
{
	char arch_file[512];
	char weight_file[512];

	strcpy(arch_file, dirpath);
	strcpy(weight_file, dirpath);

	strcat(arch_file, "architecture.mdl");
	strcat(weight_file, "weightsandbias.mdl");

	nn_Model* model = _nn_Model_loadModelArchitecture(arch_file);
	_nn_Model_modifyModel(weight_file, model);

	return model;
}

static void _nn_printModelLayers(nn_Model* model)
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

static void _nn_printModelLayersValues(nn_Model* model)
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

static void _nn_printModelArchitecture(nn_Model* model)
{
	printf("Model architecture:\n * num layers: %lu\n", model->num_layers);
	for (size_t i = 0; i < model->num_layers; i++) {
		nn_Layer *layer = model->layers[i];
		nn_ShapeDescription shape = layer->shape;
		printf(" - layer %lu : dims=%lu, x=%lu, y=%lu, z=%lu (%lu)\n", i, shape.dims, shape.x, shape.y, shape.z, shape.range);
		printf("    -> activation=%s\n", activation_str[layer->activation]);
	}
	printf(" * Loss function=%s\n", losses_str[model->loss]);
}


