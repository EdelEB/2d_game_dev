#include "menu_start.h"

gamestate_id test_text_input(void)
{
	Menu* menu = menu_get_by_id(MENU_START);
	ui_text_input* t = menu->text_input_list[0];

	slog("%s", t->str);
	return NONE;
}

Menu* menu_start_init()
{
	Menu* menu_start = menu_new();
	menu_start->id = MENU_START;

	menu_start->label_list[0] = ui_create_title_label(
		"The Martian Trail",
		WINDOW_WIDTH>>2,
		25
	);
	// TODO : Use a better Title Label
	menu_start->label_list[0]->sprite = ui_create_sprite(
		gf2d_sprite_load_image("assets/images/ui/button_green.png"),
		vector2d(menu_start->label_list[0]->render_rect.x, menu_start->label_list[0]->render_rect.y),
		vector2d(1.2, 0.5),
		vector2d(50,100),
		vector3d(0, 0, 0),
		1
	);

	menu_start->button_list[0] = ui_create_button(
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1) - 20,
		200,
		40,
		"New Game",
		start_new_game
	);
	menu_start->button_list[0]->sprite_default =
		ui_create_sprite(
			gf2d_sprite_load_image("assets/images/ui/button_green.png"),
			vector2d(menu_start->button_list[0]->click_box.x, menu_start->button_list[0]->click_box.y),
			vector2d(0.30, 0.25),
			vector2d(1, 50),
			vector3d(0,0,0),
			1
		);
	menu_start->button_list[0]->sprite_hover =
		ui_create_sprite(
			gf2d_sprite_load_image("assets/images/ui/button_purple.png"),
			vector2d(menu_start->button_list[0]->click_box.x, menu_start->button_list[0]->click_box.y),
			vector2d(0.30, 0.25),
			vector2d(1, 50),
			vector3d(0, 0, 0),
			1
		);
	menu_start->button_list[0]->sprite_pressed =
		ui_create_sprite(
			gf2d_sprite_load_image("assets/images/ui/button_blue.png"),
			vector2d(menu_start->button_list[0]->click_box.x, menu_start->button_list[0]->click_box.y),
			vector2d(0.20, 0.17),
			vector2d(1, 62),
			vector3d(0, 0, 0),
			1
		);
	menu_start->button_list[0]->hide_click_box = 1;



	menu_start->button_list[1] = ui_create_button(
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 1) + 60,
		200,
		40,
		"Load Game",
		load_game
	);



	menu_start->text_input_list[0] = ui_create_text_input(
		vector2d(200, 200),
		test_text_input
	);


	return menu_start;
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

