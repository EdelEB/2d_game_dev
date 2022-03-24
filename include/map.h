#ifndef __MAP_H__
#define __MAP_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "gamestate.h"

struct MAP {
	Sprite	   *image;
	Sprite	   *ship_image;
	Uint32		spot;
	TTF_Font   *crew_button_font;
	SDL_Texture*crew_button_texture;
	SDL_Rect	crew_button_rect;
	Vector2D	position;
} map;

void map_init();

void map_set_position_by_spot(Uint32 spot);

void map_draw();

gamestate_id map_listen(Uint32 mouse_state, int mx, int my);

#endif