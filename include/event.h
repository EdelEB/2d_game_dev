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

	SDL_Texture    *texture;
	SDL_Rect		render_rect, button_rect; 

	gamestate_id   clicked;   /**<a pointer to a clicked function for this option*/
}EventOption;

typedef struct EVENT{
	Uint8		   _inuse;
	char		   *title;
	gamestate_id	id;
	char		   *prompt;
	EventOption		options[MAX_OPTIONS]; /**<location of the options in the array is important becuase it determines where it is displayed and which function is called on_press*/

	SDL_Texture	   *title_texture, *prompt_texture;	/**<for rendering purposes*/
	//for some reason I get a memory access location exception when I make this SDL_Rect a pointer, so it's not
	SDL_Rect		title_rect, prompt_rect;		/**<for rendering purposes*/
}Event;

/**
 * @brief creates a manager which holds an array of events as well as some rendering info (text_label fonts and color)
 * @param max_events is the maximum number of events that the manager is capable of holding at one time
 */
void event_manager_init(Uint32 max_events);

/* NOT WORKING AS INTENDED */
void event_manager_load_all(void);

void event_free(Event* e);

void event_manager_close(void);

/**
 * @brief looks through all currently initialized events and returns the one with the inputted id
 * @param id is an used to identify unique events (multiple events should not have the same id)
 * @return a pointer to the Event with id or NULL if not found
 */
Event* get_event_by_id(gamestate_id id);

/**
 * @brief uses simple_logger's slog to log all string variables in an event. This is for debugging
 * @param e is the Event* whose variables will be logged
 */
void event_log(Event* e);

/**
 * @brief draws all event text_label(title, prompt, options) to the screen and button outlines
 * @param e is a Event* that provides the strings to be printed
 */
void event_draw(Event* e);

/**
 * @brief listens for mouse events that and determines if/which event button is pressed
 * @param e is the Event object that it will be listening for. This is important because it determines how many buttons it is checking for and what functions will be called if one is clicked
 * @param mouse_state = 1 if mouse is clicked
 * @param mx is the x position of the mouse
 * @param my is the y position of the mouse
 * @return if something is pressed it returns the corresponding new gamestate_id, NULL if no buttons are pressed
 */
gamestate_id event_listen(Event* e, Uint32 mouse_state, int* mx, int* my);



void event_manager_save_all(char* filename);

SJson* event_to_json(Event* e);

Menu* event_json_to_menu(SJson* json);

Menu* event_menu_from_json(SJson* json);


/**
 * @brief this initializes all unique events. 
 * @note simple_json is giving me issues. This is my alternative. It's called code_vomit because I know it's terrible and bad practice.
 */
void code_vomit_add_all_events(void);

#endif