#include <SDL.h>
#include "simple_logger.h"

#include "event.h"

static Uint32 MAX_EVENTS = 20;

typedef struct{
	Uint32 max_events;
	Event* event_list;
}EventManager;

static EventManager event_manager = { 0 };

void event_manager_init()
{
	if (event_manager.event_list)
	{
		slog("event manager already initialized dumb dumb");
	}
	event_manager.max_events = MAX_EVENTS;
	event_manager.event_list = gfc_allocate_array(sizeof(Event), MAX_EVENTS);

	atexit(event_manager_close);
	slog("event manager initialized");
}

Event* event_new()
{
	int i;
	for (i = 0; i < event_manager.max_events; i++)
	{
		// Do I need an inuse var if I can use name? Can I use name?
		if (event_manager.event_list[i].name) 
		{
			//This is here to make sure the same event slot isn't given away twice.
			event_manager.event_list[i].name = 'p'; // placeholder
			return &event_manager.event_list[i];
		}
	}
}

void event_free(Event* e)
{
	if (!e)
	{
		slog("cannot free nothing (event)");
		return;
	}
	else
	{
		memset(e, 0, sizeof(Event));
	}
}

void event_manager_close() 
{
	int i;

	for (i = 0; i < event_manager.max_events; i++)
	{
		
	}
}

void event_manager_load_all()
{
	SJson* json = sj_load("assets/json/test.json");

	if (json)
	{
		slog("%s", json->get_string);
		slog( "%s", json->sjtype);
	}
	else 
	{
		slog("No Json");
	}
}

Event* get_event_by_name(char* name)
{
	int i;
	for (i = 0; i < event_manager.max_events; i++)
	{
		if (strcmp(event_manager.event_list[i].name, name) == 0)
		{
			return &event_manager.event_list[i];
		}
	}
	// Event <name> doesn't exist
	return NULL;
}
