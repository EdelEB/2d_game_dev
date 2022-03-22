#ifndef __MAP_H__
#define __MAP_H__

#include <SDL.h>

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

struct MAP {
	Sprite*		image;
	Sprite*		ship_image;
	Uint32		spot;
	Vector2D	position;
} map;

void map_init();

void map_set_position_for_spot(Uint32 spot);

void map_draw();

#endif