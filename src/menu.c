#include "menu.h"

typedef struct{
	Uint32 max_menus;	/**<max number of menus in game*/
	Menu* menu_list;	/**<array of all menus*/
}MenuManager;

MenuManager menu_manager = { 0 };

void menu_manager_init(Uint32 max_menus)
{
	if (max_menus == 0)
	{
		slog("Cannot allocate memory for zero menus. Stop wasting my time");
		return;
	}
	if (menu_manager.menu_list)
	{
		slog("Entity manager already initialized dumb dumb");
	}
	menu_manager.max_menus = max_menus;
	menu_manager.menu_list = gfc_allocate_array(sizeof(Menu), max_menus);

	atexit(menu_manager_close);
	slog("menu manager initialized");
}

void menu_manager_close() 
{
	int i;
	for (i = 0; i < menu_manager.max_menus; i++) { menu_free(&menu_manager.menu_list[i]); }

	memset(&menu_manager, 0, sizeof(Menu) * menu_manager.max_menus);
	slog("Menu Manager closed");
}

Menu* menu_new(void)
{
	int i;
	for (i = 0; i < menu_manager.max_menus; i++)
	{
		if (menu_manager.menu_list[i]._inuse) continue;

		menu_manager.menu_list[i]._inuse = 1;
		return &menu_manager.menu_list[i];
	}
	return NULL;  // you need a bigger max_menus and bigger menu_list; this shouldn't be a problem right now though
}

void menu_free(Menu* m)
{
	if (!m)
	{
		slog("menu_free cannot free NULL Menu*");
		return;
	}

	int i;

	for (i = 0; i < MAX_MENU_LABELS; i++) { ui_label_free(m->label_list[i]); }
	for (i = 0; i < MAX_MENU_BUTTONS; i++) { ui_button_free(m->button_list[i]); }
	for (i = 0; i < MAX_MENU_SPRITES; i++) { ui_sprite_free(m->sprite_list[i]); }
	for (i = 0; i < MAX_MENU_BUTTONS; i++) { ui_draggable_free(m->draggable_list[i]); }

	memset(m, 0, sizeof(Menu));
}

Menu* menu_get_by_id(gamestate_id id)
{
	int i;
	for (i = 0; i < menu_manager.max_menus; i++) 
	{
		if (menu_manager.menu_list[i]._inuse &&
			menu_manager.menu_list[i].id == id) {
			return &menu_manager.menu_list[i];
		}
	}

	slog("Error: menu_get_by_id() failed");
	return NULL;
}

gamestate_id menu_listen(Menu* m, Uint8 mouse_state, int *mx, int *my, Uint8* keys)
{
	gamestate_id id = 0;
	int i;
	
	if (!m)
	{
		slog("menu_listen cannot listen with NULL Menu*");
		return NONE;
	}

	for (i = 0; i < MAX_MENU_BUTTONS; i++)
	{
		if (m->button_list[i] && m->button_list[i]->_inuse)
			id = ui_button_listen(m->button_list[i], mouse_state, *mx, *my);
		if (id) return id;
	}
	for (i = 0; i < MAX_MENU_BUTTONS; i++)
	{
		if (m->draggable_list[i] && m->draggable_list[i]->_inuse)
			ui_draggable_listen(m->draggable_list[i], mouse_state, *mx, *my);
	}
	for (i = 0; i < MAX_MENU_TEXT_INPUTS; i++)
	{
		if (m->text_input_list[i] && m->text_input_list[i]->_inuse)
			id = ui_text_input_listen(m->text_input_list[i], mouse_state, *mx, *my, keys);
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
		if (m->button_list[i] && m->button_list[i]->_inuse)
			ui_button_render(m->button_list[i]);
	}
	for (i = 0; i < MAX_MENU_LABELS; i++)
	{
		if (m->label_list[i] && m->label_list[i]->_inuse)
			ui_label_render(m->label_list[i]);
	}
	for (i = 0; i < MAX_MENU_BUTTONS; i++)
	{
		if (m->draggable_list[i] && m->draggable_list[i]->_inuse)
			ui_draggable_render(m->draggable_list[i]);
	}
	for (i = 0; i < MAX_MENU_TEXT_INPUTS; i++)
	{
		if (m->text_input_list[i]) {
			ui_text_input_render(m->text_input_list[i]);
		}
	}
}



//Menu menu_load(char* filename)
//{
//	Menu ret;
//	Menu* menu = &ret;
//	SJson* file = sj_load(filename);
//	SJson* json, *comp;
//	SJson* arr = sj_array_new();
//	int i;
//	TTF_Font* font;
//
//	json = sj_object_get_value(file, "<menu name>");
//	arr = sj_object_get_value(json, "sprites");
//	for(i = 0; arr && i < sj_array_get_count(arr); i++)
//	{
//		comp = sj_array_get_nth(arr, i);
//		menu->sprite_list[i] = ui_create_sprite(
//			sj_get_string_value(sj_object_get_value(comp, "filename")),
//			vector2d(
//				sj_get_integer_value(sj_object_get_value(comp, "position_x"), NULL),
//				sj_get_integer_value(sj_object_get_value(comp, "position_y"), NULL)),
//			vector2d(
//				sj_get_integer_value(sj_object_get_value(comp, "scale_x"), NULL),
//				sj_get_integer_value(sj_object_get_value(comp, "scale_y"), NULL)),
//			vector2d(
//				sj_get_integer_value(sj_object_get_value(comp, "scale_center_x"), NULL),
//				sj_get_integer_value(sj_object_get_value(comp, "scale_center_y"), NULL)),
//			vector3d(
//				sj_get_integer_value(sj_object_get_value(comp, "rotation_x"), NULL),
//				sj_get_integer_value(sj_object_get_value(comp, "rotation_y"), NULL),
//				sj_get_integer_value(sj_object_get_value(comp, "rotation_z"), NULL)),
//			sj_get_integer_value(sj_object_get_value(comp, "frame_count"), NULL)
//		);
//	}
//
//	arr = sj_object_get_value(json, "labels");
//	for (i = 0; arr && i < sj_array_get_count(arr); i++) 
//	{
//		comp = sj_array_get_nth(arr, i);
//		switch ((int)sj_get_string_value(comp, "font_code")) {
//		case 0: font = font_info.text_font; break;
//		case 1: font = font_info.title_font; break;
//		case 3: font = font_info.header_font; break;
//		}
//		menu->label_list[i] = ui_create_label_helper(
//			sj_get_string_value(sj_object_get_value(comp, "text_label")),
//			sj_get_integer_value(sj_object_get_value(comp, "position_x"), NULL),
//			sj_get_integer_value(sj_object_get_value(comp, "position_y"), NULL),
//			font
//			);
//	}
//	
//	arr = sj_object_get_value(json, "buttons");
//	for (i = 0; arr && i < sj_array_get_count(arr); i++)
//	{
//		comp = sj_array_get_nth(arr, i);
//		
//		menu->button_list[i] = ui_create_button(
//			sj_get_integer_value(sj_object_get_value(comp, "position_x"), NULL),
//			sj_get_integer_value(sj_object_get_value(comp, "position_y"), NULL),
//			sj_get_integer_value(sj_object_get_value(comp, "dimensions_w"), NULL),
//			sj_get_integer_value(sj_object_get_value(comp, "dimensions_h"), NULL),
//			sj_get_string_value(sj_object_get_value(comp, "text_label")),
//			sj_get_string_value(sj_object_get_value(comp, "func"))
//		);
//	}
//
//
//	return ret;
//}




