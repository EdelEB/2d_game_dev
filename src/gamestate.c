#include "gamestate.h"

void gamestate_new(void) 
{
	gamestate.food = 40;
	gamestate.map_spot = 0;
}

void gamestate_load(char* filename) {}

void gamestate_save(char* filename) {}