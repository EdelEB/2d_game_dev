#include "crew.h"

crew_member current_crew[MAX_CREW];

crew_member crew_options[TOTAL_CREW_MEMBERS];

void crew_mental_breakdown(crew_member* cm)
{
	if (!cm)
	{
		slog("crew_mental_breakdown called on NULL crew_member*");
		return;
	}
	return NONE;
}

//gamestate_id crew_lower_morale()
//{
//	int i;
//	Uint8 is_breakdown = 0;
//
//	for (i = 0; i < MAX_CREW; i++)
//	{
//		if (current_crew[i].is_alive)
//		{
//			if (current_crew[i].morale != 0) { current_crew[i].morale--; }
//			if (current_crew[i].morale == 0) { is_breakdown = 1; }
//		}
//	}
//	if (is_breakdown) { return EVENT_CREW_BREAKDOWN; }
//
//	return NONE;
//}
//
//gamestate_id crew_raise_morale()
//{
//	int i;
//	for (i = 0; i < MAX_CREW; i++)
//	{
//		if (current_crew[i] && current_crew[i]->is_alive)
//		{
//			if (current_crew[i]->morale != 10) { current_crew[i]->morale++; }
//		}
//	}
//	return NONE;
//}
//
//gamestate_id crew_lower_hunger()
//{
//	int i;
//	Uint8 is_starving = 0;
//
//	for (i = 0; i < MAX_CREW; i++)
//	{
//		if (current_crew[i] && current_crew[i]->is_alive)
//		{
//			if (current_crew[i]->hunger != 0) { current_crew[i]->hunger--; }
//			if (current_crew[i]->hunger == 0) { is_starving = 1; }
//		}
//	}
//	if (is_starving) { return EVENT_CREW_STARVING; }
//
//	return NONE;
//}
//
//gamestate_id crew_raise_hunger()
//{
//	int i;
//	for (i = 0; i < MAX_CREW; i++)
//	{
//		if (current_crew[i] && current_crew[i]->is_alive)
//		{
//			if (current_crew[i]->hunger != 10) { current_crew[i]->hunger++; }
//		}
//	}
//	return NONE;
//}


char* get_crew_title(event_clearance c)
{
	switch (c)
	{
			case PILOT			:
				return "Pilot";
			case DOCTOR			:
				return "Doctor";
			case ENGINEER		:
				return "Engineer";
			case PHYSICOLOGIST	:
				return "Phsycologist";
			case PROGRAMMER		:
				return "Programmer";
			case PAINTER		:
				return "Painter";
			case COMEDIAN		:
				return "Comedian";
			case MUSICIAN		:
				return "Musician";
			case NUTRITIONIST	:
				return "Nustritionist";
			default:
				return "DEFAULT";
	}
}

void code_vomit_create_crew_members()
{
	int i;
	char* name;
	char crew_num;

	for (i = 0; i < 8; i++) 
	{
		//name = "Crew Member";
		//crew_num = (char)i;
		//strcat(name, crew_num);

		crew_options[i].name = "Name Goes Here";
		crew_options[i].clearance = i+1;
		crew_options[i].hunger = 10;
		crew_options[i].morale = 10;
		crew_options[i].title = get_crew_title(i+1);
	}
}



int current_spot = 0;


struct CREW_SELECT_VIEW {
	ui_label	l1;
	ui_label	l2;
	ui_button	next_button;
}crew_select_view;

int crew_next_delay = 50;

void crew_next()
{
	if (crew_next_delay > 0) {
		crew_next_delay--;
		return;
	}
	slog("NEXT CREW MEMBER");
	current_spot++;
	if (!crew_options[current_spot]._inuse) {
		current_spot = 0;
	}

	crew_next_delay = 50;
	return;
}

void think_crew_select()
{
	ui_listen_button_alone(&crew_select_view.next_button);
}

void display_crew_select() 
{
	gf2d_graphics_clear_screen();

	SDL_Surface* surface;
	SDL_Color color = { 255,255,255,255 };
	TTF_Font* font = TTF_OpenFont("assets/fonts/Swansea.ttf", 30);
	SDL_Texture* texture;
	SDL_Rect rect;

	surface = TTF_RenderText_Solid(
		font,
		crew_options[current_spot].name ,
		color
	);
	texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(),
		surface
	);

	rect.x = 200;
	rect.y = 200;
	SDL_QueryTexture(
		texture,
		NULL,
		NULL,
		&rect.w,
		&rect.h
	);
	SDL_RenderCopy(
		gf2d_graphics_get_renderer(),
		texture,
		NULL,
		&rect
	);
	

	surface = TTF_RenderText_Solid(
		font,
		crew_options[current_spot].title,
		color
	);
	texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(),
		surface
	);
	rect.x = 200;
	rect.y = 400;
	SDL_QueryTexture(
		texture,
		NULL,
		NULL,
		&rect.w,
		&rect.h
	);

	SDL_RenderCopy(
		gf2d_graphics_get_renderer(),
		texture,
		NULL,
		&rect
	);

	crew_select_view.next_button = ui_create_button(10, 10, 100, 50, "NEXT", crew_next);
	ui_render_button(&crew_select_view.next_button);
	
	//l1 = ui_create_header_label(crew_options[current_spot].name, 400, 200);
	//l2 = ui_create_header_label(crew_options[current_spot].title, 400, 300);
	//
	//ui_render_label(&l1);
	//ui_render_label(&l2);
}