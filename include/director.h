#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include <SDL.h>

#include "gamestate.h"
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

void director_draw(gamestate_id id);

gamestate_id director_think(gamestate_id id, Uint32 mouse_state, int *mx, int *my);

#endif