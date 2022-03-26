#include "menu_crew_select.h"

Menu crew_select = { 0 };
Uint32 current_spot = 0;  // current index in crew_options

Menu* menu_crew_select_init(void)
{
	crew_select.id = CREW_SELECT;

	crew_select.button_list[0] = ui_create_button(
		WINDOW_WIDTH-100,
		WINDOW_HEIGHT-50, 
		100,
		50,
		"Next",
		next_crew_member);

	crew_select.button_list[1] = ui_create_button(
		(WINDOW_WIDTH >> 1) - 50,
		(WINDOW_HEIGHT >> 3) * 7,
		100,
		50,
		"Select",
		select_crew_member
	);

	crew_select.label_list[0] = ui_create_title_label(
		"Select Your Crew",
		(WINDOW_WIDTH >> 1) - 125,
		(WINDOW_HEIGHT >> 4)
	);

	return &crew_select;
}

void next_crew_member(void)
{
	slog("NEXT CREW MEMBER");
	
	current_spot++;
	if (!crew_options[current_spot]._inuse) 
	{
		current_spot = 0;
	}

	return;
}

void select_crew_member(void)
{
	slog("CREW MEMBER SELECTED");
	slog(crew_options[current_spot].name);

	return;
}