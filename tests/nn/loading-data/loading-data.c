// loading-data.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"
#include <string.h>

extern linked_list *GPL;
extern bool _nn_random_init_done;

int main(int argc, char **argv) {
    setVerbose(true);
    initMemoryTracking();

    // get path to project as arg
    char input_path[255], output_path[255];
    char *path_to_project = argv[1];
    strcpy(input_path, path_to_project);
    strcpy(output_path, path_to_project);
    strcat(input_path, "/datas/xor/input.txt");
    strcat(output_path, "/datas/xor/output.txt");

    nn_Data data =
        nn_loadSingleDataInputOutput(input_path, output_path, false, NULL);
    printf("created data.\n");
    data.printData(data);

    // free data
    _nn_freeData(data);

    // free model
    // nn_freeModel(model);
    mem_freeGPL(false);

    return 0;
}
