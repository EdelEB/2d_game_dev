#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "simple_json.h"

typedef enum {
	ASTEROIDS_AHEAD,
	RATIONS_LOW,
	RATIONS_MISSING,
	MOUSE_FOUND
}event_title;

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
}EventOption;

typedef struct EVENT{
	char*				name;
	Uint8				_inuse;
	event_title			title;
	char*				prompt;
	EventOption			options[5]; /**<location of the options in the array is important becuase it determines where it is displayed and which function is called on_press*/
}Event;

void event_manager_init(Uint32 max_events);

void event_manager_load_all();

void event_free(Event* e);

void event_manager_close();

Event* get_event_by_title(event_title title);

void event_log(Event* e);

void code_vomit_add_all_events();

#endif