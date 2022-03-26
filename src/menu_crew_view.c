#include "menu_crew_view.h"

Menu crew_view = { 0 };

Menu* menu_crew_view_init(void)
{
	int i, j, x_offset;
	crew_member* cm;
	char* alive;

	crew_view.id = CREW_VIEW;

	crew_view.label_list[0] = ui_create_title_label(
		"Crew View",
		(WINDOW_WIDTH >> 1) - 300,
		WINDOW_HEIGHT >> 4
	);

	for (i = 0, j=1; i < MAX_CREW && j < MAX_MENU_LABELS-2 ; i++, j+=3)
	{
		cm = &current_crew[i];
		x_offset = (WINDOW_WIDTH >> 3) + 200 * (i);

		crew_view.label_list[j] = ui_create_header_label(
			cm->name,
			x_offset,
			WINDOW_HEIGHT >> 2
		);

		crew_view.label_list[j + 1] = ui_create_text_label(
			cm->title,
			x_offset,
			WINDOW_HEIGHT >> 1
		);

		if (cm->is_alive) { alive = "Alive"; }
		else { alive = "Dead"; }
		crew_view.label_list[j + 2] = ui_create_text_label(
			alive,
			x_offset,
			(WINDOW_HEIGHT >> 1) + (WINDOW_HEIGHT >>2)
		);
	}
	return &crew_view;
}

void feed_crew(void) {}