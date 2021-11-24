// in_out_tuple_linked_list.h

#ifndef NN_LINKED_LIST_H
#define NN_LINKED_LIST_H

/* linked_list Declaration */
#include <stdlib.h>
#include <stdbool.h>
#include "utils/verbosity/verbose.h"
#include "in_out_tuple.h"

/* Constants */
#define IOT_LL_INDEX_ERROR_MSG "iot_linked_list:IndexError: %d out of bounds with linked list of length %d"
#define IOT_LL_MEM_ALLOC_ERROR_MSG "iot_linked_list:AllocationError: Could not allocate memory of size: %lu"
#define IOT_LL_ACESS_EMPTY_LIST "iot_linked_list:Tried to access an empty linked list"
#define IOT_LL_NULL_POINTER_NODE "iot_linked_list:node at index: %d the list is NULL"


/* Structures */

// linked list node structure
typedef struct iot_ll_node iot_ll_node;
struct iot_ll_node
{
	nn_InOutTuple* value;
	struct iot_ll_node *next;
};

// Linked List structure
typedef struct iot_linked_list iot_linked_list;
// (https://stackoverflow.com/questions/17052443/c-function-inside-struct)

struct iot_linked_list
{
	// attributes
	int length;
	size_t value_size;
	iot_ll_node *head;

	// methods
	nn_InOutTuple* (*get_value_at)(iot_linked_list*, int index);
	void (*insert_value_at)(iot_linked_list*, int index, nn_InOutTuple* value);
	iot_ll_node* (*append_value)(iot_linked_list*, nn_InOutTuple*, iot_ll_node* helper);
	nn_InOutTuple* (*remove_value_at)(iot_linked_list*, int index);
	int (*index_of)(iot_linked_list*, nn_InOutTuple* value);
};

/* Init function */
iot_linked_list* init_iot_linked_list();

/* Free function */
void free_iot_linked_list(iot_linked_list* list, bool free_value);


/* 'Private' Methods */

// free node
void _free_iot_linked_list_node(iot_ll_node* node, bool free_value);

// error message
void _iot_linked_list_exit_msg(const char* msg);

// is an index valid ?
bool _iot_linked_list_valid_index(iot_linked_list *list, int index);

// assert that an index is valid
void _iot_linked_list_assert_index(iot_linked_list *list, int index);

// get the node at the index
iot_ll_node* _iot_linked_list_get_node_at(iot_linked_list *list, int index);

// create a new node
iot_ll_node* _iot_linked_list_new_node();


/* 'Public' Methods */

// get value at index
nn_InOutTuple* iot_linked_list_get_value_at(iot_linked_list *list, int index);

// insert value at index
void iot_linked_list_insert_value_at(iot_linked_list *list, int index, nn_InOutTuple* value);

// append value
iot_ll_node* iot_linked_list_append_value(iot_linked_list *list, nn_InOutTuple* value, iot_ll_node* helper);

// remove value at index
nn_InOutTuple* iot_linked_list_remove_value_at(iot_linked_list *list, int index);

// remove value
int iot_linked_list_index_of(iot_linked_list *list, nn_InOutTuple* value);

//transforms the list into a dynamical allocated array assigned to gpl
nn_InOutTuple** iot_linked_list_to_array(iot_linked_list* list);

#endif
