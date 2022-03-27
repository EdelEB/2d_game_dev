#include "gamestate.h"

void gamestate_new(void) 
{
	gamestate.food = 40;
	gamestate.map_spot = 0;
}

void gamestate_load(char* filename) {}

void gamestate_save(char* filename) {}

gamestate_id crew_lower_morale()
{
	int i;
	Uint8 is_breakdown = 0;

	for (i = 0; i < MAX_CREW; i++)
	{
		if (gamestate.crew[i].is_alive)
		{
			if (gamestate.crew[i].morale > 0)
			{
				gamestate.crew[i].morale--;
			}
			else
			{
				is_breakdown = 1;
			}
		}
	}
	if (is_breakdown) { return EVENT_CREW_BREAKDOWN; }

	return NONE;
}

gamestate_id crew_raise_morale()
{
	int i;
	for (i = 0; i < MAX_CREW; i++)
	{
		if (gamestate.crew[i].is_alive)
		{
			if (gamestate.crew[i].morale != 10) 
			{ 
				gamestate.crew[i].morale++; 
			}
		}
	}
	return NONE;
}

gamestate_id crew_lower_hunger()
{
	int i;
	Uint8 is_starving = 0;

	for (i = 0; i < MAX_CREW; i++)
	{
		if (gamestate.crew[i].is_alive)
		{
			if (gamestate.crew[i].hunger > 0) 
			{ 
				gamestate.crew[i].hunger--; 
			}
			else 
			{ 
				is_starving = 1; 
			}
		}
	}
	if (is_starving) { return EVENT_CREW_STARVING; }

	return NONE;
}

gamestate_id crew_raise_hunger()
{
	int i;
	for (i = 0; i < MAX_CREW; i++)
	{
		if (gamestate.crew[i].is_alive)
		{
			if (gamestate.crew[i].hunger != 10)
			{
				gamestate.crew[i].hunger++;
			}
		}
	}
	return NONE;
}
