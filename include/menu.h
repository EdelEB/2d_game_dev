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

#define MAX_MENUS 256
#define MAX_MENU_OBJECTS 128

typedef struct MENU {
	Uint8		   _inuse;
	gamestate_id	id;
	char*			title;

	ui_object*		object_list[MAX_MENU_OBJECTS];
}Menu;

void menu_manager_init(Uint32 max_menus);
void menu_manager_close(void);
Menu* menu_new(void);

Menu* menu_get_by_id(gamestate_id id);

gamestate_id menu_listen(Menu* m, Uint8 mouse_state, int* mx, int* my, Uint8* keys);

void menu_draw(Menu *m);

void menu_free(Menu *m);

void menu_save(Menu* m, char* filename);

SJson* menu_to_json(Menu* menu);

Menu* menu_from_json(SJson* json);

void menu_load(char *filename);

#endif