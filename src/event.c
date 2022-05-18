#include "event.h"

typedef struct{
	Uint32		max_events;
	Event	   *event_list;
}EventManager;

EventManager event_manager = { 0 };

void event_manager_init(Uint32 max_events)
{
	if (max_events == 0)
	{
		slog("Cannot allocate memory for zero events. Stop wasting my time");
		return;
	}
	if (event_manager.event_list)
	{
		slog("event_manager already initialized");
	}
	event_manager.max_events = max_events;
	event_manager.event_list = gfc_allocate_array(sizeof(Event), max_events);

	atexit(event_manager_close);
	slog("event manager initialized");
}

void event_manager_close(void)
{
	int i;
	for (i = 0; i < event_manager.max_events; i++)
	{
		event_free(&event_manager.event_list[i]);
	}
	if (event_manager.event_list) {
		free(event_manager.event_list);
	}
}

void event_manager_save_all(char* filename)
{
	int i;
	SJson* json = sj_object_new();
	SJson* arr = sj_array_new();
	SJson* data;

	if (!json || !arr) return;

	for (i = 0; i < event_manager.max_events; i++)
	{
		if (!event_manager.event_list[i]._inuse) continue;
		
		data = event_to_json(&event_manager.event_list[i]);
		if (data) sj_array_append(arr, data);
	}

	sj_object_insert(json, "Events", arr);
	sj_save(json, filename);

	sj_free(arr);
	sj_free(json);
}


Event* event_new(void)
{
	int i;
	for (i = 0; i < event_manager.max_events; i++)
	{
		if (event_manager.event_list[i]._inuse) continue;
		event_manager.event_list[i]._inuse = 1;
		return &event_manager.event_list[i];
	}
	return NULL;
}

void event_free(Event* e)
{
	if (!e)	{ return; }
	else { memset(e, 0, sizeof(Event)); }
}

SJson* event_to_json(Event* e)
{
	int i;
	SJson* ret = sj_object_new();
	SJson* arr, * data, *option;

	if (!e) return;
	if (!ret) return;

	data = sj_new_int(e->id);
	if (data) sj_object_insert(ret, "id", data);

	data = sj_new_str(e->title);
	if (data) sj_object_insert(ret, "title", data);

	data = sj_new_str(e->prompt);
	if (data) sj_object_insert(ret, "prompt", data);

	arr = sj_array_new();
	if (!arr) return;

	for (i = 0; i < MAX_OPTIONS; i++)
	{
		if (!e->options[i]._inuse) continue;

		option = sj_object_new();
		if (!option) return;
			
		data = sj_new_int(e->options[i].clearance);
		if (data) sj_object_insert(option, "clearance", data);

		data = sj_new_str(e->options[i].text);
		if (data) sj_object_insert(option, "text", data);

		data = sj_new_int(e->options[i].clicked);
		if (data) sj_object_insert(option, "simple_nav", data);

		sj_array_append(arr, option);
	}

	sj_object_insert(ret, "options", arr);

	return ret;
}

Menu* event_menu_from_json(SJson* json)
{
	Menu* menu = menu_new();
	SJson* arr, * data, * option;
	char str[128];
	int i, temp;

	if (!json) { slog("NULL SJson* passed to event_menu_from_json()"); return NULL; }
	if (!menu) { slog("menu received NULL Menu* in event_menu_from_json()"); return NULL; }

	data = sj_object_get_value(json, "id");
	if (data) sj_get_integer_value(data, &menu->id);

	data = sj_object_get_value(json, "title");
	if (data) {
		sprintf(str, "%s", sj_get_string_value(data));
		menu->title = str;
	}
	menu->object_list[0] = ui_create_label(str, WINDOW_WIDTH >> 3, WINDOW_HEIGHT >> 4, TITLE);

	data = sj_object_get_value(json, "prompt");
	sprintf(str, "%s", sj_get_string_value(data));
	menu->object_list[1] = ui_create_label(str, WINDOW_WIDTH >> 4, WINDOW_HEIGHT >> 2, HEADER);

	arr = sj_object_get_value(json, "options");
	if (!arr) { slog("Tried convert empty Event to Menu"); return NULL; }

	for (i = 0; i < sj_array_get_count(arr); i++)
	{
		option = sj_array_get_nth(arr, i);
		if (!option) continue;

		data = sj_object_get_value(option, "clearance");
		sj_get_integer_value(data, &temp);
		if (!crew_check_clearance(temp)) 
		{
			sprintf(str, "<Option Locked : crew member with clearance %d required>", temp);
			menu->object_list[i + 2] = ui_create_button(
				(WINDOW_WIDTH >> 4) - 20,
				(WINDOW_HEIGHT >> 3) * (i + 3) - 30,
				(WINDOW_WIDTH >> 3) * 7,
				(WINDOW_HEIGHT >> 3),
				str,
				NULL
			);
			continue;
		}

		data = sj_object_get_value(option, "text");
		if (data) sprintf(str, "%s", sj_get_string_value(data));

		menu->object_list[i + 2] = ui_create_button(
			(WINDOW_WIDTH >> 4) - 20 ,
			(WINDOW_HEIGHT >> 3) * (i + 3) - 30,
			(WINDOW_WIDTH >> 3) * 7,
			(WINDOW_HEIGHT >> 3),
			str,
			NULL
		);

		data = sj_object_get_value(option, "simple_nav");
		sj_get_integer_value(data, &menu->object_list[i + 2]->button->simple_nav);

	}

	sj_free(json);

	return menu;
}

void event_menu_load_all(char *filename)
{
	int i;
	SJson *json, *arr, *object, *data;

	json = sj_load(filename);
	
	if (!json) {slog("sj_load failed in event_menu_load_all()"); return;}
	
	arr = sj_object_get_value(json, "Events");
	if (!arr) return;

	int size = sj_array_get_count(arr);
	for (i = 0; i < size; i++)
	{
		object = sj_array_get_nth(arr, i);
		if (!object) continue;

		event_menu_from_json(object);
	}
}