#include "simple_logger.h"
#include "ent_asteroid.h"

#include "tools.h"

asteroid_think(Entity* ent)
{
	if (!ent) { return; }
	
	vector2d_add(ent->position, ent->position, ent->velocity);
	
}

Entity* ent_asteroid_new(Vector2D position, Vector2D velocity, float size) {

	Entity* ent = entity_new();
	if (!ent)
	{
		slog("entity_manager could not find space for new ship. What a shitty manager");
		return NULL;
	}

	ent->team = ANTI_PLAYER;

	Vector2D direction;
	int mx, my;
	float angle;

	ent->sprite = gf2d_sprite_load_image("assets/images/asteroid.png");
	ent->think = asteroid_think;

	vector2d_copy(ent->draw_scale, vector2d(size, size));
	ent->maxs = ent->draw_scale;
	ent->mins = ent->maxs;

	vector2d_copy(ent->velocity, velocity);
	
	//ent->draw_offset.x = -64;
	//ent->draw_offset.y = -64;	
	
	mx = -100;
	my = simple_random(0, 700);
	direction.x = mx - ent->position.x;
	direction.y = my - ent->position.y;
	angle = vector2d_angle(
		vector2d(
			simple_random(0,180),
			simple_random(0,180)
		)
	);
	ent->rotation.z = angle;
	vector2d_copy(ent->position, position);
	
	slog("Asteroid Spawned at (%d,  %d)", position.x, position.y);
	return ent;
}