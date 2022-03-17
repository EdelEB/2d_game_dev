#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_graphics.h"

#define MAX_OPTIONS 5
extern const Uint32 WINDOW_HEIGHT;
extern const Uint32 WINDOW_WIDTH;

typedef enum {
	ASTEROIDS_AHEAD,
	RATIONS_LOW,
	RATIONS_MISSING,
	MOUSE_FOUND
}event_id;

typedef enum{
	DEFAULT,
	PILOT,
	MEDIC,
	ENGINEER,
	PHYSICOLOGIST
}event_clearance;

typedef	struct EVENT_OPTION{
	Uint8			_inuse;
	char*			text;
	event_clearance	clearance;	/**<clearance code determines if this option is visible to the player*/

	SDL_Texture*	texture;
	SDL_Rect		rect; 
}EventOption;

typedef struct EVENT{
	Uint8			   _inuse;
	char*				title;
	event_id			id;
	char*				prompt;
	EventOption			options[MAX_OPTIONS]; /**<location of the options in the array is important becuase it determines where it is displayed and which function is called on_press*/

	SDL_Texture		   *title_texture, *prompt_texture;	/**<for rendering purposes*/
	//for some reason I get a memory access location exception when I make this SDL_Rect a pointer, so it's not
	SDL_Rect			title_rect, prompt_rect;		/**<for rendering purposes*/
}Event;

void event_manager_init(Uint32 max_events);

void event_manager_load_all();

void event_free(Event* e);

void event_manager_close();

Event* get_event_by_id(event_id id);

void event_log(Event* e);

void code_vomit_add_all_events();

void event_draw(Event* e);

#endif