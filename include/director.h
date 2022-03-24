#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include <SDL.h>

#include "gamestate.h"
#include "entity.h"
#include "event.h"
#include "notification.h"
#include "mini_game.h"
#include "mini_asteroid_dodge.h"
#include "mini_mouse_hunt.h"
#include "mini_ration_split.h"
#include "map.h"

typedef enum {
	NO_STATE,
	MENU,
	EVENT,
	NOTE,
	MINI
}state_type;

/*
* @brief prepares the director for the rest of its uses. Initializes all minigames and menus 
*/

void director_init();

/*
* @brief draws the screen corresponding to the state provided
* @param id is the gamestate_id used to determine what will be drawn
*/
void director_draw(gamestate_id id);

/*
* @brief runs the "think" equivalent of whatever state the game is in. Minigame/Event/Menu
* @param id	is the gamestate_id that determines what thinks are run
* @param mouse_state is 1 when the mouse is clicked and 0 when not
* @param mx is the mouse's location on the x axis
* @param my is the mouse's location on the y axis
* @return gamestate_id of next gamestate to be run, NONE otherwise 
*/
gamestate_id director_think(gamestate_id id, Uint32 mouse_state, int *mx, int *my);

#endif