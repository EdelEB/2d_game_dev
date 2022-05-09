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
	gamestate_id	id;

	ui_image*		background;
	Menu*			editor_menu;
	Menu*			working_menu;
	Editable		editable_list[MAX_MENU_OBJECTS]; /**< this is an array of all the ui_label's present in the menu*/
} MenuEditor;

void menu_editor_init();

void menu_editor_save_menu();

void menu_editor_new_menu();

void menu_editor_render();

gamestate_id menu_editor_listen(Uint32 mouse_state, int mx, int my, Uint8* keys);

void menu_editor_save_menu(void);

menu_editor_set_background(void);



Editable* editable_new();

void editable_create_label();

void editable_create_button();

void editable_create_image();

void editable_create_text_input();

void editable_listen(Editable* e, Uint32 mouse_state, int mx, int my);

void editable_render(Editable* e);

void editable_free(Editable* e);

void prompt_save(void);
void prompt_label(void);
void prompt_button(void);
void prompt_image(void);
void prompt_background(void);


#endif
