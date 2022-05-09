#include "menus_json.h"

void menus_json_load_all()
{
	menus_json_load_ship_view();
	menus_json_load_ship_select();
	menus_json_load_map_mars();
	menus_json_load_code_guess();
}

/* ship_view */

void menus_json_load_ship_view()
{
	SJson* json = sj_load("assets/json/menus/ship_view.json");
	Menu* menu;

	if (!json) { return; }
	
	menu = menu_from_json(json);
	menu->id = SHIP_VIEW;

	menu->object_list[18] = ui_create_button_standard(
		WINDOW_WIDTH - 120,
		WINDOW_HEIGHT - 120,
		"Map",
		NULL
	);
	menu->object_list[18]->button->simple_nav = MAP;

	for (int i = 14; i < 18; i++)
	{
		ui_button_set_images(
			menu->object_list[i]->button,
			"quest",
			vector2d(0.1,0.1),
			vector2d(0,0),
			vector3d(0,0,0)
		);
		menu->object_list[i]->button->hide_click_box = 1;
	}
	menu->object_list[14]->button->simple_nav = EVENT_FAINT_BEEPS;
	menu->object_list[15]->button->simple_nav = EVENT_RATIONS_MISSING;
	menu->object_list[16]->button->simple_nav = EVENT_MORALE_LOW;
	menu->object_list[17]->button->simple_nav = EVENT_AI_TAKEOVER;

}

/*ship_select*/

gamestate_id add_food_storage()
{
	Menu* menu = menu_get_by_id(SHIP_SELECT);

	if (!gamestate.room_1) { 
		gamestate.room_1 = FOOD_STORAGE; 
		menu->object_list[20] = ui_create_image(
			"assets/images/ship_view/icon_food.png",
			vector2d(179, 328),
			vector2d(0.088, 0.094),
			vector2d(1, 1),
			vector3d(0, 0, 0)
		);
		return NONE; 
	}
	else gamestate.room_2 = FOOD_STORAGE; return CREW_SELECT;
}

gamestate_id add_fuel_storage()
{
	Menu* menu = menu_get_by_id(SHIP_SELECT);

	if (!gamestate.room_1) { 
		gamestate.room_1 = FUEL_STORAGE; 
		menu->object_list[20] = ui_create_image(
			"assets/images/ship_view/icon_fuel.png",
			vector2d(179, 328),
			vector2d(0.088, 0.094),
			vector2d(1, 1),
			vector3d(0, 0, 0)
		);
		return NONE; 
	}
	else gamestate.room_2 = FUEL_STORAGE; return CREW_SELECT;
}

gamestate_id add_entertainment()
{
	Menu* menu = menu_get_by_id(SHIP_SELECT);

	if (!gamestate.room_1) { 
		gamestate.room_1 = ENTERTAINMENT; 
		menu->object_list[20] = ui_create_image(
			"assets/images/ship_view/icon_morale.png",
			vector2d(179, 328),
			vector2d(0.088, 0.094),
			vector2d(1, 1),
			vector3d(0, 0, 0)
		);
		return NONE; 
	}
	else gamestate.room_2 = ENTERTAINMENT; return CREW_SELECT;
}

void menus_json_load_ship_select()
{
	SJson* json = sj_load("assets/json/menus/ship_select.json");
	Menu* menu;

	if (!json) { return; }

	menu = menu_from_json(json);
	menu->id = SHIP_SELECT;

	ui_button_set_images(menu->object_list[17]->button,
		BUTTON_STANDARD_FILE,
		vector2d(0.4, 0.25),
		vector2d(50, 10),
		vector3d(0, 0, 0)
		);
	menu->object_list[17]->button->hide_click_box = 1;
	menu->object_list[17]->button->on_click = add_food_storage;

	ui_button_set_images(menu->object_list[18]->button,
		BUTTON_STANDARD_FILE,
		vector2d(0.4, 0.25),
		vector2d(50, 10),
		vector3d(0, 0, 0)
	);
	menu->object_list[18]->button->hide_click_box = 1;
	menu->object_list[18]->button->on_click = add_fuel_storage;

	ui_button_set_images(menu->object_list[19]->button,
		BUTTON_STANDARD_FILE,
		vector2d(0.5, 0.25),
		vector2d(50, 10),
		vector3d(0, 0, 0)
	);
	menu->object_list[19]->button->hide_click_box = 1;
	menu->object_list[19]->button->on_click = add_entertainment;

}


/* map_mars */

void menus_json_load_map_mars()
{
	SJson* json = sj_load("assets/json/menus/map_mars.json");
	Menu* menu;

	if (!json) { return; }

	menu = menu_from_json(json);
	menu->id = MAP_MARS;
}

/* Code Guess */

gamestate_id on_guess()
{
	Menu* menu = menu_get_by_id(FB_GUESS);

	if (!menu) return;

	if (strcmp(menu->object_list[0]->text_input->str, "12") == 0)
	{
		strcpy(menu->object_list[0]->text_input->str, " ");
		return FB_GUESS_SUCCESS;
	}
	strcpy(menu->object_list[0]->text_input->str, " ");
	return FB_GUESS_FAIL;
}

void menus_json_load_code_guess()
{
	SJson* json = sj_load("assets/json/menus/code_guess.json");
	Menu* menu;

	if (!json) { return; }

	menu = menu_from_json(json);
	menu->id = FB_GUESS;
	menu->object_list[0]->text_input->button_enter->on_click = on_guess;
}