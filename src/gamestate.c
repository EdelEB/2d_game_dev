#include "gamestate.h"

void gamestate_new(void) 
{
	gamestate.food = 40;
	gamestate.map_spot = 0;
}

void gamestate_load(char* filename) 
{
	SJson* arr = sj_load(SAVE_FILE);
	int i, j;

	/*sj_get
	for (i = 0; i < MAX_CREW; i++) {
		for (j = 0; j < 7; j++) {
			gamestate.crew[i] = arr[i][j];
		}
	}
	*/

}

void gamestate_save(char* filename) 
{
	int i;
	SJson *arr, *ret = sj_array_new();
	SJson* data;

	for (i = 0; i < MAX_CREW; i++)
	{
		
		arr = sj_array_new();
		data = sj_new_str(gamestate.crew[i].name);
		sj_array_append(arr, data);
		data = sj_new_str(gamestate.crew[i].title);
		sj_array_append(arr, data);
		data = sj_new_int(gamestate.crew[i].clearance);
		sj_array_append(arr, data);
		data = sj_new_int(gamestate.crew[i].hunger);
		sj_array_append(arr, data);
		data = sj_new_int(gamestate.crew[i].morale);
		sj_array_append(arr, data);
		data = sj_new_int(gamestate.crew[i].is_alive);
		sj_array_append(arr, data);
		data = sj_new_int(gamestate.crew[i]._inuse);
		sj_array_append(arr, data);
		
		sj_array_append(ret, arr);
	}	

	sj_save(ret, SAVE_FILE);
}

Uint8 crew_check_clearance(event_clearance c)
{
	int i;

	if (c == DEFAULT) { return 1; }

	for (i = 0; i < MAX_CREW; i++) {
		if (gamestate.crew[i].is_alive && gamestate.crew[i].clearance == c)
		{
			return 1;
		}
	}

	return 0;
}

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
