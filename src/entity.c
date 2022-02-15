
#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_color.h"

#include "entity.h"

typedef struct
{
	Uint32 max_entities;		/**<*/
	Entity* entity_list;		/**<*/
}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_draw(Entity* entity) 
{
	gf2d_sprite_draw(
		entity->sprite,
		entity->drawScale,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		entity->sprite->frame_h);

}

void entity_free(Entity* entity)
{
	memset(entity, 0, sizeof(entity));
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

	free(&entity_manager);								// is this allowed ? 

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
		slog("cannot allocate memory for zero entities");
		return;
	}
	entity_manager.max_entities = max_entities;
	entity_manager.entity_list = malloc(sizeof(Entity) * max_entities);

	for (i = 0; i < max_entities; i++) { // IDK
		Entity temp;
		temp._inuse = 0;
		entity_manager.entity_list[i] = temp;
	}
	
	

	atexit(entity_manager_close);
}

Entity* entity_new() 
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (!entity_manager.entity_list[i]._inuse)continue;

		entity_manager.entity_list[i]._inuse = 1;
		entity_manager.entity_list[i].drawScale.x = 1;
		entity_manager.entity_list[i].drawScale.y = 1;
		return &entity_manager.entity_list[i];
	}

	// need a bigger max_entities; this shouldn't be a problem right now though
	
}

void entity_think(Entity* entity) {
	return;
}

// eof