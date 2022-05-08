#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_graphics.h"
#include "gf2d_draw.h"

#include "menu.h"
#include "gamestate.h"

#define MAX_OPTIONS 5
extern const Uint32 WINDOW_HEIGHT;
extern const Uint32 WINDOW_WIDTH;

typedef	struct EVENT_OPTION{
	Uint8		   _inuse;

	char		   *text;
	event_clearance	clearance;	/**<clearance code determines if this option is visible to the player*/
	gamestate_id   clicked;   /**<a pointer to a clicked function for this option*/
}EventOption;

typedef struct EVENT{
	Uint8		   _inuse;
	char		   *title;
	gamestate_id	id;
	char		   *prompt;
	EventOption		options[MAX_OPTIONS];
}Event;

/**
 * @brief creates a manager which holds an array of events as well as some rendering info (text_label fonts and color)
 * @param max_events is the maximum number of events that the manager is capable of holding at one time
 */
void event_manager_init(Uint32 max_events);

void event_free(Event* e);

void event_manager_close(void);


void event_menu_load_all(char* filename);

void event_manager_save_all(char* filename);

SJson* event_to_json(Event* e);

Menu* event_menu_from_json(SJson* json);

#endif