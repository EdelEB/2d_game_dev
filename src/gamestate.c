#include "gamestate.h"

void gamestate_new(void) 
{
	gamestate.food = 40;
	gamestate.map_spot = 0;
}

void gamestate_load(char* filename) 
{
	SJson *json = sj_load(SAVE_FILE);
	SJson *arr, *object, *data;
	int i, j;

	if (!json) 
	{
		slog("gamestate_load cannot load empty json*");
		return;
	}

	data = sj_object_get_value(json, "food");
	sj_string_as_integer(data->v.string, &gamestate.food);

	arr = sj_object_get_value(json, "crew");
	for (i = 0; i < MAX_CREW; i++)
	{								  
		object = sj_array_get_nth(arr, i);

		data = sj_object_get_value(object, "name");
		//strncpy(gamestate.crew[i].name, sj_get_string_value(data), len); // this needs to be implemented to free(json) 
		gamestate.crew[i].name = sj_get_string_value(data);

		data = sj_object_get_value(object, "title");
		//strncpy(gamestate.crew[i].title, sj_get_string_value(data), WORD_LEN); // this needs to be implemented to free(json)
		gamestate.crew[i].title = sj_get_string_value(data);

		data = sj_object_get_value(object, "clearance");
		sj_string_as_integer(data->v.string, &gamestate.crew[i].clearance);

		data = sj_object_get_value(object, "hunger");
		sj_string_as_integer(data->v.string, &gamestate.crew[i].hunger);

		data = sj_object_get_value(object, "morale");
		sj_string_as_integer(data->v.string, &gamestate.crew[i].morale);

		data = sj_object_get_value(object, "is_alive");
		sj_string_as_integer(data->v.string, &gamestate.crew[i].is_alive);

		data = sj_object_get_value(object, "_inuse");
		sj_string_as_integer(data->v.string, &gamestate.crew[i]._inuse);
	}

	//sj_free(json);	// Names don't display correctly when used
	//sj_free(arr);		// Names and Titles don't display correctly when used
	//sj_free(object);	// Messes up final crew member data
	sj_free(data);
	
	return;
}	

void gamestate_save(char* filename) 
{
	slog("SAVE ATTEMPT");

	int i;
	SJson *json = sj_object_new(), 
		*arr = sj_array_new(),
		*object,
		*data;

	for (i = 0; i < MAX_CREW; i++)
	{
		object = sj_object_new();

		data = sj_new_str(gamestate.crew[i].name);
		sj_object_insert(object, "name", data);

		data = sj_new_str(gamestate.crew[i].title);
		sj_object_insert(object, "title", data);

		data = sj_new_int((int)gamestate.crew[i].clearance);
		sj_object_insert(object, "clearance", data);

		data = sj_new_int(gamestate.crew[i].hunger);
		sj_object_insert(object, "hunger", data);

		data = sj_new_int(gamestate.crew[i].morale);
		sj_object_insert(object, "morale", data);

		data = sj_new_int(gamestate.crew[i].is_alive);
		sj_object_insert(object, "is_alive", data);

		data = sj_new_int(gamestate.crew[i]._inuse);
		sj_object_insert(object, "_inuse", data);
		
		sj_array_append(arr, object);
	}	
	sj_object_insert(json, "crew", arr);
	sj_object_insert(json, "food", sj_new_int(gamestate.food));

	sj_save(json, filename);

	sj_object_free(json);
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
