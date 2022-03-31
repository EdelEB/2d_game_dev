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

void menu_free(Menu* m)
{
	if (!m)
	{
		slog("menu_free cannot free NULL Menu*");
		return;
	}

	int i;
	for (i = 0; i < MAX_MENU_SPRITES; i++)
	{
		if(m->sprite_list[i]._inuse){
			gf2d_sprite_free(m->sprite_list[i].sprite);
		}
	}
	
	memset(m, 0, sizeof(Menu));

	// FOR WHEN I START USING POINTERS
	/*for (i = 0; i < MAX_MENU_BUTTONS; i++)
	{
		free(m->button_list[i]);
	}
	for (i = 0; i < MAX_MENU_LABELS; i++)
	{
		free(m->label_list[i]);
	}*/
}

//
//Menu* menu_new();
//
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
//			sj_get_string_value(sj_object_get_value(comp, "text")),
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
//			sj_get_string_value(sj_object_get_value(comp, "text")),
//			sj_get_string_value(sj_object_get_value(comp, "func"))
//		);
//	}
//
//
//	return ret;
//}




