// mem-manager.h

#ifndef NN_MEM_MANAGER_H
#define NN_MEM_MANAGER_H

#include "linked_list.h"
#include <stdbool.h>

bool memory_is_tracked = false;
linked_list* GPL; // GPL : Global Pointer List

void initMemoryTracking(void);

void trackAllocatedObject(void* ptr);

void untrackFreedObject(void *ptr);

#endif
