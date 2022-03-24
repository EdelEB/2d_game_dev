#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <SDL.h>
#include "crew.h"
#include "enum_declarations.h"


struct GAMESTATE{
	crew_member		crew[5];
	Uint32			food;
	Uint32			map_spot; 
	gamestate_id	completed_events[40];
}gamestate;

//gamestate* load_gamestate(char* filename);

//void save_gamestate(char* filename);

#endif // !__GAME_STATE_H__
