#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include <SDL.h>

#include "game_state.h"
#include "entity.h"
#include "event.h"
#include "notification.h"
#include "mini_game.h"
#include "mini_asteroid_dodge.h"
#include "map.h"

typedef enum {
	NO_STATE,
	MENU,
	EVENT,
	NOTE,
	MINI
}state_type;

void director_init();

void director_draw(game_state_id id);

game_state_id director_think(game_state_id id, Uint32 mouse_state, int *mx, int *my);

#endif