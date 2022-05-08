#include "notification.h"

Uint32 rest = 0;
Uint32 REST_DELAY = 30;

typedef struct {
	Uint32			max_notes;
	TTF_Font	   *title_font, *text_font;
	SDL_Color		font_color;
	Notification   *note_list;
}NoteManager;

NoteManager note_manager = { 0 };

void note_manager_init(Uint32 max_notes)
{
	if (max_notes == 0)
	{
		slog("Cannot allocate memory for zero notes. Stop wasting my time");
		return;
	}
	note_manager.max_notes = max_notes;
	note_manager.note_list = gfc_allocate_array(sizeof(Notification), max_notes);

	atexit(note_manager_close);
	slog("note manager initialized");
}

void note_manager_close(void)
{
	slog("this should close note_manger but it doesn't");
}

Notification* note_new(void)
{
	Uint32 i;
	for (i = 0; i < note_manager.max_notes; i++)
	{
		if (note_manager.note_list[i]._inuse) continue;

		note_manager.note_list[i]._inuse = 1;
		return &note_manager.note_list[i];
	}
	return NULL;
}



SJson* note_to_json(Notification* n)
{
	SJson* json = sj_object_new();
	SJson* data;
	int i;
	char str[16];

	if (!n || !json ) return;

	data = sj_new_int(n->id);
	if(data) sj_object_insert(json, "id", data);

	data = sj_new_str(n->title);
	if (data) sj_object_insert(json, "title", data);

	for (i = 0; i < MAX_LINES; i++)
	{
		if (!n->line_strings[i]) continue;
		
		data = sj_new_str(n->line_strings[i]);
		sprintf(str, "%d", i);
		if (data->get_string == "") data = sj_new_str(" "); // Does not load properly if there are empty strings
		if (data) sj_object_insert(json, str, data);
	}

	return json;
}

void note_save_all(char* filename)
{
	int i;
	SJson* json = sj_object_new();
	SJson* arr = sj_array_new();
	SJson* note;

	if (!filename || !json || !arr) return;

	for (i = 0; i < note_manager.max_notes; i++)
	{
		if (!note_manager.note_list[i]._inuse) { continue; }

		note = note_to_json(&note_manager.note_list[i]);
		if (note) sj_array_append(arr, note);
	}

	sj_object_insert(json, "Notes", arr);

	sj_save(json, filename);
	sj_free(json);
}

Menu* note_menu_from_json(SJson* json)
{
	int i;
	char str1[128], str2[16];
	SJson* data;
	Menu* menu = menu_new();

	if (!json || !menu) return;

	data = sj_object_get_value(json, "id");
	if (data) sj_get_integer_value(data, &menu->id);

	data = sj_object_get_value(json, "title");
	if (data) {
		sprintf(str1, "%s", sj_get_string_value(data));
		menu->title = str1;
	}
	menu->object_list[0] = ui_create_label(str1, WINDOW_WIDTH >> 2, WINDOW_HEIGHT >> 4, TITLE);

	menu->object_list[1] = ui_create_button_standard(
		(WINDOW_WIDTH >> 1) - 100,
		(WINDOW_HEIGHT >> 3) * 6,
		"Next",
		NULL
	);
	menu->object_list[1]->button->simple_nav = MAP;

	for (i = 0; i < MAX_LINES; i++)
	{
		sprintf(str2, "%d", i);
		data = sj_object_get_value(json, str2);
		if (!data) continue;
		
		sprintf(str1, sj_get_string_value(data));
		menu->object_list[i + 2] = ui_create_label(str1, 100, 50 * i + 300, TEXT);
	}

	return menu;
}

void note_menu_load_all(char* filename)
{
	int i;
	SJson* json, * arr, * data;

	json = sj_load(filename);	
	if (!json) { slog("filename may be incorrect for note_menu_load_all"); return; }

	arr = sj_object_get_value(json, "Notes");
	if (!arr) return;

	for (i = 0; i < sj_array_get_count(arr); i++)
	{
		data = sj_array_get_nth(arr, i);
		if (data) note_menu_from_json(data);
	}

	sj_free(json);
}