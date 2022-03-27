#include "mini_ration_split.h"

MiniGame ration_split = { 0 };

typedef struct MY_LINE
{
	Uint8	   _inuse;
	Vector2D	pt1;
	Vector2D	pt2;
}my_line;

SDL_Rect ration_rect;
my_line line_list[MAX_CREW];
Uint8 cuts_remaining;
Uint32 cut_delay = 50;
ui_label cuts_label = { 0 };

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

	for (i = 0; i < (MAX_CREW); i++)
	{
		if (line_list[i]._inuse) continue;

		my_line temp_line;
		temp_line.pt1 = p1;
		temp_line.pt2 = p2;
		temp_line._inuse = 1;
		line_list[i] = temp_line;
		cuts_remaining--;
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
	
	for (i = 0; i < (MAX_CREW+1 - 1); i++)
	{
		if (line_list[i]._inuse)
		{
			gf2d_draw_line(line_list[i].pt1, line_list[i].pt2, color);
		}
	}

	//This is gross and will be fixed later
	char* str[20];
	sprintf(str, "Cuts Remaining: %d", cuts_remaining);
	cuts_label = ui_create_text_label(
		str,
		WINDOW_WIDTH - 275,
		50
	);
	ui_label_render(&cuts_label);
}

gamestate_id determine_ration_result()
{
	int i, j;
	float biggest = 0, smallest = 1500, tempf;
	my_line temp;
	
	slog("PreSort:");
	for (i = 0; i < MAX_CREW; i++)
	{
		slog("Line %d : %f", i, line_list[i].pt1.x);
	}
	/* bubble sort */
	for(i = 0 ; i < MAX_CREW-1 ; i++)
	{ 
		for (j = i+1; j < MAX_CREW; j++)
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
	for (i = 0; i < MAX_CREW; i++)
	{
		slog("Line %d : %f", i, line_list[i].pt1.x);
	}
	
	for (i = 0; i < MAX_CREW+1; i++) {
		/*first slice*/
		if (i == 0){
			tempf = line_list[i].pt1.x - ration_rect.x;
		}
		/*last slice*/
		else if (i == MAX_CREW) {
			tempf = ration_rect.x + ration_rect.w - line_list[i - 1].pt1.x;
		}
		/*intermitant slices*/
		else{
			tempf = line_list[i].pt1.x - line_list[i - 1].pt1.x;
		}

		slog("temp(%.0f) biggest(%.0f) smallest(%.0f)", tempf, biggest, smallest);

		if (tempf >= biggest) { biggest = tempf; }
		if (tempf <= smallest) { smallest = tempf; }
	}

	/*Win Condition is based on the difference in size between the biggest and smallest piece*/
	slog("biggest(%.0f) - smallest(%.0f) = %.0f", biggest, smallest, biggest - smallest);
	if (biggest - smallest < 70){
		return SUCCESS_RATION_SPLIT;
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

	cuts_remaining = MAX_CREW;

	for (i = 0; i < MAX_CREW; i++)
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

	if (cuts_remaining < 1) {
		self->end_state = determine_ration_result();
	}

	listen_cuts();

}

void mini_ration_end(MiniGame* self)
{
	slog("Minigame ration split ended");
}
