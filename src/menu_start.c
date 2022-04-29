#include "menu_start.h"

Menu menu_start = { 0 };

Menu* menu_start_init()
{
	menu_start.id = MENU_START;

	menu_start.label_list[0] = ui_create_title_label(
		"The Martian Trail",
		25,
		25
	);

	menu_start.button_list[0] = ui_create_button(
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1) - 20,
		200,
		40,
		"New Game",
		start_new_game
	);

	menu_start.button_list[1] = ui_create_button(
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1) + 60,
		200,
		40,
		"Load Game",
		load_game
	);

	return &menu_start;
}

gamestate_id start_new_game(void)
{
	gamestate_new();
	return CREW_SELECT;
}

gamestate_id load_game(void)
{
	slog("load clicked");
	gamestate_load(SAVE_FILE);
	menu_crew_view_init();
	return CREW_VIEW;
}

