#include "simple_logger.h"
#include "ent_ship.h"

#define FRAME_COUNT 2   // This is here becuase the frame count in the sprite is in 2 locations and I don't feel like writing it in both every time

void ship_think(Entity* self)
{
    Vector2D direction;
    int mx, my;
    float angle;
    const Uint8* keys;
    if (!self)return;
    self->frame = (self->frame + 0.1);
    if (self->frame >= FRAME_COUNT)self->frame = 0;

    
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
    Entity* ent;
    ent = entity_new();

    if (!ent)
    {
        slog("entity_manager could not find space for new ship. What a shitty manager");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/my_ship.png", 128, 128, FRAME_COUNT);
    ent->think = ship_think;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->draw_scale.x = 1.2;
    ent->draw_scale.y = 1.2;
    vector2d_copy(ent->position, position);
    return ent;
}


//eof