#ifndef __ENT_SHIP_H__
#define __ENT_SHIP_H__

#include "entity.h"

/**
 * @brief creates an instance of a ship entity
 * @param position the spawn location
 * @returns NULL on error or a pointer to the ship ent
 */
Entity* ent_ship_new(Vector2D position);

#endif