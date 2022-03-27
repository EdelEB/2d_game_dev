
#include "map.h"

//void map_init(void)
//{
//	SDL_Surface *surface;
//	SDL_Color color = { 255,255,255,255 };
//
//	map.image = gf2d_sprite_load_image("assets/images/backgrounds/bg_map.png");
//	map.ship_image = gf2d_sprite_load_image("assets/images/my_ship2.png");
//
//
//	map.crew_button_font = TTF_OpenFont("assets/fonts/Swansea.ttf", 30);
//	surface = TTF_RenderText_Solid(
//		map.crew_button_font,
//		"Crew View",
//		color	
//	);
//	map.crew_button_texture = SDL_CreateTextureFromSurface(
//		gf2d_graphics_get_renderer(),
//		surface
//	);
//	map.crew_button_rect.x = 10;
//	map.crew_button_rect.y = 10;
//	SDL_QueryTexture(
//		map.crew_button_texture, 
//		NULL, 
//		NULL, 
//		&map.crew_button_rect.w, 
//		&map.crew_button_rect.h
//	); 
//
//	// NOTE : This is where game data will be loaded
//	map.spot = 0;
//	map_set_position_by_spot(0);
//}

void map_init(void)
{
	map.image = gf2d_sprite_load_image("assets/images/backgrounds/bg_map.png");
	map.ship_image = gf2d_sprite_load_image("assets/images/my_ship2.png");

	
	map.crew_view_button = ui_create_button(
		20,
		20,
		170,
		40,
		"Crew View",
		map_to_crew_view		
	);

	map.travel_button = ui_create_button(
		WINDOW_WIDTH-120,
		WINDOW_HEIGHT-120,
		170,
		40,
		"Travel",
		map_travel
	);

	// NOTE : This is where game data will be loaded
	map.spot = 0;
	map_set_position_by_spot(0);
}

gamestate_id map_listen(Uint32 mouse_state, int mx, int my)
{
	gamestate_id id;
	id = ui_button_listen(&map.crew_view_button, mouse_state, mx, my);
	if (id) { return id; }
	id = ui_button_listen(&map.travel_button, mouse_state, mx, my);
	return id;

}

void map_draw(void)
{
	gf2d_sprite_draw_image(map.image, vector2d(0,0));
	gf2d_sprite_draw_image(map.ship_image, map.position);
	
	ui_button_render(&map.crew_view_button);
	ui_button_render(&map.travel_button);
}	

void map_set_position_by_spot(Uint32 spot)
{
	switch (spot)
	{
		case 0:
			map.position.x = 20;
			map.position.y = 630;
			break;
		case 1:
			map.position.x = 304;
			map.position.y = 510;
			break;
		case 2:
			map.position.x = 545;
			map.position.y = 366;
			break;
		case 3:
			map.position.x = 764;
			map.position.y = 230;
			break;
		case 4:
			map.position.x = 954;
			map.position.y = 130;
			break;
	}
}

gamestate_id map_travel(void) 
{
	map.spot++;
	map_set_position_by_spot(map.spot);

	switch (map.spot)
	{
	case 0:
		break;
	case 1:
		return EVENT_ASTEROIDS_AHEAD;
	case 2:
		return EVENT_AI_TAKEOVER;
	case 3:
		return EVENT_RATIONS_MISSING;
	case 4:
		return EVENT_RATIONS_LOW;
	}

	return NONE;
}

gamestate_id map_to_crew_view(void)
{
	return CREW_VIEW;
}
