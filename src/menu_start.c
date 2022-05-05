#include "menu_start.h"

Menu* menu_start_init()
{
	Menu* menu = menu_new();
	menu->id = MENU_START;

	menu->object_list[0] = ui_create_label(
		"The Martian Trail",
		WINDOW_WIDTH>>2,
		25,
		TITLE
	);
	// TODO : Use a better Title Label
	menu->object_list[0]->label->image = ui_create_image(
		"assets/images/ui/button_test_1.png",
		vector2d(
			menu->object_list[0]->label->render_rect.x, 
			menu->object_list[0]->label->render_rect.y
		),
		vector2d(1.2, 0.5),
		vector2d(50,100),
		vector3d(0, 0, 0)
	)->i;

	menu->object_list[1] = ui_create_button(
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1) - 20,
		200,
		40,
		"New Game",
		start_new_game
	);

	ui_button_set_images(
		menu->object_list[1]->b,
		"button_test", 
		vector2d(0.30, 0.25),
		vector2d(1, 50),
		vector3d(0, 0, 0)
	);

	menu->object_list[2] = ui_create_button(
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1) + 60,
		200,
		40,
		"Load Game",
		load_game
	);

	menu->object_list[3] = ui_create_draggable(
		vector2d(100, 100),
		vector2d(100, 100)
	);

	menu->object_list[4] = ui_create_text_input(
		vector2d(200, 100),
		start_new_game
	);

	return menu;
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

