#ifndef __EVENT_SIMPLE_H__
#define __EVENT_SIMPLE_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "gf2d_graphics.h"
#include "simple_logger.h"

#include "gamestate.h" 
#include "ui_stuff.h"
//#include "enum_declarations.h"

#define MAX_LINES 4
extern const Uint32 WINDOW_HEIGHT;
extern const Uint32 WINDOW_WIDTH;

typedef struct EVENT_SIMPLE {
	Uint8		   _inuse;
	gamestate_id	id;
	
	ui_label		labels[MAX_LINES];

}Event_Simple;

void event_simple_manager_init(Uint32 max_event_simples);

void event_simple_manager_close(void);

Event_Simple* event_simple_new(void);

Event_Simple* get_event_simple_by_id(gamestate_id id);

void event_simple_draw(Event_Simple* es);

gamestate_id event_simple_listen(Uint32 mouse_state);

void load_events_simple(char* filename);

//void add_all_event_simples(void);

#endif