#include "ent_ship.h"
#include "ent_asteroid.h"
#include "mini_asteroid_dodge.h"

MiniGame asteroid_dodge = { 0 };

void spawn_asteroids() 
{
    int i;
    Entity* asteroid1, * asteroid2, *asteroid3;

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
        asteroid3 = ent_asteroid_new(
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

        asteroid1->mini_code = ASTEROID_DODGE;
        asteroid2->mini_code = ASTEROID_DODGE;
        asteroid3->mini_code = ASTEROID_DODGE;

        SDL_Delay(2500);
        entity_free(asteroid1);
        entity_free(asteroid2);
        entity_free(asteroid3);
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
    asteroid_dodge.title        = "Asteroid Dodge";
    asteroid_dodge.background   = gf2d_sprite_load_image("assets/images/backgrounds/bg_space.png");
    asteroid_dodge.code         = ASTEROID_DODGE;
    asteroid_dodge.run          = mini_asteroid_run;
    asteroid_dodge.end          = mini_asteroid_end;

    slog("Asteroid Dodge initialized");
    return &asteroid_dodge;
}