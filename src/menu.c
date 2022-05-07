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

SJson* menu_to_json(Menu* menu)
{
	int i;
	SJson* ret = sj_object_new();
	SJson* arr, *data;

	if (!menu) return;
	if (!ret) slog("ret received NULL SJson* in menu_to_json");  return NULL;

	data = sj_new_int(menu->id);
	if(data) sj_object_insert(ret, "id", data);
	
	data = sj_new_str(menu->title);
	if (data) sj_object_insert(ret, "title", data);
	//else sj_object_insert(ret, "title", sj_new_str("no_name"));

	arr = sj_array_new();
	if (!arr) slog("arr received NULL SJson* in menu_to_json"); return NULL;

	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (!menu->object_list[i]) continue;
		
		data = ui_object_to_json(menu->object_list[i]);
		if (data) sj_object_insert(data, "index", sj_new_int(i)); //TODO: NULL check
		if(data) sj_array_append(arr, data);
	}

	sj_object_insert(ret, "objects", arr);

	return ret;
}

Menu* menu_from_json(SJson* json)
{
	Menu* menu = menu_new();
	ui_object* object;
	SJson *arr, *data;
	char str[128];
	int i;

	if (!json)slog("NULL SJson* passed to menu_from_json()"); return NULL;
	if (!menu) slog("menu received NULL Menu* in menu_from_json()"); return NULL;

	data = sj_object_get_value(json, "id");
	if (data) sj_get_integer_value(data, &menu->id);

	data = sj_object_get_value(json, "title");
	if (data) {
		sprintf(str, "%s", sj_get_string_value(data));
		menu->title = str;
	}

	arr = sj_object_get_value(json, "objects");
	if (!arr) slog("Tried convert empty Menu to json"); return NULL;

	for (i = 0; i < MAX_MENU_OBJECTS && i < sj_array_get_count(arr); i++) 
	{
		data = sj_array_get_nth(arr, i);
		if (data) object = ui_object_from_json(data);
		if(object) menu->object_list[object->index] = object;
	}

	return menu;
}