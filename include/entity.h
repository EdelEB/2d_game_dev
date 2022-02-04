#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SDL.h>
#include "gf2d_sprite.h"

typedef struct ENTITY_S
{
	Uint8		_inuse;			/**<flag that is true when entity is active and false up for reassignment*/
	Sprite*		sprite;			/**<sprite used to draw the sprite*/
	Vector2D	position;		/**<where entity is located*/
	Vector2D	velocity;		/**<how entity moves*/
	Vector2D	drawScale;		/**<the scale factor for drawing entity*/
	Vector2D	mins, maxs;		/**<describe the bounding box around entity*/
	void	(*think)(struct ENTITY_S* self); 

}Entity;

/**
* @brief free memory of an entity
* @param entity the entity being drawn
*/

void entity_draw(Entity* entity);

/**
* @brief free memory of an entity
* @param entity the entity to free
*/

void entity_free(Entity* entity);

/**
* @brief creates the entity manager
* @param max_entities 
*/

void entity_manager_init(Uint32 max_entities);

/**
* @brief frees all entities in manager's array
*/

void entity_manager_clear();

/**
* @brief get a new empty entity
* @returns NULL on error, or a pointer to blank entity
*/

Entity* entity_new();

/**
* @brief decides what action an entity should do
* @param entity is the entity doing the decided upon action
*/

void entity_think(Entity* entity);

#endif