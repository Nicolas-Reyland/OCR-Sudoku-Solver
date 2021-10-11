// mem-manager.h

#ifndef NN_MEM-MANAGER_H
#define NN_MEM-MANAGER_H

#include "linked_list.h"
#include <stdbool.h>

extern bool memory_is_tracked = false;
linked_list* GPL; // GPL : Global Pointer List

void initMemoryTracking(void);

void* memtrack_malloc(size_t size);

void* memtrack_calloc(size_t number, size_t size);

void mmtrack_removeAllocatedObject(void* ptr);

void _nn_trackAllocatedObject(void* ptr);

void _nn_removeAllocatedObject(void *ptr);

#endif

