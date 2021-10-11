// mem-manager.c

#include "mem-management.h"

bool memory_tracking_initialized = false;

void initMemoryTracking(void)
{
	memory_tracking_initialized = true;
	GPL = init_linked_list();
}

void* mem_malloc(size_t size)
{
	void* ptr = malloc(size);
	_mem_trackAllocatedObject(ptr);
	return ptr;
}

void* mem_calloc(size_t number, size_t size)
{
	voir* ptr = calloc(number, size);
	_mem_trackAllocatedObject(ptr);
	return ptr;
}

void _mem_trackAllocatedObject(void* ptr)
{
	GPL->append_value(GPL, ptr);
}

void mem_free(void *ptr)
{
	// TODO: we search two times through the list. optimize this pls
	int index = GPL->index_of(GPL, ptr);
	if (index == -1) {
		printf("/!\\ Ptr %x is not in the list! Freeing anyway.\n", ptr);
		free(ptr);
		return;
	}
	GPL->remove_value_at(GPL, index);
  free(ptr);
  ptr = NULL;
}
