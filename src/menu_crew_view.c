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
		(WINDOW_WIDTH >> 1) - 300,
		WINDOW_HEIGHT >> 4
	);

	for (i = 0, j=1; i < MAX_CREW && j < MAX_MENU_LABELS-4 ; i++, j+=5)
	{
		cm = &current_crew[i];
		x_offset = (WINDOW_WIDTH >> 3) + 190 * (i);
		y_offset = (WINDOW_HEIGHT >> 3);

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

		sprintf(str, "Hunger: %i", cm->hunger);
		crew_view.label_list[j + 2] = ui_create_text_label(
			str,
			x_offset,
			y_offset<<2
		);

		sprintf(str, "Morale: %i", cm->morale);
		crew_view.label_list[j + 3] = ui_create_text_label(
			str,
			x_offset,
			y_offset*5
		);
		
		if (cm->is_alive) { sprintf(str, "Alive"); }
		else { sprintf(str, "Dead"); }
		crew_view.label_list[j + 4] = ui_create_text_label(
			str,
			x_offset,
			y_offset*6
		);
	}
	return &crew_view;
}

void feed_crew(void) {}