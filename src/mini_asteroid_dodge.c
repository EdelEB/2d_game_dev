#include "ent_ship.h"
#include "ent_asteroid.h"
#include "mini_asteroid_dodge.h"
#include "tools.h"

extern const Uint32 WINDOW_WIDTH, WINDOW_HEIGHT;

const Uint32 MAX_ROUNDS = 5;
Uint32 rounds = 0;
#define ASTEROID_WAIT 750
int asteroid_spawn_timer = ASTEROID_WAIT;

MiniGame asteroid_dodge = { 0 };

MiniGame* mini_asteroid_init()
{
    asteroid_dodge.title = "Asteroid Dodge";
    asteroid_dodge.background = gf2d_sprite_load_image("assets/images/backgrounds/bg_space.png");
    asteroid_dodge.id = MINI_ASTEROID_DODGE;
    asteroid_dodge.end_state = NONE;
    asteroid_dodge.start    = mini_asteroid_start;
    asteroid_dodge.run      = mini_asteroid_run;
    asteroid_dodge.end      = mini_asteroid_end;

    slog("Asteroid Dodge initialized");
    return &asteroid_dodge;
}

void mini_asteroid_start(MiniGame* self)
{
    mini_start(self);
    
    rounds = 0;

    self->entities[0] = ent_ship_new(vector2d(500,300));
    
    for (int i = 1; i < 4; i++) 
    {
        self->entities[i] = ent_asteroid_new();
        self->entities[i]->position = vector2d(WINDOW_WIDTH<<1, WINDOW_HEIGHT<<1);
    }
}


Uint8 check_collision(MiniGame* self) 
{
    Entity* ship = self->entities[0];
    Entity* ast;
    int ship_right, ship_bottom, a_right, a_bottom;
    Uint8 a, b, c, d;

    
    for (int i = 1; i < 4; i++)
    {
        ast = self->entities[i];
        ship_right = ship->hitbox.x + ship->hitbox.w;
        ship_bottom = ship->hitbox.y + ship->hitbox.h;
        a_right = ast->hitbox.x + ast->hitbox.w;
        a_bottom = ast->hitbox.y + ast->hitbox.h;

        a = (ast->hitbox.x > ship->hitbox.x && ast->hitbox.x < ship_right);
        b = (ast->hitbox.y > ship->hitbox.y && ast->hitbox.y < ship_bottom);
        c = (a_bottom > ship->hitbox.y && a_bottom < ship_bottom);
        d = (a_right > ship->hitbox.x && a_right < ship_right);

        if ( a && (b || c) || d && (b || c) )
            return 1;
    }
    return 0;
}

void mini_asteroid_run(MiniGame* self)
{
    if (asteroid_spawn_timer > 0) 
    { 
        asteroid_spawn_timer--; 
    }
    else
    {
        randomize_asteroids();
        rounds++;
        asteroid_spawn_timer = ASTEROID_WAIT;
    }

    if (rounds > MAX_ROUNDS) {
        self->end_state = SUCCESS_ASTEROID_DODGE;
        return;
    }
    if (check_collision(self))
    {
        self->end_state = FAIL_ASTEROID_DODGE;
        return;
    }

    return;
}

void mini_asteroid_end(MiniGame* mini)
{

    slog("mini_asteroid_dodge ended");
}


void randomize_asteroids()
{
    int i;
    for(i = 1 ; i<4 ; i++)
    {
        if (!asteroid_dodge.entities[i]) continue;

        ent_asteroid_randomize( 
            asteroid_dodge.entities[i],
            vector2d(
                simple_random(WINDOW_WIDTH, WINDOW_WIDTH+50),
                simple_random(0, 700)
            ),
            vector2d(
                simple_random(-4, -2),
                simple_random(-1, 1)
            ),
            (gfc_random() / 5 + 0.1)
        );
    }
}


