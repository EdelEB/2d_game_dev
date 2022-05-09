
#include "map.h"

void map_init(void)
{
	Menu* menu = menu_new();
	
	if (!menu) return;

	menu->title = "Map";
	menu->id = MAP;

	menu_set_background(menu, "assets/images/backgrounds/map_space.png");
	menu->object_list[0] = ui_create_image(
		"assets/images/ship_view/ship.png",
		vector2d(0,0),
		vector2d(0.075,0.075),
		vector2d(0,0),
		vector3d(0,0,0)
	);

	menu->object_list[1] = ui_create_button_standard(
		20,
		20,
		"Crew View",
		map_to_crew_view		
	);

	menu->object_list[2] = ui_create_button_standard(
		WINDOW_WIDTH-120,
		WINDOW_HEIGHT-120,
		"Travel",
		map_travel
	);

	menu->object_list[3] = ui_create_button_standard(
		WINDOW_WIDTH - 320,
		WINDOW_HEIGHT - 120,
		"Ship View",
		NULL
	);
	menu->object_list[3]->button->simple_nav = SHIP_VIEW;

	map_set_position_by_spot(gamestate.map_spot);
}

void map_set_position_by_spot(Uint32 spot)
{
	Menu* menu = menu_get_by_id(MAP);
	if (!menu) return;

	gamestate_save(SAVE_FILE);

	switch (spot)
	{
		case 0:
			menu->object_list[0]->image->position.x = 20;
			menu->object_list[0]->image->position.y = 630;
			break;
		case 1:
			menu->object_list[0]->image->position.x = 304;
			menu->object_list[0]->image->position.y = 510;
			break;
		case 2:
			menu->object_list[0]->image->position.x = 545;
			menu->object_list[0]->image->position.y = 366;
			break;
		case 3:
			menu->object_list[0]->image->position.x = 764;
			menu->object_list[0]->image->position.y = 230;
			break;
		case 4:
			menu->object_list[0]->image->position.x = 954;
			menu->object_list[0]->image->position.y = 130;
			break;
	}
}

gamestate_id map_travel(void) 
{
	gamestate.map_spot++;
	crew_lower_hunger();
	crew_lower_morale();
	map_set_position_by_spot(gamestate.map_spot);
	
	switch (gamestate.map_spot)
	{
	case 0:
		break;
	case 1:
		return EVENT_RATIONS_MISSING;
	case 2:
		return EVENT_ASTEROIDS_AHEAD;
	case 3:
		return EVENT_RATIONS_LOW;
	case 4:
		return EVENT_MARS_INCOMING;
	}

	return NONE;
}

gamestate_id map_to_crew_view(void)
{
	crew_view_update();
	return CREW_VIEW;
}
