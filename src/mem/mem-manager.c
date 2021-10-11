// mem-manager.c

#include "mem-manager.h"

void initMemoryTracking(void)
{
        memory_is_tracked = true;
        GPL = init_linked_list();
}

void* memtrack_malloc(size_t size)
{
        void* ptr = malloc(size);
        _nn_trackAllocatedObject(ptr);
        return ptr;
}

void* memtrack_calloc(size_t number, size_t size)
{
        void* ptr = calloc(number, size);
        _nn_trackAllocatedObject(ptr);
        return ptr;
}

void mmtrack_removeFreedObject(void* ptr)
{
	_nn_untrackFreedObject(ptr);
	ptr = NULL;
}

void _nn_trackAllocatedObject(void* ptr)
{
        GPL->append_value(GPL, ptr);
}

void _nn_untrackFreedObject(void *ptr)
{
        // TODO: we are searching two times through the linked list
	// optimize this pls
        int index = GPL->index_of(GPL, ptr);
        GPL->remove_value_at(GPL, ptr);
}


