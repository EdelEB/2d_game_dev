#include "crew.h"

#define TOTAL_CREW_MEMBERS 20
#define MAX_CREW 5
crew_member* current_crew[MAX_CREW];

crew_member* crew_options[TOTAL_CREW_MEMBERS];

void crew_mental_breakdown(crew_member* cm)
{
	if (!cm)
	{
		slog("crew_mental_breakdown called on NULL crew_member*");
		return;
	}
	return NONE;
}

gamestate_id crew_lower_morale()
{
	int i;
	Uint8 is_breakdown = 0;

	for (i = 0; i < MAX_CREW; i++)
	{
		if (current_crew[i] && current_crew[i]->is_alive)
		{
			if (current_crew[i]->morale != 0) { current_crew[i]->morale--; }
			if (current_crew[i]->morale == 0) { is_breakdown = 1; }
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
		if (current_crew[i] && current_crew[i]->is_alive)
		{
			if (current_crew[i]->morale != 10) { current_crew[i]->morale++; }
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
		if (current_crew[i] && current_crew[i]->is_alive)
		{
			if (current_crew[i]->hunger != 0) { current_crew[i]->hunger--; }
			if (current_crew[i]->hunger == 0) { is_starving = 1; }
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
		if (current_crew[i] && current_crew[i]->is_alive)
		{
			if (current_crew[i]->hunger != 10) { current_crew[i]->hunger++; }
		}
	}
	return NONE;
}