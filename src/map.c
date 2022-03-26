
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

gamestate_id map_to_crew_view(void)
{
	return CREW_VIEW;
}

gamestate_id map_listen(Uint32 mouse_state, int mx, int my)
{
	return ui_button_listen(&map.crew_view_button, mouse_state, mx, my);
}

void map_draw(void)
{
	gf2d_sprite_draw_image(map.image, vector2d(0,0));
	gf2d_sprite_draw_image(map.ship_image, map.position);
	
	ui_button_render(&map.crew_view_button);
}	