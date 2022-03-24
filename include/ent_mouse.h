#ifndef __ENT_MOUSE_H__
#define __ENT_MOUSE_H__

#include "entity.h"

/**
 * @brief creates an instance of a mouse entity
 * @param position the spawn location
 * @returns NULL on error or a pointer to the mouse ent
 */
Entity* ent_mouse_new(Vector2D position);

#endif