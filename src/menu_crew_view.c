#include "menu_crew_view.h"

Menu* menu_crew_view_init(void)
{
	Menu* menu = menu_new();
	int i, j, x_offset, y_offset;
	crew_member* cm;
	char str[16];

	menu->id = CREW_VIEW;

	menu->object_list[0] = ui_create_label(
		"Crew View",
		(WINDOW_WIDTH >> 1) - 250,
		WINDOW_HEIGHT >> 4,
		TITLE
	);

	sprintf(str, "Food: %d", gamestate.food);
	menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_label(
		str,
		25,
		75,
		TEXT
	);

	menu->object_list[MAX_MENU_OBJECTS - 2] = ui_create_button(
		WINDOW_WIDTH - 100,
		75,
		75,
		40,
		"Map",
		crew_view_to_map
	);

	for (i = 0; i < MAX_CREW ; i++ )
	{
		cm = &gamestate.crew[i];
		x_offset = (WINDOW_WIDTH >> 3) + 190 * (i);
		y_offset = (WINDOW_HEIGHT >> 3);
		j = 6 * i + 1; 
		if(j < MAX_MENU_OBJECTS - 4)
		{
			menu->object_list[j] = ui_create_label(
				cm->name,
				x_offset,
				y_offset << 1,
				HEADER
			);

			menu->object_list[j + 1] = ui_create_label(
				cm->title,
				x_offset,
				y_offset * 3,
				TEXT
			);

			sprintf(str, "Hunger: %d", cm->hunger);
			menu->object_list[j + 2] = ui_create_label(
				str,
				(WINDOW_WIDTH >> 3) + 190 * (i),
				(WINDOW_HEIGHT >> 3) << 2,
				TEXT
			);

			sprintf(str, "Morale: %d", cm->morale);
			menu->object_list[j + 3] = ui_create_label(
				str,
				x_offset,
				y_offset * 5,
				TEXT
			);

			if (cm->is_alive) { sprintf(str, "Alive"); }
			else { sprintf(str, "Dead"); }
			menu->object_list[j + 4] = ui_create_label(
				str,
				x_offset,
				y_offset * 6,
				TEXT
			);

			menu->object_list[j + 5] = ui_create_button(
				x_offset,
				y_offset * 7,
				x_offset - 190 * i,
				y_offset - 20,
				"Feed",
				get_feed_crew_member_func(i)
			);
		}
	}

	return &menu;
}

gamestate_id crew_view_to_map(void)
{
	return MAP;
}

void crew_view_update()
{
	Uint8 i;
	for (i = 0; i < MAX_CREW; i++)
	{
		crew_view_update_member_hunger(i);
		crew_view_update_member_morale(i);
		crew_view_update_member_alive(i);
	}
}

void crew_view_update_member_hunger(int i)
{
	char str[16];
	crew_member* cm = &gamestate.crew[i];
	Menu* menu = menu_get_by_id(CREW_VIEW);

	sprintf(str, "Hunger: %d", cm->hunger);
	ui_label_update(menu->object_list[6 * i + 3]->l, str);

	sprintf(str, "Food: %d", gamestate.food);
	ui_label_update(menu->object_list[MAX_MENU_OBJECTS - 1]->l, str);
}


void crew_view_update_member_morale(int i)
{
	char str[16];

	sprintf(str, "Morale: %d", gamestate.crew[i].morale);
	ui_label_update(
		menu_get_by_id(CREW_VIEW)->object_list[6 * i + 4]->l,
		str
	);
}

void crew_view_update_member_alive(int i)
{
	char str[16];

	if (gamestate.crew[i].is_alive) { sprintf(str, "Alive"); }
	else { sprintf(str, "Dead"); }
	ui_label_update(
		menu_get_by_id(CREW_VIEW)->object_list[6*i + 5]->l,
		str
	);
}

void feed_crew_member(int i) 
{
	slog("FEEDING CREW MEMBER");
	if (gamestate.food < 1)
	{
		slog("feed_crew_member cannot feed crew with no food");
		return NONE;
	}
	else if (!gamestate.crew[i].is_alive)
	{
		slog("feed_crew_member cannot feed dead crew member");
		return NONE;
	}

	gamestate.food--;
	gamestate.crew[i].hunger++;
	crew_view_update_member_hunger(i);
	
	return NONE;
}

void fcm0(void) {
	feed_crew_member(0);
}
void fcm1(void) {
	feed_crew_member(1);
}
void fcm2(void) {
	feed_crew_member(2);
}
void fcm3(void) {
	feed_crew_member(3);
}
void fcm4(void) {
	feed_crew_member(4);
}
void* get_feed_crew_member_func(int i) {
	switch (i)
	{
	case 0: return fcm0;
	case 1: return fcm1;
	case 2: return fcm2;
	case 3: return fcm3;
	case 4: return fcm4;
	}
}