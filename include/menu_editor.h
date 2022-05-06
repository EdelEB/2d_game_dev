#ifndef __MENU_EDITOR_H__
#define __MENU_EDITOR_H__

#include <stdio.h>
#include <direct.h>

#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_json.h"

#include "gamestate.h"
#include "ui_stuff.h"
#include "menu.h"

typedef struct EDITABLE {
	Uint8			_inuse;
	
	ui_object*		object;		/**< object that will be saved to the menu*/
	ui_draggable*	draggable;	/**< allows object to be moved*/
	ui_sizable*		sizable;	/**< allws object size to be modified*/
}Editable;

typedef struct MENU_EDITOR{
	Uint8			_inuse;
	gamestate_id	id;

	Menu*			menu;	
	Editable		editable_list[MAX_MENU_OBJECTS]; /**< this is an array of all the ui_label's present in the menu*/
} MenuEditor;

MenuEditor menu_editor = {0};

void menu_editor_init();

void menu_editor_save_menu();

void menu_editor_new_menu(char* title);

void menu_editor_render();

gamestate_id menu_editor_listen(Uint32 mouse_state, int mx, int my);


Editable* editable_new();

void editable_create_label(char* str, Vector2D position, ui_label_type);

void editable_listen(Editable* e);

#endif
