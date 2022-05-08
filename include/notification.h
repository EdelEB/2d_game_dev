#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "gf2d_graphics.h"
#include "simple_logger.h"

#include "gamestate.h"
#include "menu.h"

#define MAX_LINES 4
extern const Uint32 WINDOW_HEIGHT;
extern const Uint32 WINDOW_WIDTH;

typedef struct NOTIFICATION{
	Uint8		   _inuse;
	gamestate_id	id;
	char*			title;
	char*			line_strings[MAX_LINES];
}Notification;

void note_manager_init(Uint32 max_notes);

void note_manager_close(void);

Notification* note_new(void);

void note_save_all(char* filename);

SJson* note_to_json(Notification* n);

void note_menu_load_all(char* filename);

#endif