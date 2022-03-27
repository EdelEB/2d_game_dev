#include "menu_crew_view.h"

Menu crew_view = { 0 };

Menu* menu_crew_view_init(void)
{
	int i, j, x_offset, y_offset;
	crew_member* cm;
	char str[16];

	crew_view.id = CREW_VIEW;

	crew_view.label_list[0] = ui_create_title_label(
		"Crew View",
		(WINDOW_WIDTH >> 1) - 250,
		WINDOW_HEIGHT >> 4
	);

	sprintf(str, "Food: %d", gamestate.food);
	crew_view.label_list[MAX_MENU_LABELS - 1] = ui_create_text_label(
		str,
		25,
		75
	);

	for (i = 0; i < MAX_CREW ; i++ )
	{
		cm = &gamestate.crew[i];
		x_offset = (WINDOW_WIDTH >> 3) + 190 * (i);
		y_offset = (WINDOW_HEIGHT >> 3);
		j = 5 * i + 1; 
		if(j < MAX_MENU_LABELS - 4)
		{
			crew_view.label_list[j] = ui_create_header_label(
				cm->name,
				x_offset,
				y_offset << 1
			);

			crew_view.label_list[j + 1] = ui_create_text_label(
				cm->title,
				x_offset,
				y_offset * 3
			);

			update_crew_member_hunger(i);

			sprintf(str, "Morale: %d", cm->morale);
			crew_view.label_list[j + 3] = ui_create_text_label(
				str,
				x_offset,
				y_offset * 5
			);

			if (cm->is_alive) { sprintf(str, "Alive"); }
			else { sprintf(str, "Dead"); }
			crew_view.label_list[j + 4] = ui_create_text_label(
				str,
				x_offset,
				y_offset * 6
			);

			crew_view.button_list[i] = ui_create_button(
				x_offset,
				y_offset * 7,
				x_offset - 190 * i,
				y_offset - 20,
				"Feed",
				get_feed_crew_member_func(i)
			);
		}
	}

	crew_view.button_list[i] = ui_create_button(
		WINDOW_WIDTH - 100,
		75,
		75,
		40,
		"Map",
		crew_view_to_map
	);

	return &crew_view;
}

gamestate_id crew_view_to_map(void)
{
	return MAP;
}

void update_crew_member_hunger(int i)
{
	int j;
	char str[16];
	crew_member* cm = &gamestate.crew[i];

	j = 5 * i + 1;
	sprintf(str, "Hunger: %i", cm->hunger);
	crew_view.label_list[j + 2] = ui_create_text_label(
		str,
		(WINDOW_WIDTH >> 3) + 190 * (i),
		(WINDOW_HEIGHT >> 3) << 2
	);

	sprintf(str, "Food: %i", gamestate.food);
	crew_view.label_list[MAX_MENU_LABELS - 1] = ui_create_text_label(
		str,
		25,
		75
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

	gamestate.food--;
	gamestate.crew[i].hunger++;
	update_crew_member_hunger(i);
	
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