#ifndef __MINI_ASTEROID_DODGE_H__
#define __MINI_ASTEROID_DODGE_H__

#include <SDL.h>
#include "mini_game.h"

MiniGame* mini_asteroid_init();

void mini_asteroid_run();

void mini_asteroid_close (MiniGame* mini);

#endif