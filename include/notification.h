#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "gf2d_graphics.h"
#include "simple_logger.h"

#include "game_state.h"

#define MAX_LINES 4
extern const Uint32 WINDOW_HEIGHT;
extern const Uint32 WINDOW_WIDTH;

typedef struct NOTIFICATION{
	Uint8		   _inuse;
	game_state_id	id;
	char*			title;
	char*			line_strings[MAX_LINES];

	SDL_Texture	   *title_texture;
	
	SDL_Texture	   *line_textures[MAX_LINES];
	SDL_Rect		title_rect;
	SDL_Rect		line_rects[MAX_LINES];

}Notification;

void note_manager_init(Uint32 max_notes);

void note_manager_close();

Notification* note_new();

void note_free(Notification* n);

void note_create_render_variables(Notification *n);

void note_create_all_render_variables();

Notification* get_note_by_id(game_state_id id);

void note_log(Notification* n);

void note_log_all();

void note_draw(Notification* n);

game_state_id note_listen(Uint32 mouse_state);

void add_all_notes();

#endif