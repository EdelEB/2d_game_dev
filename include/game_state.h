#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <SDL.h>
#include "crew.h"
#include "event.h"

typedef struct{
	crew_member		crew[5];
	Uint16			food;
	Uint32			progress; 
	event_title		completed_events[20];
}game_state;

game_state* load_gamestate(char* filename);

void save_gamestate(char* filename);

#endif // !__GAME_STATE_H__
