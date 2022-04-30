#ifndef __MAP_H__
#define __MAP_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "ui_stuff.h"
#include "menu_crew_view.h"
#include "gamestate.h"

//int traveling = 0;
//const int travel_time = 70;

struct MAP {
	Sprite	   *image;
	Sprite	   *ship_image;
	Uint32		spot;
	ui_button	*crew_view_button, *travel_button;
	Vector2D	position;
} map;

void map_init(void);

void map_set_position_by_spot(Uint32 spot);

void map_draw(void);

gamestate_id map_listen(Uint32 mouse_state, int mx, int my);

gamestate_id map_to_crew_view(void);

gamestate_id map_travel(void);

#endif