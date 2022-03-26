#include "menu_crew_select.h"

Menu crew_select = { 0 };
Uint32 crew_options_index = 0;  // current index in crew_options
Uint16 crew_additions = 0;


void update_name_and_title(void)
{
	crew_select.label_list[1] = ui_create_header_label(
		crew_options[crew_options_index].name,
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1)
	);
	crew_select.label_list[2] = ui_create_header_label(
		crew_options[crew_options_index].title,
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1) + 50
	);
}

Menu* menu_crew_select_init(void)
{
	crew_select.id = CREW_SELECT;
	char* crew_member_display_string;

	crew_select.button_list[0] = ui_create_button(
		WINDOW_WIDTH-200,
		(WINDOW_HEIGHT >> 3) * 7,
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
		(WINDOW_WIDTH >> 1) - 300,
		(WINDOW_HEIGHT >> 4)
	);

	update_name_and_title();

	return &crew_select;
}

gamestate_id next_crew_member(void)
{
	slog("NEXT CREW MEMBER");
	
	crew_options_index++;
	
	for (crew_options_index; crew_options_index <= MAX_CREW_OPTIONS; crew_options_index++)
	{
		if (crew_options_index == MAX_CREW_OPTIONS) { crew_options_index = 0; }
		if (crew_options[crew_options_index]._inuse) break;
	}
	update_name_and_title();

	return NONE;
}

gamestate_id select_crew_member(void)
{
	slog("CREW MEMBER SELECTED");
	slog(crew_options[crew_options_index].name);

	gamestate.crew[crew_additions] = crew_options[crew_options_index];
	crew_options[crew_options_index]._inuse = 0;
	crew_additions++;

	if (crew_additions == MAX_CREW)	
	{ 
		menu_crew_view_init();
		return CREW_VIEW; 
	}

	next_crew_member();

	return NONE;
}