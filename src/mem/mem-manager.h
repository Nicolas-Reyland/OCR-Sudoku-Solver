// mem-manager.h

#ifndef NN_MEM-MANAGER_H
#define NN_MEM-MANAGER_H

#include "linked_list.h"
#include <stdbool.h>

extern bool memory_is_tracked = false;
linked_list* GPL; // GPL : Global Pointer List

void initMemoryTracking(void)
{
	memory_is_tracked = true;
	GPL = init_linked_list();
}

void trackAllocatedObject(void* ptr)
{
	GPL->append_value(GPL, ptr);
}

void removeAllocatedObject(void *ptr)
{
	// TODO: we search two times through the list. optimize this pls
	int index = GPL->index_of(GPL, ptr);
	GPL->remove_value_at(GPL, ptr);
}

#endif
