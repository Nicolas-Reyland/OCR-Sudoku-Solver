// mem-manager.h

#ifndef MEM_MEM_MANAGER_H
#define MEM_MEM_MANAGER_H

#include "linked_list.h"
#include "utils/verbosity/verbose.h"
#include <stdbool.h>
#include <stdlib.h>

extern linked_list *GPL;

// public functions
void initMemoryTracking(void);
void *mem_malloc(size_t size);
void *mem_calloc(size_t number, size_t size);
void _mem_trackAllocatedObject(void *ptr);
void mem_free(void *ptr);
void mem_freeGPL(bool re_init_gpl);

#endif
