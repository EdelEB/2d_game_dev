#include "menu_start.h"

Menu* menu_start_init()
{
	Menu* menu_start = menu_new();
	menu_start->id = MENU_START;

	menu_start->draggable_list[0] = ui_create_draggable(
		vector2d(100, 100),
		vector2d(100, 100)
	);

	menu_start->label_list[0] = ui_create_title_label(
		"The Martian Trail",
		25,
		25
	);

	menu_start->button_list[0] = ui_create_button(
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1) - 20,
		200,
		40,
		"New Game",
		start_new_game
	);
	/*menu_start->button_list[0].sprite_default =
		ui_create_sprite(
			gf2d_sprite_load_image("assets/images/ui/button_green"),
			vector2d(menu_start->button_list[0].click_box.x, menu_start->button_list[0].click_box.y),
			vector2d(1,1),
			vector2d(1, 1),
			vector3d(0,0,0),
			1
		);
	menu_start->button_list[0].sprite_current = &menu_start->button_list[0].sprite_default;

	menu_start->button_list[0].sprite_default =
		ui_create_sprite(
			gf2d_sprite_load_image("assets/images/ui/button_purple"),
			vector2d(menu_start->button_list[0].click_box.x, menu_start->button_list[0].click_box.y),
			vector2d(1, 1),
			vector2d(1, 1),
			vector3d(0, 0, 0),
			1
		);*/

	menu_start->button_list[1] = ui_create_button(
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

