#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "simple_json.h"

typedef enum{
	DEFAULT,
	PILOT,
	MEDIC,
	ENGINEER,
	PHYSICOLOGIST
}event_clearance;

typedef	struct EVENT_OPTION{
	char*			text;
	event_clearance	clearance;	/**<clearance code determines if this option is visible to the player*/
	void			(*func)();	/**<function with the code that executes when option is picked */
}EventOption;

typedef struct EVENT {
	char*				name;
	char*				prompt;
	Uint32				max_options;
	EventOption*		options;

}Event;

void event_load(char* filename);

void event_manager_load_all();

void event_manager_init();

void event_free(Event* e);

void event_manager_close();

void event_executor(char* e_name);

#endif