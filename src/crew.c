#include "crew.h"

//gamestate_id crew_lower_morale()
//{
//	int i;
//	Uint8 is_breakdown = 0;
//
//	for (i = 0; i < MAX_CREW; i++)
//	{
//		if (gamestate.crew[i].is_alive)
//		{
//			if (gamestate.crew[i].morale != 0) { gamestate.crew[i].morale--; }
//			if (gamestate.crew[i].morale == 0) { is_breakdown = 1; }
//		}
//	}
//	if (is_breakdown) { return EVENT_CREW_BREAKDOWN; }
//
//	return NONE;
//}
//
//gamestate_id crew_raise_morale()
//{
//	int i;
//	for (i = 0; i < MAX_CREW; i++)
//	{
//		if (gamestate.crew[i] && gamestate.crew[i]->is_alive)
//		{
//			if (gamestate.crew[i]->morale != 10) { gamestate.crew[i]->morale++; }
//		}
//	}
//	return NONE;
//}
//
//gamestate_id crew_lower_hunger()
//{
//	int i;
//	Uint8 is_starving = 0;
//
//	for (i = 0; i < MAX_CREW; i++)
//	{
//		if (gamestate.crew[i] && gamestate.crew[i]->is_alive)
//		{
//			if (gamestate.crew[i]->hunger != 0) { gamestate.crew[i]->hunger--; }
//			if (gamestate.crew[i]->hunger == 0) { is_starving = 1; }
//		}
//	}
//	if (is_starving) { return EVENT_CREW_STARVING; }
//
//	return NONE;
//}
//
//gamestate_id crew_raise_hunger()
//{
//	int i;
//	for (i = 0; i < MAX_CREW; i++)
//	{
//		if (gamestate.crew[i] && gamestate.crew[i]->is_alive)
//		{
//			if (gamestate.crew[i]->hunger != 10) { gamestate.crew[i]->hunger++; }
//		}
//	}
//	return NONE;
//}

char* get_crew_title(event_clearance c)
{
	switch (c)
	{
			case PILOT			:
				return "Pilot";
			case DOCTOR			:
				return "Doctor";
			case ENGINEER		:
				return "Engineer";
			case PHYSICOLOGIST	:
				return "Phsycologist";
			case PROGRAMMER		:
				return "Programmer";
			case PAINTER		:
				return "Painter";
			case COMEDIAN		:
				return "Comedian";
			case MUSICIAN		:
				return "Musician";
			case NUTRITIONIST	:
				return "Nustritionist";
			default:
				return "DEFAULT";
	}
}

void code_vomit_create_crew_member_options(void)
{
	int i;
	char* crew_names[8] = {"John", "Ken", "Will", "Tom", "Rich", "Ralph", "Carlos", "Bart"};

	for (i = 0; i < 8; i++) 
	{
		crew_options[i]._inuse = 1;
		crew_options[i].is_alive = 1;
		crew_options[i].name = crew_names[i];
		crew_options[i].clearance = i+1;
		crew_options[i].hunger = 10;
		crew_options[i].morale = 10;
		crew_options[i].title = get_crew_title(i+1);
	}
}