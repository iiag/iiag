#ifndef ITEM_H
#define ITEM_H

typedef struct item item_t;
typedef struct form form_t;

#include "list.h"
#include "effect.h"

#define ITEM_CONSUMABLE 1
#define ITEM_EQUIPPABLE 2

struct form {
	/**
	 * A bit field contain type flags
	 *
	 * Valid type flags have the ITEM_ prefix
	 */
	unsigned type;
};

struct item {
	/**
	 * The form the item takes, i.e. its "class"
	 */
	form_t *form;

	/**
	 * Effects the item causes upon cosumption
	 */
	list_t *consume_potentials;

	/**
	 * Effects the item causes upon equipping
	 */
	list_t *equip_potentials;
};

#endif
