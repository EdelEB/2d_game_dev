#include "ent_mouse.h"

void mouse_think(Entity* e)
{
    if (!e) { slog("mouse think recieved NULL Entity*"); return; }
    
    if (e->hitbox.y + e->hitbox.h > WINDOW_HEIGHT)
    {
        e->position.y += 11;
    }
    else if (e->hitbox.y < 0)
    {
        e->position.y += 11;
    }

}

Entity* ent_mouse_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("entity_manager could not find space for new mouse");
        return NULL;
    }

    ent->team = NPE;
    ent->sprite = gf2d_sprite_load_image("assets/images/mouse.png");
    ent->maxs = vector2d(18, 20);
    ent->mins = ent->maxs;
    ent->think = mouse_think;
    //ent->draw_offset.x = -64;
    //ent->draw_offset.y = -64;
    //ent->rotation.x = 64;
    //ent->rotation.y = 64;
    ent->draw_scale.x = 0.2;
    ent->draw_scale.y = 0.2;
    vector2d_copy(ent->position, position);

    SDL_Rect rect;
    rect.x = ent->position.x;
    rect.y = ent->position.y;
    rect.w = ((int)(ent->maxs.x) << 2);
    rect.h = ((int)(ent->maxs.y) << 2);
    ent->hitbox = rect;

    return ent;
}