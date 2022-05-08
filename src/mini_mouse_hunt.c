#include "mini_mouse_hunt.h"

MiniGame mouse_hunt = { 0 };

#define ROUND_DELAY 400;
int mouse_round_timer = ROUND_DELAY;
const Uint32 MAX_MOUSE_ROUNDS = 10;
Uint32 mouse_round = 0;

MiniGame* mini_mouse_init()
{	
	mouse_hunt.id = MINI_MOUSE_HUNT;
	mouse_hunt.title = "Mouse Hunt";
	mouse_hunt.end_state = NONE;
	mouse_hunt.start = mini_mouse_start;
	mouse_hunt.run = mini_mouse_run;
	mouse_hunt.end = mini_mouse_end;
	mouse_hunt.background = gf2d_sprite_load_image("assets/images/minis/mouse_hunt/bg_ship_interior.png");
	
	slog("Mouse Hunt Initialized");
	return &mouse_hunt;
}

void mini_mouse_round(MiniGame * self)
{
	slog("MOUSE ROUND");

	

	if (self->entities[0]->position.x < 0) 
	{
		self->entities[0]->position = vector2d(
				-50,
				simple_random(300, 700)
			);

		self->entities[0]->velocity = vector2d(
			simple_random(9, 12),
			simple_random(-4, 4)
		);
	}
	else
	{
		self->entities[0]->position = vector2d(
			WINDOW_WIDTH + 50,
			simple_random(300, 700)
		);


		self->entities[0]->velocity = vector2d(
			simple_random(-12, -9),
			simple_random(-4, 4)
		);
	}


}

Uint8 mouse_hit()
{
	int mx, my;
	Uint32 mouse_state = SDL_GetMouseState(&mx, &my);

	if (mouse_hunt.entities[0] && mouse_state == 1)
	{
		if (mx > mouse_hunt.entities[0]->hitbox.x &&
			mx < mouse_hunt.entities[0]->hitbox.x + mouse_hunt.entities[0]->hitbox.w &&
			my > mouse_hunt.entities[0]->hitbox.y &&
			my < mouse_hunt.entities[0]->hitbox.y + mouse_hunt.entities[0]->hitbox.h)

			return 1;
	}
	return 0;

}

void mini_mouse_start(MiniGame* self)
{
	mini_start(self);

	mouse_round = 0;

	self->entities[0] = ent_mouse_new(vector2d(-100, 0));
}

void mini_mouse_run(MiniGame* self)
{
	if (mouse_round_timer > 0)
	{
		mouse_round_timer--;
	}
	else
	{
		mini_mouse_round(self);
		mouse_round++;
		mouse_round_timer = ROUND_DELAY;
	}
	
	if (mouse_round > MAX_MOUSE_ROUNDS)
	{
		self->end_state = FAIL_MOUSE_HUNT;
	}
	if (mouse_hit())
	{
		//slog("MOUSE CLICKED");
		self->entities[0]->sprite = gf2d_sprite_load_image("assets/images/minis/mouse_hunt/mouse_dead.png");
		self->end_state = SUCCESS_MOUSE_HUNT;
	}

}

void mini_mouse_end()
{
	slog("Minigame Mouse Hunt Ended");
}