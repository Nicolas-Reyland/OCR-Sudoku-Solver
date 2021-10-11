// mem-manager.c

#include "mem-management.h"

bool memory_tracking_initialized = false;

void initMemoryTracking(void)
{
	memory_tracking_initialized = true;
	GPL = init_linked_list();
}

void trackAllocatedObject(void* ptr)
{
	GPL->append_value(GPL, ptr);
}

void untrackFreedObject(void *ptr)
{
	// TODO: we search two times through the list. optimize this pls
	int index = GPL->index_of(GPL, ptr);
	GPL->remove_value_at(GPL, index);
  // TODO: mabe a way to assert that the ptr is freed ?
  ptr = NULL;
}
