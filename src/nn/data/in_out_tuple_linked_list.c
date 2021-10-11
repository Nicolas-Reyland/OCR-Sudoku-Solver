/* iot_linked_list Implementation */
#include "in_out_tuple_linked_list.h"
#include <stdio.h>
#include <errno.h>

// init function
iot_linked_list* init_iot_linked_list()
{
	iot_linked_list* list = (iot_linked_list*) malloc(sizeof(iot_linked_list));
	// setup attributes
	list->length = 0;
	list->value_size = sizeof(nn_InOutTuple*);
	list->head = NULL;
	// attach methods
	list->get_value_at = &iot_linked_list_get_value_at;
	list->insert_value_at = &iot_linked_list_insert_value_at;
	list->append_value = &iot_linked_list_append_value;
	list->remove_value_at = &iot_linked_list_remove_value_at;
	list->index_of = &iot_linked_list_index_of;

	return list;
}

/* Free function */
void free_iot_linked_list(iot_linked_list* list)
{
	_free_iot_linked_list_node(list->head);
	free(list);
}

// free node
void _free_iot_linked_list_node(iot_ll_node* node)
{
	if (node->next != NULL) _free_iot_linked_list_node(node->next);
	free(node);
}


// error message
void _iot_linked_list_exit_msg(const char* msg)
{
	fprintf(stderr, "%s", msg);
	fprintf(stderr, "%s", "\n");
	exit(EXIT_FAILURE);
}

// is an index valid ?
bool _iot_linked_list_valid_index(iot_linked_list *list, int index)
{
	return 0 <= index && index < list->length;
}

// assert that an index is valid
void _iot_linked_list_assert_index(iot_linked_list *list, int index)
{
	if (_iot_linked_list_valid_index(list, index)) return;
	char msg[255];
	sprintf(msg, IOT_LL_INDEX_ERROR_MSG, index, list->length);
	_iot_linked_list_exit_msg(msg);
}

// get the value at the index
iot_ll_node* _iot_linked_list_get_node_at(iot_linked_list *list, int index)
{
	iot_ll_node* node = list->head;

	for (int i = 0; i < index; i++)
	{
		node = node->next;
		// verify after we get the next node: the head should have been verified in the _assert_index call
		if (node == NULL)
		{
			char msg[255];
			sprintf(msg, IOT_LL_NULL_POINTER_NODE, i);
			_iot_linked_list_exit_msg(msg);
		}
	}

	return node;
}

// create a new node
iot_ll_node* _iot_linked_list_new_node()
{
	// allocate new memory for node
	size_t iot_ll_node_size = sizeof(struct iot_ll_node);
	struct iot_ll_node *node = (struct iot_ll_node*) malloc(iot_ll_node_size);
	// if the pointer is NULL, the allocation faled
	if (node == NULL)
	{
		char msg[255];
		sprintf(msg, IOT_LL_MEM_ALLOC_ERROR_MSG, iot_ll_node_size);
		_iot_linked_list_exit_msg(msg);
	}
	return node;
}


/* 'Public' Methods */

// get value at index
nn_InOutTuple* iot_linked_list_get_value_at(iot_linked_list *list, int index)
{
	_iot_linked_list_assert_index(list, index);
	return _iot_linked_list_get_node_at(list, index)->value;
}

// insert value at index
void iot_linked_list_insert_value_at(iot_linked_list *list, int index, nn_InOutTuple* value)
{
	// init list ?
	if (index == 0)
	{
		// insert at head
		iot_ll_node* new_head = _iot_linked_list_new_node();
		new_head->value = value;
		new_head->next = list->head;
		list->head = new_head;
	}
	// append after head
	else
	{
		int new_index = index - 1;
		_iot_linked_list_assert_index(list, new_index);
		// get nodes
		iot_ll_node* prev_node = _iot_linked_list_get_node_at(list, new_index);
		iot_ll_node* after_node = prev_node->next;
		iot_ll_node* new_node = _iot_linked_list_new_node();
		new_node->value = value;
		// set connections
		prev_node->next = new_node;
		new_node->next = after_node;
	}

	// increment list length
	list->length++;
}

// append value
void iot_linked_list_append_value(iot_linked_list *list, nn_InOutTuple* value)
{
	// add to empty list
	if (list->length == 0)
	{
		list->head = _iot_linked_list_new_node();
		list->head->value = value;
	}
	// add to tail of the list
	else
	{
		iot_ll_node *tail = _iot_linked_list_get_node_at(list, list->length - 1);
		tail->next = _iot_linked_list_new_node();
		tail->next->value = value;
	}

	// increment list length
	list->length++;
}

// remove value at index
nn_InOutTuple* iot_linked_list_remove_value_at(iot_linked_list *list, int index)
{
	_iot_linked_list_assert_index(list, index);

	// remove the head
	if (index == 0)
	{
		// there is only the head
		if (list->length == 1)
		{
			free(list->head);
			list->head = NULL;
		}
		// there are more
		else
		{
			iot_ll_node* old_head = list->head;
			iot_ll_node* new_head = old_head->next;
			list->head = new_head;
			free(old_head);
			old_head = NULL;
		}
	}
	// remove somewhere else
	else
	{
		iot_ll_node* previous_node = _iot_linked_list_get_node_at(list, index - 1);
		iot_ll_node* current_node = previous_node->next;
		iot_ll_node* next_node = current_node->next;
		previous_node->next = next_node;
		free(current_node);
		current_node = NULL;
	}

	// decrement the length
	list->length--;
}

// index of value
int iot_linked_list_index_of(iot_linked_list *list, nn_InOutTuple* value)
{
	iot_ll_node* node = list->head;
	int index = 0;
	do
	{
		if (node->value == value) {
			return index;
		}
		node = node->next;
		index++;
	} while (node != NULL);

	return -1;
}
