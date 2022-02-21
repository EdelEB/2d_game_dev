
#include "simple_logger.h"
#include "ent_asteroid.h"

asteroid_think(Entity* ent)
{
	if (!ent) { return; }

	vector2d_copy(ent->position, ent->velocity);


}


Entity* ent_asteroid_new(Vector2D position, Vector2D velocity, float size) {

	Entity* ent = entity_new();
	if (!ent)
	{
		slog("entity_manager could not find space for new ship. What a shitty manager");
		return NULL;
	}

	vector2d_copy(ent->draw_scale, vector2d(size, size));
	vector2d_copy(ent->velocity, velocity);

	ent->sprite = gf2d_sprite_load_image("images/asteroid.png");
	ent->think = asteroid_think;
	//ent->draw_offset.x = -64;
	//ent->draw_offset.y = -64;
	ent->rotation.x = 64;
	ent->rotation.y = 64;
	vector2d_copy(ent->position, position);
	return ent;
}