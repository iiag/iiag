#ifndef EFFECT_H
#define EFFECT_H

typedef struct potential potential_t;
typedef struct effect effect_t;

#include "creature.h"

/**
 * The "class" of some collection of effects
 */
struct potential {
	/**
	 * The statistic effects of this potential take hold of
	 */
	enum stat stat;

	/**
	 * The initial value the effect has
	 */
	int value;

	/**
	 * The initial rate of change of the value
	 */
	int velocity;

	/**
	 * The rate of change of the velocity
	 */
	int acceleration;
};

/**
 * An effect that is actually effecting a creature
 */
struct effect {
	/**
	 * The potential of the effect, its "class"
	 */
	potential_t *potential;

	/**
	 * The creature the effect has taken hold of
	 *
	 * TODO not sure if we should have this
	 */
	creature_t *creature;

	/**
	 * The current value of the effect
	 */
	int value;

	/**
	 * The current velocity of the effect
	 */
	int velocity;
};

/**
 * Adds an effect to a creature
 *
 * @param potential  The potential to make the effect from
 * @param creature   The creature to effect
 * @return The created effect
 *
 * TODO Should this attach the effect?
 * TODO If so, should this return the effect?
 */
effect_t *effect_add(potential_t *potential, creature_t *creature);

/**
 * Removes an effect from a creature
 *
 * @param effect  The effect to remove
 */
void effect_remove(effect_t *effect);

#endif
