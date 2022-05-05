#include "menu_crew_select.h"

Uint32 crew_options_index = 0;  // current index in crew_options
Uint16 crew_additions = 0;

Menu* menu_crew_select_init(void)
{
	Menu* menu = menu_new();
	if (!menu) { slog("Could not allocate Menu* for menu"); }

	menu->id = CREW_SELECT;
	char* crew_member_display_string;

	menu->object_list[0] = ui_create_button(
		WINDOW_WIDTH-200,
		(WINDOW_HEIGHT >> 3) * 7,
		100,
		50,
		"Next",
		next_crew_member);

	menu->object_list[1] = ui_create_button(
		(WINDOW_WIDTH >> 1) - 50,
		(WINDOW_HEIGHT >> 3) * 7,
		100,
		50,
		"Select",
		select_crew_member
	);

	menu->object_list[2] = ui_create_label(
		"Select Your Crew",
		(WINDOW_WIDTH >> 1) - 300,
		(WINDOW_HEIGHT >> 4),
		TITLE
	);

	menu->object_list[3] = ui_create_label(
		crew_options[crew_options_index].name,
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1),
		HEADER
	);
	menu->object_list[4] = ui_create_label(
		crew_options[crew_options_index].title,
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1) + 50,
		HEADER
	);

	return &menu;
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

	if (crew_additions >= MAX_CREW)	
	{ 
		menu_crew_view_init();
		return CREW_VIEW; 
	}

	next_crew_member();

	return NONE;
}

void update_name_and_title(void)
{
	Menu* menu = menu_get_by_id(CREW_SELECT);

	ui_label_update(
		menu->object_list[3]->label,
		crew_options[crew_options_index].name
	);
	ui_label_update(
		menu->object_list[4]->label,
		crew_options[crew_options_index].title
	);
}