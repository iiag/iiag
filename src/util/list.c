#include <stdlib.h>
#include "list.h"

struct list_elm {
	void * value;
	list_elm_t * next;
};

list_t * list_new(void)
{
	list_t * ls = malloc(sizeof(list_t));
	*ls = NULL;
	return ls;
}

void list_destroy(list_t * ls)
{
	list_elm_t * tmp;
	list_elm_t * elm = *ls;

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
	elm->next = *ls;
	*ls = elm;
}

void * list_pop(list_t * ls)
{
	void * value;
	list_elm_t * elm;

	elm = *ls;
	value = elm->value;
	*ls = elm->next;
	free(elm);

	return value;
}

void list_foreach(list_t * ls, list_foreach_func func)
{
	list_elm_t * elm = *ls;

	while (elm) {
		func(elm->value);
		elm = elm->next;
	}
}
