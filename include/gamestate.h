#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <SDL.h>
#include "crew.h"
#include "enum_declarations.h"

struct GAMESTATE{
	crew_member		crew[MAX_CREW];
	Uint32			food;
	Uint32			map_spot; 
	gamestate_id	completed_events[40];
}gamestate;

/*
* @brief Sets gamestate to the default start values
*/
void gamestate_new(void);

void gamestate_load(char* filename);

void gamestate_save(char* filename);

#endif // !__GAME_STATE_H__
