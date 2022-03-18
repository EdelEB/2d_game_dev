#include "event.h"

typedef struct{
	Uint32 max_events;
	TTF_Font *title_font, *prompt_font, *text_font;
	SDL_Color font_color;
	Event* event_list;
}EventManager;

EventManager event_manager = { 0 };
Vector4D rect_color = { 255,255,255,255 };
SDL_Rect util_rect; // utility rectangle used to render all text boxes. This is probably bad practice but I don't know what else to do right now
Uint32 rest = 0;		// prevents the button clicks from activating a bunch of times
Uint32 REST_DELAY = 50;	// determines how long it will take before a button can be pressed again (game loop iterations)

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
	event_manager.title_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 80);
	event_manager.prompt_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 40);
	event_manager.text_font = TTF_OpenFont("assets/fonts/Swansea.ttf", 30);
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

void event_listen(Event* e, Uint32 mouse_state, int* mx, int* my)
{
	if (rest > 0) {
		rest--;
		return;
	}

	if (mouse_state == 1) {
		if (e->options[0]._inuse &&
			*mx > e->options[0].button_rect.x && 
			*mx < e->options[0].button_rect.x + e->options[0].button_rect.w &&
			*my > e->options[0].button_rect.y)
		{
			for (int i = 0; i < MAX_OPTIONS; i++)
			{
				if (e->options[i]._inuse &&
					*my < e->options[i].button_rect.y + e->options[i].button_rect.h)
				{
					slog("button %i pressed", i);
					rest = REST_DELAY;
					return;
				}
			}
		}
	}
}

void event_draw(Event* e) 
{
	SDL_RenderCopy(gf2d_graphics_get_renderer(), e->title_texture, NULL, &e->title_rect);
	SDL_RenderCopy(gf2d_graphics_get_renderer(), e->prompt_texture, NULL, &e->prompt_rect);

	for (int i = 0; i < MAX_OPTIONS; i++)
	{
		if (e->options[i]._inuse)
		{
			SDL_RenderCopy(gf2d_graphics_get_renderer(), e->options[i].texture, NULL, &e->options[i].render_rect);
			gf2d_draw_rect(e->options[i].button_rect, rect_color);
		}
	}
}

void create_render_variables(Event* e)
{
	SDL_Surface* surface;
	
	/*Title*/
	surface = TTF_RenderText_Solid(
		event_manager.title_font, 
		e->title, 
		event_manager.font_color
	);
	e->title_texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(), 
		surface
	);
	e->title_rect.x = WINDOW_WIDTH >> 2;
	e->title_rect.y = WINDOW_HEIGHT >> 4;
	SDL_QueryTexture(e->title_texture, NULL, NULL, &e->title_rect.w, &e->title_rect.h); // I don't understand what this does but it doesn't render the text unless I do it

	/*Prompt*/
	surface = TTF_RenderText_Solid(
		event_manager.prompt_font,
		e->prompt,
		event_manager.font_color
	);
	e->prompt_texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(),
		surface
	);
	e->prompt_rect.x = WINDOW_WIDTH >> 3;
	e->prompt_rect.y = WINDOW_HEIGHT >> 2; 
	SDL_QueryTexture(e->prompt_texture, NULL, NULL, &e->prompt_rect.w, &e->prompt_rect.h); // I don't understand what this does but it doesn't render the text unless I do it

	/*Options*/
	for (int i = 0; i < MAX_OPTIONS; i++)
	{
		if(e->options[i]._inuse)
		{
			surface = TTF_RenderText_Solid(
				event_manager.text_font,
				e->options[i].text,
				event_manager.font_color
			);
			e->options[i].texture = SDL_CreateTextureFromSurface(
				gf2d_graphics_get_renderer(),
				surface
			);
			e->options[i].render_rect.x = WINDOW_WIDTH >> 4;
			e->options[i].render_rect.y = (WINDOW_HEIGHT >> 3) * (i+3);
			//e->options[i].render_rect.w = WINDOW_WIDTH >> 2 + WINDOW_WIDTH >> 2 + WINDOW_WIDTH >> 2 + 100;
			//e->options[i].render_rect.h = WINDOW_HEIGHT >> 3; //why aren't these needed? aren't they just NULL then. What do these even do when printing out text
			SDL_QueryTexture(e->options[i].texture, NULL, NULL, &e->options[i].render_rect.w, &e->options[i].render_rect.h); // you probably shouldn't mess with this
		
			e->options[i].button_rect.x = e->options[i].render_rect.x - 20;
			e->options[i].button_rect.y = e->options[i].render_rect.y - 30;
			e->options[i].button_rect.w = (WINDOW_WIDTH >> 3)*7 ; //e->options[i].render_rect.w + 40;
			e->options[i].button_rect.h = e->options[i].render_rect.h + 60;
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
	e->title = "Asteroids Ahead";
	e->prompt = "You are approaching an asteroid field...";
	o = &e->options[0];
	o->_inuse = 1;
	o->text	= "Your the best. Navigate the asteroid field yourself. <mini game>";
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
