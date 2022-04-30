#ifndef __MENU_H__
#define __MENU_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_json.h"

#include "gamestate.h"
#include "ui_stuff.h"
#include "menu_crew_select.h"

#define MAX_MENU_SPRITES 10
#define MAX_MENU_BUTTONS 10
#define MAX_MENU_LABELS 40
#define MAX_MENUS 10

typedef struct MENU {
	Uint8		   _inuse;
	gamestate_id	id;

	Sprite*			sprite_list[MAX_MENU_SPRITES];	/**< this is an array of all the sprite's present in the menu*/
	ui_label		label_list[MAX_MENU_LABELS];	/**< this is an array of all the ui_label's present in the menu*/
	ui_button		button_list[MAX_MENU_BUTTONS];	/**< this is an array of all the ui_button's present in the menu*/
	ui_draggable	draggable_list[MAX_MENU_BUTTONS];
}Menu;

gamestate_id menu_listen(Menu *m, Uint8 mouse_state, int* mx, int* my);

void menu_manager_init(Uint32 max_menus);
void menu_manager_close(void);
Menu* menu_new(void);
Menu* menu_get_by_id(gamestate_id id);

void menu_draw(Menu *m);

void menu_free(Menu *m);

void menu_load(char *filename);

#endif