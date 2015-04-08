#ifndef LIST_H
#define LIST_H

typedef struct list_elm list_elm_t;
typedef list_elm_t * list_t;

typedef void (* list_foreach_func)(void *, void *);

// Instantiates a new, empty list
list_t * list_new(void);

// Frees a list and all elements in it
// Call list_foreach if custom destruct logic is needed
void list_destroy(list_t *);

// Pushes a new element to the begining of the list
void list_push(list_t *, void *);

// Pops an element from the begining of the list
void * list_pop(list_t *);

// Applies a function to each element in the list
// The first argument to the given function will be the element's value, the 2nd `arg2`
void list_foreach(list_t *, void * arg2, list_foreach_func);

#endif
