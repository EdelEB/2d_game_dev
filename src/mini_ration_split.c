#include "mini_ration_split.h"

MiniGame ration_split = { 0 };

typedef struct MY_LINE
{
	Uint8	   _inuse;
	Vector2D	pt1;
	Vector2D	pt2;
}my_line;

my_line line_list[MAX_CREW-1];

Uint8 cut_count = 0;

SDL_Rect ration_rect;

Uint32 cut_delay = 50;

MiniGame* mini_ration_init(void)
{
	ration_split.title = "Ration Split";
	ration_split.id = MINI_RATION_SPLIT;
	ration_split.background = gf2d_sprite_load_image("assets/images/backgrounds/bg_ration.jpg");
	ration_split.start = mini_ration_start;
	ration_split.run = mini_ration_run;
	ration_split.end = mini_ration_end;

	ration_rect.x = WINDOW_WIDTH >> 4;
	ration_rect.y = WINDOW_HEIGHT >> 3;
	ration_rect.w = (WINDOW_WIDTH >> 4) * 14;
	ration_rect.h = (WINDOW_HEIGHT >> 3) * 6;

	return &ration_split;
}

void add_line(Vector2D p1, Vector2D p2)
{
	int i;

	for (i = 0; i < (MAX_CREW - 1); i++)
	{
		if (line_list[i]._inuse) continue;

		my_line temp_line;
		temp_line.pt1 = p1;
		temp_line.pt2 = p2;
		temp_line._inuse = 1;
		line_list[i] = temp_line;
		cut_count++;
		return;
	}

	
}

void listen_cuts()
{
	if (cut_delay > 0) {
		cut_delay--;
		return;
	}

	int mx, my;
	Uint32 mouse_state = SDL_GetMouseState(&mx, &my);

	if (mouse_state == 1 &&
		mx > ration_rect.x &&
		mx < ration_rect.x + ration_rect.w &&
		my > ration_rect.y &&
		my < ration_rect.y + ration_rect.h)
	{
			slog("RATION CLICKED");
			add_line(vector2d(mx, ration_rect.y), vector2d(mx, ration_rect.y+ration_rect.h));

			cut_delay = 50;
	}

}

void mini_ration_draw()
{
	int i;
	Vector4D color = { 255, 255, 255, 255 };

	gf2d_draw_rect(ration_rect, color);
	
	for (i = 0; i < (MAX_CREW - 1); i++)
	{
		if (line_list[i]._inuse)
		{
			gf2d_draw_line(line_list[i].pt1, line_list[i].pt2, color);
		}
	}
}

gamestate_id determine_ration_result()
{
	int i, j;
	my_line temp;
	
	slog("PreSort:");
	for (i = 0; i < MAX_CREW - 1; i++)
	{
		
		slog("Line %i : %f", i, line_list[i].pt1.x);
	}

	/* bubble sort */
	for(i = 0 ; i < MAX_CREW-2 ; i++)
	{ 
		for (j = i+1; j < MAX_CREW - 1; j++)
		{
			if (line_list[i].pt1.x > line_list[j].pt1.x)
			{
				temp = line_list[i];
				line_list[i] = line_list[j];
				line_list[j] = temp;
			}
		}
	}

	slog("PostSort");
	for (i = 0; i < MAX_CREW - 1; i++)
	{
		slog("Line %i : %f", i, line_list[i].pt1.x);
	}

	return FAIL_RATION_SPLIT;
}

void mini_ration_start(MiniGame* self)
{
	int i;

	if (!self) {
		slog("mini_ration_start called with NULL MiniGame*");
		return;
	}

	for (i = 0; i < (MAX_CREW - 1); i++)
	{
		line_list[i]._inuse = 0;
	}

	mini_start(self);
}


void mini_ration_run(MiniGame* self)
{
	if (!self) {
		slog("mini_ration_run called with NULL MiniGame*");
		return;
	}

	if (cut_count == MAX_CREW - 1) {
		slog("Cut Count: %i", cut_count);
		self->end_state = determine_ration_result();
	}

	listen_cuts();

}

void mini_ration_end(MiniGame* self)
{
	slog("Minigame ration split ended");
}
