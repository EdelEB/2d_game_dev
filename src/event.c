#include "event.h"

typedef struct{
	Uint32 max_events;
	TTF_Font* title_font;
	TTF_Font* text_font;
	SDL_Color font_color;
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
	event_manager.title_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 20);
	event_manager.text_font = TTF_OpenFont("assets/fonts/Swansea.ttf", 20);
	SDL_Color temp = { 255,255,255,255 }; // Why is this necessary you ask? I don't know. I get an error if I do it directly without the temporary variable
	event_manager.font_color = temp; 

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

Event* get_event_by_id(event_id id)
{
	int i;
	for (i = 0; i < event_manager.max_events; i++)
	{
		if (event_manager.event_list[i].id == id)
		{
			return &event_manager.event_list[i];
		}
	}
	// Event with <id> doesn't exist
	return NULL;
}

void event_log(Event* e) 
{
	if (!e) { slog("-ERROR: tried logging a NULL event"); return; }
	int i;

	slog("\n-EVENT LOG: %s\nPrompt: %s", e->title, e->prompt);
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

void event_listen(Uint8 mouse_state, int* mx, int* my)
{
	if (mouse_state == 1) {

		if (true)
		{

		}

	}
}

void event_draw(Event* e) 
{
	SDL_RenderCopy(gf2d_graphics_get_renderer(), e->title_texture, NULL, &e->title_rect);

	for (int i = 0; i < MAX_OPTIONS; i++)
	{
		if (e->options[i]._inuse)
		{
			SDL_RenderCopy(gf2d_graphics_get_renderer(), e->options[i].texture, NULL, &e->options[i].rect);
		}
	}
}

void create_render_variables(Event* e)
{
	SDL_Surface* surface;
	
	surface = TTF_RenderText_Solid(
		event_manager.title_font, 
		e->title, 
		event_manager.font_color
	);
	e->title_texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(), 
		surface
	);
	e->title_rect.x = WINDOW_WIDTH / 3;
	e->title_rect.y = WINDOW_HEIGHT / 8;

	SDL_QueryTexture(e->title_texture, NULL, NULL, &e->title_rect.w, &e->title_rect.h);

	for (int i = 0; i < MAX_OPTIONS; i++)
	{
		if(e->options[i]._inuse)
		{
			//slog("Creating Textures for %s : %i",e->title, i);

			surface = TTF_RenderText_Solid(
				event_manager.text_font,
				e->options[i].text,
				event_manager.font_color
			);
			e->options[i].texture = SDL_CreateTextureFromSurface(
				gf2d_graphics_get_renderer(),
				surface
			);

			e->options[i].rect.x = WINDOW_WIDTH / 3;
			e->options[i].rect.y = WINDOW_HEIGHT/8 + WINDOW_HEIGHT/8 * (i+1);
			//e->options[i].rect.w = 100;
			//e->options[i].rect.h = 10; //why aren't these needed? aren't they just NULL then. What do these even do when printing out text

			SDL_QueryTexture(e->options[i].texture, NULL, NULL, &e->options[i].rect.w, &e->options[i].rect.h); // you probably shouldn't mess with this
		}
	}
}

void create_all_render_variables() 
{
	int i;
	for (i = 0; i < event_manager.max_events; i++)
	{
		if (event_manager.event_list[i]._inuse) 
		{
			create_render_variables(&event_manager.event_list[i]);
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
	e->id = ASTEROIDS_AHEAD;
	e->title	= "Asteroids Ahead";
	e->prompt = "You are approaching an asteroid field...";
	o = &e->options[0];
	o->_inuse = 1;
	o->text	= "Your the bes=-t. Navigate the asteroid field yourself. <mini game>";
	o->clearance = DEFAULT;
	o = &e->options[1];
	o->_inuse = 1;
	o->text	= "Let Jesus take the wheel. <random>";
	o->clearance = DEFAULT;
	o = &e->options[2];
	o->_inuse = 1;
	o->text	= "Trust your pilot to guide you through the field. <skip mini game>";
	o->clearance = PILOT;
	o = &e->options[3];
	o->_inuse = 1;
	o->text	= "Take the long way around";
	o->clearance = DEFAULT;


	e = event_new();
	if (!e){
		slog("failed to create event in code vomit");
		return;
	}
	e->id =	RATIONS_LOW;
	e->title = "Rations Low";
	e->prompt = "You are running out of food. It seems you have to split your rations. How will you do so.";	
	o = &e->options[0];
	o->_inuse = 1;
	o->text = "Try and divide them equally. <mini game>";
	o->clearance = DEFAULT;
	o = &e->options[1];
	o->_inuse = 1;
	o->text = "I am the greatest and most important. I will eat all the rations myself.";
	o->clearance = DEFAULT;
	o = &e->options[2];
	o->_inuse = 1;
	o->text = "Let the crew fight it out.";
	o->clearance = DEFAULT;


	e = event_new();
	if (!e) {
		slog("failed to create event in code vomit");
		return;
	}
	e->id = RATIONS_MISSING;
	e->title = "Rations Missing";
	e->prompt = "You discover that some of the rations are missing.";
	o = &e->options[0];
	o->_inuse = 1;
	o->text = "Investigate the crime seen.";
	o->clearance = DEFAULT;
	o = &e->options[1];
	o->_inuse = 1;
	o->text = "Tell the group to deal with it.";
	o->clearance = DEFAULT;
	o = &e->options[2];
	o->_inuse = 1;
	o->text = "Blame someone you don't like.";
	o->clearance = DEFAULT;


	e = event_new();
	if (!e) {
		slog("failed to create event in code vomit");
		return;
	}
	e->id = MOUSE_FOUND;
	e->title = "Mouse Found";
	e->prompt = "You discover a mouse has stowed away on your ship and has been eating your rations";
	o = &e->options[0];
	o->_inuse = 1;
	o->text = "Try and throw things at it. <mini game>";
	o->clearance = DEFAULT;
	o = &e->options[1];
	o->_inuse = 1;
	o->text = "Have you engineer make a mouse trap. <skip mini game>";
	o->clearance = DEFAULT;
	o = &e->options[2];
	o->_inuse = 1;
	o->text = "Blame someone you don't like.";
	o->clearance = ENGINEER;



	create_all_render_variables();
}
