#include "simple_logger.h"
#include "ent_asteroid.h"

#include "tools.h"

void asteroid_think(Entity* self)
{
	if (!self) { return; }
	
	vector2d_add(self->position, self->position, self->velocity);	
}

void ent_asteroid_randomize(Entity* ent, Vector2D position, Vector2D velocity, float size)
{
	Vector2D direction;
	int mx, my;
	float angle;


	slog("draw scale: %f", size);
	vector2d_copy(ent->draw_scale, vector2d(size, size));
	ent->maxs = vector2d(ent->draw_scale.x * 200, ent->draw_scale.y *200);
	ent->mins = ent->maxs;

	vector2d_copy(ent->velocity, velocity);

	//ent->draw_offset.x = -64;
	//ent->draw_offset.y = -64;	

	mx = -100;
	my = simple_random(0, 700);
	direction.x = mx - ent->position.x;
	direction.y = my - ent->position.y;
	/*angle = vector2d_angle(
		vector2d(
			simple_random(0, 180),
			simple_random(0, 180)
		)
	);
	ent->rotation.z = angle;*/
	vector2d_copy(ent->position, position);

	SDL_Rect rect;
	rect.x = ent->position.x;
	rect.y = ent->position.y;
	rect.w = ((int)(ent->maxs.x) << 2);
	rect.h = ((int)(ent->maxs.y) << 2);
	ent->hitbox = rect;
}

Entity* ent_asteroid_new() {

	Entity* ent = entity_new();
	if (!ent)
	{
		slog("entity_manager could not find space for new asteroid in ent_asteroid_new");
		return NULL;
	}

	ent->team = ANTI_PLAYER;
	ent->sprite = gf2d_sprite_load_image("assets/images/asteroid.png");
	ent->think = asteroid_think;

	slog("Asteroid Spawned in ent_asteroid_new()");
	
	return ent;
}

