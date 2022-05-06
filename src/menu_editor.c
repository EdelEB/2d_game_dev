#include "menu_editor.h"

void menu_editor_init()
{

	menu_editor.menu = menu_new();

}

Editable* editable_new()
{
	int i;
	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (menu_editor.editable_list[i]._inuse) continue;
		menu_editor.editable_list[i]._inuse = 1;
		return &menu_editor.editable_list[i];
	}
	return NULL;
}

gamestate_id menu_editor_listen(Uint32 mouse_state, int mx, int my, Uint8* keys)
{
	int i;

	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (!menu_editor.editable_list[i]._inuse) continue;
		editable_listen(&menu_editor.editable_list[i], mouse_state, mx, my, keys);
	}

	return menu_listen(menu_editor.menu, mouse_state, &mx, &my, keys);
}

void editable_create_label( char* str, Vector2D position, ui_label_type type)
{
	Editable* e = editable_new();

	if (!e) return;

	e->object = ui_create_label(str, position.x, position.y, type);
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = ui_create_sizable(position, vector2d(100,100))->sizable;
}

void editable_create_button(char* str, Vector2D position, Vector2D size)
{
	Editable* e = editable_new();

	if (!e) return;

	e->object = ui_create_button(position.x, position.y, size.x, size.y, str, NULL);
	e->draggable = ui_create_draggable(position, vector2d(size.x / 4, size.y / 4))->draggable;
	e->sizable = ui_create_sizable(position, size)->sizable;
}

void editable_create_image(char* filename, Vector2D position) 
{
	Editable* e = editable_new();

	if (!e) return;

	e->object = ui_create_image(filename, position, vector2d(0,0), vector2d(0,0), vector3d(0,0,0));
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = ui_create_sizable(position, vector2d(200,200))->sizable;
}

void editable_create_text_input(char* str, Vector2D position, ui_label_type type)
{
	Editable* e = editable_new();

	if (!e) return;

	e->object = ui_create_text_input(position, NULL);
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = NULL;// ui_create_sizable(position, vector2d())->sizable;
}


void editable_listen(Editable* e)
{

}