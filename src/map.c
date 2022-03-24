
#include "map.h"

void map_init()
{
	SDL_Surface *surface;
	SDL_Color color = { 255,255,255,255 };

	map.image = gf2d_sprite_load_image("assets/images/backgrounds/bg_map.png");
	map.ship_image = gf2d_sprite_load_image("assets/images/my_ship2.png");


	map.crew_button_font = TTF_OpenFont("assets/fonts/Swansea.ttf", 30);
	surface = TTF_RenderText_Solid(
		map.crew_button_font,
		"Crew View",
		color	
	);
	map.crew_button_texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(),
		surface
	);
	map.crew_button_rect.x = 10;
	map.crew_button_rect.y = 10;
	SDL_QueryTexture(
		map.crew_button_texture, 
		NULL, 
		NULL, 
		&map.crew_button_rect.w, 
		&map.crew_button_rect.h
	); 

	// NOTE : This is where game data will be loaded
	map.spot = 0;
	map_set_position_by_spot(0);
}

void map_set_position_by_spot(Uint32 spot)
{
	switch (spot)
	{
		case 0:
			map.position.x = 20;
			map.position.y = 630;
			break;
	}
}

gamestate_id map_listen(Uint32 mouse_state, int mx, int my)
{

	if (mouse_state == 1 &&
		mx > map.crew_button_rect.x &&
		mx < map.crew_button_rect.x + 50 &&
		my > map.crew_button_rect.y &&
		my < map.crew_button_rect.y + 20)
	{
		return CREW_VIEW;
	}

	return NONE;
}

void map_draw()
{
	gf2d_sprite_draw_image(map.image, vector2d(0,0));
	gf2d_sprite_draw_image(map.ship_image, map.position);
	SDL_RenderCopy(
		gf2d_graphics_get_renderer(), 
		map.crew_button_texture, 
		NULL, 
		&map.crew_button_rect
	);
	//gf2d_draw_rect(map.crew_button_rect, vector4d(255,255,255,255));
}	