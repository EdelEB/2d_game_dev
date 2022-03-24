#ifndef __MINI_MOUSE_HUNT_H__
#define __MINI_MOUSE_HUNT_H__

#include <SDL.h>
#include "gf2d_graphics.h"

#include "tools.h"
#include "mini_game.h"
#include "ent_mouse.h"



MiniGame* mini_mouse_init();

void mini_mouse_start(MiniGame* self);

void mini_mouse_run(MiniGame* self);

void mini_mouse_end(MiniGame* self);

#endif