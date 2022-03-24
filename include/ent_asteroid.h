#ifndef __ENT_ASTEROID_H__
#define __ENT_ASTEROID_H__

#include "entity.h"

// Asteroids are used to 

/**
* @brief creates a new asteroid entity
* @returns a pointer to an asteroid entity or NULL on Error
*/
Entity* ent_asteroid_new();

/**
 * @brief sets given asteroid entity asttributes randomly instance of a asteroid entity
 * @param asteroid is the entity whose attributes are being adjusted
 * @param position the spawn location
 * @param size float used to scale the asteroid
 * @param velocity speed and direction the asteriod is moving
 */
void ent_asteroid_randomize(Entity* asteroid, Vector2D position, Vector2D velocity, float size);

void asteroid_think(Entity* self);

#endif