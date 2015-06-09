#ifndef CREATURE_H
#define CREATURE_H

typedef struct stat stat_t;
typedef struct creature creature_t;

// TODO standardize and expand
enum stat {

	// Physical capacity for action
	STAT_HEALTH,

	// Mental capacity for action
	STAT_MENTAL,

	// Magical or spiritual capacity for action
	STAT_SOUL,

	// skills
	STAT_SWORDSMANSHIP,
	STAT_BASKET_WEAVING,
	STAT_CARPENTRY,
	STAT_BASKETWEAVING,

	// Should always be last
	STAT_COUNT
};

#include "item.h"
#include "effect.h"

struct race {
	/**
	 * The name of the race
	 */
	char *name;

	// TODO?
};

struct stat {
	/**
	 * Effective value of the stat
	 * value = base + sum(effect values)
	 */
	int value;

	/**
	 * The base value, the value unmodified by effects
	 * base <= level
	 */
	int base;

	/**
	 * The level of the statistic, dependent upon the exp, acts as a max for the
	 * base value.
	 */
	int level;

	/*
	 * The experience points for the stat, increases can cause an increase to
	 * the level.
	 */
	int exp;
};

struct creature {
	/**
	 * The race of the creature
	 */
	race_t *race;

	/**
	 * The proper name of the creature
	 */
	char *name;

	/**
	 * The physical body of the creature
	 */
	item_t *corpus;

	/**
	 * The effective stats of the creature
	 */
	stat_t stats[STAT_COUNT];

	/**
	 * The effects currently effecting the creature
	 */
	list_t *active_effects;
};

#endif
