#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct list_elm list_elm_t;

struct list {
	list_elm_t * front;
	list_elm_t * back;
	size_t size;
};

struct list_elm {
	void * value;
	list_elm_t * next;
	list_elm_t * prev;
};

list_t * list_new(void)
{
	list_t * ls = malloc(sizeof(list_t));
	ls->front = NULL;
	ls->back = NULL;
	ls->size = 0;
	return ls;
}

void list_destroy(list_t * ls)
{
	list_elm_t * tmp;
	list_elm_t * elm = ls->front;

	while (elm) {
		tmp = elm->next;
		free(elm);
		elm = tmp;
	}

	free(ls);
}

void list_push(list_t * ls, void * value)
{
	list_elm_t * elm = malloc(sizeof(list_elm_t));

	elm->value = value;
	elm->next = ls->front;
	elm->prev = NULL;

	if (ls->back) {
		// Is not the first element in the list
		ls->front->prev = elm;
	} else {
		// Is the first element in the list
		ls->back = elm;
	}

	ls->size++;
	ls->front = elm;
}

void * list_pop(list_t * ls)
{
	void * value;
	list_elm_t * elm;

	if (ls->front) {
		elm = ls->front;
		value = elm->value;

		if (ls->front == ls->back) {
			// Last element in list
			ls->back = NULL;
		} else {
			// Not last element in list
			ls->front->next->prev = NULL;
		}

		ls->size--;
		ls->front = elm->next;
		free(elm);
	} else {
		// Nothing to pop, return null
		value = NULL;
	}

	return value;
}

void list_push_back(list_t * ls, void * value)
{
	list_elm_t * elm = malloc(sizeof(list_elm_t));

	elm->value = value;
	elm->next = NULL;
	elm->prev = ls->back;

	if (ls->front) {
		// other elements in list
		ls->back->next = elm;
	} else {
		// first element in list
		ls->front = elm;
	}

	ls->size++;
	ls->back = elm;
}

void * list_pop_back(list_t * ls)
{
	void * value;
	list_elm_t * elm;

	if (ls->back) {
		elm = ls->back;
		value = elm->value;

		if (ls->front == ls->back) {
			// Last element in list
			ls->front = NULL;
		} else {
			ls->back->prev->next = NULL;
		}

		ls->size--;
		ls->back = elm->prev;
		free(elm);
	} else {
		// Nothing left to pop, return null
		value = NULL;
	}

	return value;
}

int list_size(list_t * ls)
{
	return ls->size;
}

void list_foreach(list_t * ls, void * arg2, list_foreach_func func)
{
	list_elm_t * elm = ls->front;

	while (elm) {
		func(elm->value, arg2);
		elm = elm->next;
	}
}

void list_foreach_reverse(list_t * ls, void * arg2, list_foreach_func func)
{
	list_elm_t * elm = ls->back;

	while (elm) {
		func(elm->value, arg2);
		elm = elm->prev;
	}
}
