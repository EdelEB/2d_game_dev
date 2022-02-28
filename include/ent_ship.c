#include "simple_logger.h"
#include "ent_ship.h"

#define FRAME_COUNT 2   // This is here becuase the frame count in the sprite is in 2 locations and I don't feel like writing it in both every time

void ship_think(Entity* self)
{   
    int speed = 3;
    const Uint8* keys;
    
    if (!self)return;

    // Handles sprite animation reset
    self->frame = (self->frame + 0.1);
    if (self->frame >= FRAME_COUNT)self->frame = 0;

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame  

    if (keys[SDL_SCANCODE_W])
    {
        // is the ship hitting the top of the screen
        if (self->position.y - self->mins.y < 0) { self->position.y += 5; }
        // move up
        else self->position.y -= speed;
    }
    if (keys[SDL_SCANCODE_S])
    {
        // is the ship hitting the bottom of the screen
        if (self->position.y + self->maxs.y > 700) { self->position.y -= 5; }
        // move down
        else self->position.y += speed;
    }
    if (keys[SDL_SCANCODE_A])
    {
        // is the ship hitting the left of the screen
        if (self->position.x - self->mins.x < 0) { self->position.x += 5; }
        // move left
        else self->position.x -= speed;
    }
    if (keys[SDL_SCANCODE_D])
    {
        // is the ship hitting the right of the screen
        if (self->position.x + self->maxs.x > 1160) { self->position.x -= 5; }
        // move right
        else self->position.x += speed;
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
    ent->maxs = vector2d(20, 20);
    ent->mins = ent->maxs;
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