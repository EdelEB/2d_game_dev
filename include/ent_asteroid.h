#ifndef __ENT_ASTEROID_H__
#define __ENT_ASTEROID_H__

#include "entity.h"

// Asteroids are used to 

/**
 * @brief creates an instance of a asteroid entity
 * @param position the spawn location
 * @param size float used to scale the asteroid
 * @param velocity speed and direction the asteriod is moving
 * @returns NULL on error or a pointer to the asteroid ent
 */
Entity* ent_asteroid_new(Vector2D position, Vector2D velocity, float size);

#endif