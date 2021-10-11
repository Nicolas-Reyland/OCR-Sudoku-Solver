// mem-manager.h

#ifndef MEM_MEM_MANAGER_H
#define MEM_MEM_MANAGER_H

#include "linked_list.h"
#include <stdbool.h>

// global variables
linked_list* GPL; // GPL : Global Pointer List

// public functions
void initMemoryTracking(void);
void trackAllocatedObject(void* ptr);
void untrackFreedObject(void *ptr);

#endif
