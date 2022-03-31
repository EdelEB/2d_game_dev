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
	gamestate_id	id;

	ui_sprite		sprite_list[MAX_MENU_SPRITES];	/**< this is an array of all the ui_sprite's present in the menu*/
	ui_label		label_list[MAX_MENU_LABELS];	/**< this is an array of all the ui_label's present in the menu*/
	ui_button		button_list[MAX_MENU_BUTTONS];	/**< this is an array of all the ui_button's present in the menu*/
}Menu;

gamestate_id menu_listen(Menu *m);

void menu_draw(Menu *m);

void menu_free(Menu *m);

void menu_load(char *filename);

#endif