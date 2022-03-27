#include "crew.h"

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