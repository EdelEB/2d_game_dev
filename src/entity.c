
#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_color.h"

#include "entity.h"

typedef struct
{
	Uint32 max_entities;		/**<max number of entities in game*/
	Entity* entity_list;		/**<array of all entities*/
}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_draw(Entity* entity) 
{
	gf2d_sprite_draw(
		entity->sprite,
		entity->draw_scale,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		entity->sprite->frame_h);

}

void entity_free(Entity* entity)
{
	if (!entity) {
		slog("Attempted to free null entity pointer. You are nothing to me");
		return;
	}
	if (entity->sprite) {
		gf2d_sprite_free(entity->sprite);
	}
	memset(entity, 0, sizeof(Entity));
}

void entity_manager_clear()
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (!entity_manager.entity_list[i]._inuse)continue;
		entity_free(&entity_manager.entity_list[i]);
	}
}

void entity_manager_close()
{
	entity_manager_clear;

	if (entity_manager.entity_list){
		free(entity_manager.entity_list);								// is this allowed ? 
	}

	slog("entity manager closed");
}

void entity_manager_draw_all() 
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (!entity_manager.entity_list[i]._inuse) continue;

		entity_draw(&entity_manager.entity_list[i]);
	}
}

void entity_manager_init(Uint32 max_entities)
{
	int i;

	if (max_entities == 0)
	{
		slog("Cannot allocate memory for zero entities. Stop wasting my time");
		return;
	}
	if (entity_manager.entity_list) 
	{
		slog("Entity manager already initialized dumb dumb");
	}
	entity_manager.max_entities = max_entities;
	entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max_entities);

	atexit(entity_manager_close);
	slog("entity manager initialized");
}

Entity* entity_new() 
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse) continue;

		entity_manager.entity_list[i]._inuse = 1;
		entity_manager.entity_list[i].draw_scale.x = 1;
		entity_manager.entity_list[i].draw_scale.y = 1;
		return &entity_manager.entity_list[i];
	}

	// need a bigger max_entities; this shouldn't be a problem right now though
	
}

void entity_think(Entity* entity) 
{

	if (!entity) 
	{ 
		slog("Nothing cannot think, \nalthough we can think of nothing,\nor is nothing something?\nAnyways, entity_think() called with NULL pointer");
		return; 
	}

	if (entity->think) 
	{
		entity->think(entity);
	}

	vector2d_add(entity->position, entity->position, entity->velocity);

	return;
}

void entity_manager_think_all() 
{

	int i;

	if (!entity_manager.entity_list)
	{ 
		slog("No entity list for entity_think_all()");
		return;
	}

	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (!entity_manager.entity_list[i]._inuse)continue;
		entity_think(&entity_manager.entity_list[i]);
	}
}

// eof