// mem-manager.h

#ifndef MEM_MEM_MANAGER_H
#define MEM_MEM_MANAGER_H

#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// global variables
linked_list* GPL; // GPL : Global Pointer List

// public functions
void initMemoryTracking(void);
void* mem_malloc(size_t size);
void* mem_calloc(size_t number, size_t size);
void mem_trackAllocatedObject(void* ptr);
void mem_free(void *ptr);

#endif
