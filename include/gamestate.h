#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <SDL.h>

#include "simple_json.h"

#include "crew.h"
#include "enum_declarations.h"

#define SAVE_FILE "saves/save_game.json"

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


Uint8 crew_check_clearance(event_clearance c);

gamestate_id crew_lower_morale();
gamestate_id crew_raise_morale();
gamestate_id crew_lower_hunger();
gamestate_id crew_raise_hunger();

#endif // !__GAME_STATE_H__
