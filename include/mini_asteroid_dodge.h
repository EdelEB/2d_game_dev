#ifndef __MINI_ASTEROID_DODGE_H__
#define __MINI_ASTEROID_DODGE_H__

#include "mini_game.h"
#include "ent_ship.h"
#include "ent_asteroid.h"

MiniGame* mini_asteroid_init();

void mini_asteroid_start(MiniGame* self);

void mini_asteroid_run(MiniGame* self);

void mini_asteroid_end (MiniGame* self);

void randomize_asteroids();

#endif