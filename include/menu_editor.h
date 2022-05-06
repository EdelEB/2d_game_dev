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
	
	ui_object*		object;
	ui_draggable*	draggable;
	//ui_sizable*		sizable;
}Editable;

typedef struct MENU_EDITOR{
	Uint8			_inuse;
	gamestate_id	id;
	Menu*			menu;

	Editable* editable_list[MAX_MENU_OBJECTS];			/**< this is an array of all the ui_label's present in the menu*/
} MenuEditor;

MenuEditor menu_editor = {0};


#endif
