#include <SDL.h>
#include "simple_logger.h"

#include "event.h"

typedef struct{
	Uint32 max_events;
	Event* event_list;
}EventManager;

EventManager event_manager = { 0 };

void event_manager_init(Uint32 max_events)
{
	if (max_events == 0)
	{
		slog("Cannot allocate memory for zero events. Stop wasting my time");
		return;
	}
	if (event_manager.event_list)
	{
		slog("event manager already initialized dumb dumb");
	}
	event_manager.max_events = max_events;
	event_manager.event_list = gfc_allocate_array(sizeof(Event), max_events);

	atexit(event_manager_close);
	slog("event manager initialized");
}

Event* event_new()
{
	int i;
	for (i = 0; i < event_manager.max_events; i++)
	{
		if (event_manager.event_list[i]._inuse) continue;
		event_manager.event_list[i]._inuse = 1;
		return &event_manager.event_list[i];
	}
	return NULL;
}

void event_free(Event* e)
{
	if (!e)	{ slog("cannot free nothing (event)"); }
	else { memset(e, 0, sizeof(Event)); }
}

void event_manager_close() 
{
	int i;
	for (i = 0; i < event_manager.max_events; i++)
	{
		event_free(&event_manager.event_list[i]);
	}
	if (event_manager.event_list) {
		free(event_manager.event_list);
	}
}

void event_manager_load_all()
{
	SJson* json = sj_load("assets/json/test.json");

	if (json)
	{
		slog("%s", json->get_string);
	}
	else 
	{
		slog("No Json");
	}
}

Event* get_event_by_title(event_title title)
{
	int i;
	for (i = 0; i < event_manager.max_events; i++)
	{
		if (event_manager.event_list[i].title == title)
		{
			return &event_manager.event_list[i];
		}
	}
	// Event with <title> doesn't exist
	return NULL;
}

void event_log(Event* e) 
{
	if (!e) { slog("-ERROR: tried logging a NULL event"); return; }
	int i;

	slog("\n-EVENT LOG: %s\nPrompt: %s", e->name, e->prompt);
	for (i = 0; i < 4; i++)
	{
		if (&e->options[0])
		{
			slog("\n-Option %i: %s\n\-Clearance: %i", 
				i, 
				e->options[i].text, 
				e->options[i].clearance);
		}
	}
}


// I know this is gross and there is a better way to do this with 
// JSON but it isn't working right now and I am annoyed
void code_vomit_add_all_events() 
{
	Event* e = event_new();
	EventOption *o;

	if (!e) 
	{ 
		slog("failed to create event in code vomit"); 
		return; 
	}
	e->title = ASTEROIDS_AHEAD;
	e->name	= "Asteroids Ahead";
	e->prompt = "You are approaching an asteroid field...";
	o = &e->options[0];
	o->text	= "Your the bes=-t. Navigate the asteroid field yourself. <mini game>";
	o->clearance = DEFAULT;
	o = &e->options[1];
	o->text	= "Let Jesus take the wheel. <random>";
	o->clearance = DEFAULT;
	o = &e->options[2];
	o->text	= "Trust your pilot to guide you through the field. <skip mini game>";
	o->clearance = PILOT;
	o = &e->options[3];
	o->text	= "Take the long way around";
	o->clearance = DEFAULT;


	e = event_new();
	if (!e){
		slog("failed to create event in code vomit");
		return;
	}
	e->title =	RATIONS_LOW;
	e->name = "Rations Low";
	e->prompt = "You are running out of food. It seems you have to split your rations. How will you do so.";
	o = &e->options[0];
	o->text = "Try and divide them equally. <mini game>";
	o->clearance = DEFAULT;
	o = &e->options[1];
	o->text = "I am the greatest and most important. I will eat all the rations myself.";
	o->clearance = DEFAULT;
	o = &e->options[2];
	o->text = "Let the crew fight it out.";
	o->clearance = DEFAULT;


	e = event_new();
	if (!e) {
		slog("failed to create event in code vomit");
		return;
	}
	e->title = RATIONS_MISSING;
	e->name = "Rations Missing";
	e->prompt = "You discover that some of the rations are missing.";
	o = &e->options[0];
	o->text = "Investigate the crime seen.";
	o->clearance = DEFAULT;
	o = &e->options[1];
	o->text = "Tell the group to deal with it.";
	o->clearance = DEFAULT;
	o = &e->options[2];
	o->text = "Blame someone you don't like.";
	o->clearance = DEFAULT;


	e = event_new();
	if (!e) {
		slog("failed to create event in code vomit");
		return;
	}
	e->title = MOUSE_FOUND;
	e->name = "Mouse Found";
	e->prompt = "You discover a mouse has stowed away on your ship and has been eating your rations";
	o = &e->options[0];
	o->text = "Try and throw things at it. <mini game>";
	o->clearance = DEFAULT;
	o = &e->options[1];
	o->text = "Have you engineer make a mouse trap. <skip mini game>";
	o->clearance = DEFAULT;
	o = &e->options[2];
	o->text = "Blame someone you don't like.";
	o->clearance = ENGINEER;
}
