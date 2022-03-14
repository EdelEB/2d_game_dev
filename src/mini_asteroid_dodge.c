#include "ent_ship.h"
#include "ent_asteroid.h"
#include "mini_asteroid_dodge.h"

void spawn_asteroids() 
{
    int i;
    Entity* asteroid1;
    Entity* asteroid2;

    for (i = 0; i < 10; i++)
    {
        SDL_Delay(2000);

        asteroid1 = ent_asteroid_new(
            vector2d(
                simple_random(1000, 1180),
                simple_random(0, 700)
            ),
            vector2d(
                simple_random(-4, -2),
                simple_random(-1, 1)
            ),
            gfc_random() / 5 + 0.1
        );
        asteroid2 = ent_asteroid_new(
            vector2d(
                simple_random(1000, 1180),
                simple_random(0, 700)
            ),
            vector2d(
                simple_random(-4, -2),
                simple_random(-1, 1)
            ),
            gfc_random() / 5 + 0.1
        );

        slog("Asteroid Spawned");

        SDL_Delay(2500);
        entity_free(asteroid1);
        entity_free(asteroid2);
    }
}

void mini_asteroid_run(MiniGame* mini)
{
    slog("Running Asteroid Dodge");
    mini->is_running = 1;
    Entity* ship = ent_ship_new(vector2d(500, 300));
    
    slog("Asteroid Spawner Started");
    spawn_asteroids();
    slog("Asteroid Spawner Done");

    entity_free(ship);
    mini->is_running = 0;
}

void mini_asteroid_end(MiniGame* mini) {}

MiniGame* mini_asteroid_init()
{
    MiniGame *mini = mini_new();
    if (!mini)
    {
        slog("Couldn't initialize mini_asteroid");
        return NULL;
    }
    mini->title = "Asteroid Dodge";
    mini->background = gf2d_sprite_load_image("assets/images/backgrounds/bg_space.png");
    mini->code = ASTEROID_DODGE;
    mini->run = mini_asteroid_run;
    mini->end = mini_asteroid_end;

    slog("Asteroid Dodge initialized");
    return mini;
}