#include "menu.h"

gamestate_id menu_listen(Menu* m)
{
	if (!m)
	{
		slog("menu_listen cannot listen with NULL Menu*");
		return NONE;
	}

	gamestate_id id;
	int i;

	for (i = 0; i < MAX_MENU_BUTTONS; i++)
	{
		if (!m->button_list[i]._inuse) continue;
		id = ui_button_listen_alone(&m->button_list[i]);
		if (id) return id;
	}
	return NONE;
}

void menu_draw(Menu* m)
{
	if(!m)
	{
		slog("menu_draw cannot draw NULL Menu*");
		return;
	}

	int i;
	for (i = 0; i < MAX_MENU_BUTTONS; i++)
	{
		if (!m->button_list[i]._inuse) continue;
		ui_button_render(&m->button_list[i]);
	}
	for (i = 0; i < MAX_MENU_LABELS; i++)
	{
		if (!m->label_list[i]._inuse) continue;
		ui_label_render(&m->label_list[i]);
	}
}




