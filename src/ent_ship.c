#include "simple_logger.h"
#include "ent_ship.h"

void ship_think(Entity* self)
{
    Vector2D direction;
    int mx, my;
    float angle;
    const Uint8* keys;
    if (!self)return;
    self->frame = (self->frame + 0.1);
    if (self->frame >= 16)self->frame = 0;

    SDL_GetMouseState(&mx, &my);
    direction.x = mx - self->position.x;
    direction.y = my - self->position.y;
    angle = vector2d_angle(direction) - 90;
    self->rotation.z = angle;

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    if (keys[SDL_SCANCODE_W])
    {
        // move forward
        vector2d_set_magnitude(&direction, 3);
        vector2d_copy(self->velocity, direction);
    }
    else
    {
        vector2d_scale(self->velocity, self->velocity, 0.5);
        if (vector2d_magnitude(self->velocity) < 0.05)
        {
            vector2d_clear(self->velocity);
        }
    }

}

Entity* ent_ship_new(Vector2D position)
{
    Entity* entity = entity_new();

 /*   if (!entity)
    {
        slog("entity_manager could not find space for new ship. What a shitty manager");
        return NULL;
    }*/
    entity->sprite = gf2d_sprite_load_all("images/space_bug_top.png", 128, 128, 16);
    entity->think = ship_think;
    entity->draw_offset.x = -64;
    entity->draw_offset.y = -64;
    entity->rotation.x = 64;
    entity->rotation.y = 64;
    vector2d_copy(entity->position, position);
    return entity;
}


//eof