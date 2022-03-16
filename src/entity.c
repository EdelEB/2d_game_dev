
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

Entity* entity_new()
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse) continue;

		entity_manager.entity_list[i]._inuse = 1;
		entity_manager.entity_list[i].mini_code = NO_GAME;
		entity_manager.entity_list[i].draw_scale.x = 1;
		entity_manager.entity_list[i].draw_scale.y = 1;
		return &entity_manager.entity_list[i];
	}
	return NULL;  // you need a bigger max_entities and bigger entity_list; this shouldn't be a problem right now though
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
		if (entity_manager.entity_list[i]._inuse)
		{
			entity_free(&entity_manager.entity_list[i]);
		}
	}
}

void entity_manager_close()
{
	entity_manager_clear;

	if (entity_manager.entity_list) 
	{
		free(entity_manager.entity_list);								// is this allowed ? 
	}

	slog("entity manager closed");
}

void entity_manager_init(Uint32 max_entities)
{
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

void entity_manager_think_mini(mini_code code)
{
	int i;

	if (!entity_manager.entity_list)
	{
		slog("No entity list for entity_think_all()");
		return;
	}

	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse &&
			entity_manager.entity_list[i].mini_code == code) 
		{
			entity_think(&entity_manager.entity_list[i]);
		}
	}
}

void entity_draw(Entity* ent) 
{
	Vector2D draw_position;
	if (!ent) 
	{
		slog("Cannot draw NULL entity. I am not an abstract artist");
		return;
	}
	if (!ent->sprite) { return; } // invisible entity?

	vector2d_add(draw_position, ent->position, ent->draw_offset);
	gf2d_sprite_draw(
		ent->sprite,
		draw_position,
		&ent->draw_scale,
		NULL,
		&ent->rotation,
		NULL,
		NULL,
		(Uint32)ent->frame);	// I don't know whu it needs to be casted as an int
}

void entity_manager_draw_all()
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse) 
		{
			entity_draw(&entity_manager.entity_list[i]);
		}
	}
}

void entity_manager_draw_mini(mini_code code)
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse && 
			entity_manager.entity_list[i].mini_code == code)
		{
			entity_draw(&entity_manager.entity_list[i]);
		}
	}
}

// eof