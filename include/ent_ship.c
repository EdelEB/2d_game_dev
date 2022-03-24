#include "simple_logger.h"
#include "ent_ship.h"

// This is here becuase the frame count in the sprite is in 2 locations and 
// I don't feel like writing it in both every time I mess with it
const Uint8 FRAME_COUNT = 2;

void ship_think(Entity* self)
{   
    Uint32 speed = 3;
    const Uint8* keys;
    
    if (!self)return;

    // Handles sprite animation reset
    self->frame = (self->frame + 0.1);
    if (self->frame >= FRAME_COUNT)self->frame = 0;

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame  

    //this is the wrong place to do this but I need it working right now
    if (keys[SDL_SCANCODE_W])
    {
        // is the ship hitting the top of the screen
        if (self->position.y < 0) { self->position.y += 5; }
        else self->position.y -= speed;
    }
    if (keys[SDL_SCANCODE_S])
    {
        // is the ship hitting the bottom of the screen
        if (self->position.y + self->maxs.y > 700) { self->position.y -= 5; }
        else self->position.y += speed;
    }
    if (keys[SDL_SCANCODE_A])
    {
        // is the ship hitting the left of the screen
        if (self->position.x < 0) { self->position.x += 5; }
        else self->position.x -= speed;
    }
    if (keys[SDL_SCANCODE_D])
    {
        // is the ship hitting the right of the screen
        if (self->position.x + self->maxs.x > 1160) { self->position.x -= 5; }
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

    ent->team       = PLAYER;
    ent->sprite     = gf2d_sprite_load_all("assets/images/my_ship.png", 128, 128, FRAME_COUNT);
    ent->maxs       = vector2d(20, 14);
    ent->mins       = ent->maxs;
    ent->think      = ship_think;
    ent->draw_offset.x  = -42;
    ent->draw_offset.y  = -55;
    ent->rotation.x     = 64;
    ent->rotation.y     = 64;
    ent->draw_scale.x   = 1.2;
    ent->draw_scale.y   = 1.2;
    vector2d_copy(ent->position, position);
    
    SDL_Rect rect;
    rect.x = ent->position.x;
    rect.y = ent->position.y;
    rect.w = ((int)(ent->maxs.x) << 2);
    rect.h = ((int)(ent->maxs.y) << 2);
    ent->hitbox = rect;

    return ent;
}

//eof