// mem-manager.h

#ifndef MEM_MEM_MANAGER_H
#define MEM_MEM_MANAGER_H

#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include "utils/verbosity/nn_verbose.h"

extern linked_list* GPL;

// public functions
void nn_initMemoryTracking(void);
void* mem_malloc(size_t size);
void* mem_calloc(size_t number, size_t size);
void _mem_trackAllocatedObject(void* ptr);
void mem_free(void *ptr);

#endif
