
#include "map.h"
#include "simple_logger.h"

void map_init()
{
	map.image = gf2d_sprite_load_image("assets/images/backgrounds/bg_map.png");
	map.ship_image = gf2d_sprite_load_image("assets/images/my_ship2.png");

	// NOTE : This is where game data will be loaded
	map.spot = 0;
	map_set_position_for_spot(0);
}

void map_set_position_for_spot(Uint32 spot)
{
	switch (spot)
	{
		case 0:
			map.position.x = 20;
			map.position.y = 630;
			break;
	}
}

void map_draw()
{
	gf2d_sprite_draw_image(map.image, vector2d(0,0));
	gf2d_sprite_draw_image(map.ship_image, map.position);
}