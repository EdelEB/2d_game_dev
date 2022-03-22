#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SDL.h>
#include "gf2d_sprite.h"

#include "mini_game.h"

typedef enum {
    PLAYER,         /**<human controlled entity*/
    NPE,            /**<Non Player Entity : player phases through*/
    ANTI_PLAYER,    /**<kills player on impact*/
    WORLD           /**<player collides with these and some other entities*/
}team_code;

typedef struct ENTITY_S
{
    Uint8          _inuse;          /**<this flag keeps track if this entity is active or free to reassign*/
    team_code       team;           /**<this defines who an entity will collide with and how*/
    game_state_id   id;             /**<this defines what mini game an entity is part of*/
    Sprite*         sprite;         /**<sprite used to draw the sprite*/
    float           frame;          /**<current frame to draw*/
    Vector2D        draw_offset;    /**<draw position relative to the entity position*/
    Vector2D        position;       /**<where our entity lives*/
    Vector2D        velocity;       /**<how our entity moves*/
    Vector3D        rotation;       /**<how to rotate the sprite*/
    Vector2D        draw_scale;     /**<the scale factor for drawing the sprite*/
    Vector2D        mins, maxs;     /**<describe the bounding box around this entity*/
    void (*think)(struct ENTITY_S* self);   /**<a pointer to a think function for this entity*/
}Entity;

/**
 * @brief initialize the internal entity entity_manager_init
 * @note must be called before other entity functions
 * @param max_entities how many concurrent entities will be supported
 */
void entity_manager_init(Uint32 max_entities);

/**
 * @brief draws all active entities to the screen
 */
void entity_manager_draw_all();

/**
 * @brief draws all active entities with game_state_id mini to the screen
 */
void entity_manager_draw_mini(game_state_id code);

/**
 * @brief runs any think function for all active entities
 */
void entity_manager_think_all();

/**
 * @brief runs any think function for all active entities with game_state_id mini
 */
void entity_manager_think_mini(game_state_id code);

/**
 * @brief free all active entities
 * @note for use in level transitions.
 */
void entity_manager_clear();

/**
 * @brief get a new empty entity
 * @returns NULL on error, or a pointer to a blank entity
 */
Entity* entity_new();

/**
 * @brief draws the given entity
 * @param entity the entity to draw
 */
void entity_draw(Entity* entity);

/**
 * @brief free the memory of an entity
 * @param entity the entity to free
 */
void entity_free(Entity* entity);

#endif