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
	int i;

	if (!m)
	{
		slog("menu_free cannot free NULL Menu*");
		return;
	}

	for (i = 0; i < MAX_MENU_OBJECTS; i++) { ui_object_free(m->object_list[i]); }

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

	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (!m->object_list[i] || !m->object_list[i]->_inuse) continue;
	
		id = ui_object_listen(m->object_list[i], mouse_state, *mx, *my, keys);
		if (id) return id;
	}

	return NONE;
}

void menu_draw(Menu* m)
{
	int i;

	if(!m)
	{
		slog("menu_draw cannot draw NULL Menu*");
		return;
	}

	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (m->object_list[i] && m->object_list[i]->_inuse)
			ui_object_render(m->object_list[i]);
	}
}

void menu_save(Menu* m, char* filename)
{
	int i;
	SJson* data, * object;
	SJson* json = sj_object_new();
	
	if (!json) return;

	for ( i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (!m->object_list[i] || !m->object_list[i]->_inuse) break; 
		
		object = sj_object_new();
		if (!object) return;

		data = sj_new_int(m->object_list[i]->id);
		if (data) sj_object_insert(object, "id", data);

		data = ui_object_to_json(m->object_list[i]);
		if (data) sj_object_insert(object, "object", data);
		
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
//		menu->image_list[i] = ui_create_image(
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




