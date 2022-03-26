#include "ui_stuff.h"

void ui_font_info_init(void)
{
	font_info.title_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 80);
	font_info.header_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 35);
	font_info.text_font = TTF_OpenFont("assets/fonts/Swansea.ttf", 30);
	SDL_Color temp = { 255,255,255,255 }; // Why is this necessary you ask? I don't know. I get an error if I do it directly without the temporary variable
	font_info.font_color = temp;
}

ui_label ui_create_label_helper(char* str, int x, int y, TTF_Font* font)
{
	SDL_Surface* surface;
	ui_label	 label;

	label._inuse = 1;

	surface = TTF_RenderText_Solid(
		font,
		str,
		font_info.font_color
	);
	label.texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(),
		surface
	);
	label.render_rect.x = x;
	label.render_rect.y = y;
	label.render_rect.w = 200;
	label.render_rect.h = 200;
	SDL_QueryTexture(
		label.texture,
		NULL,
		NULL,
		&label.render_rect.w,
		&label.render_rect.h
	); 

	return label;
}
ui_label ui_create_title_label(char* str, int x, int y)
{
	return ui_create_label_helper(str, x, y, font_info.title_font);
}
ui_label ui_create_header_label(char* str, int x, int y)
{
	return ui_create_label_helper(str, x, y, font_info.header_font);
}
ui_label ui_create_text_label(char* str, int x, int y)
{
	return ui_create_label_helper(str, x, y, font_info.text_font);
}

ui_button ui_create_button(int x, int y, int w, int h, char* str, void (*on_click)(void))
{
	ui_button button;

	button._inuse = 1;
	button.text_label = ui_create_text_label(str, x+5, y);
	button.click_box.x = x;
	button.click_box.y = y;
	button.click_box.w = w;
	button.click_box.h = h;
	button.on_click = on_click;

	return button;

}

void ui_label_render(ui_label* l)
{
	if (!l) {
		slog("ui_label_render received NULL ui_label*");
		return;
	}

	SDL_RenderCopy(
		gf2d_graphics_get_renderer(),
		l->texture,
		NULL,
		&l->render_rect
	);

	if (DEBUG){ gf2d_draw_rect(l->render_rect, vector4d(55, 255, 255, 255)); }
}

void ui_button_render(ui_button* b)
{
	if (!b)
	{
		slog("ui_button_render cannot render NULL ui_button*");
		return;
	}

	ui_label_render(&b->text_label);

	gf2d_draw_rect(b->click_box, vector4d(255, 255, 255, 255));
}

gamestate_id ui_button_listen(ui_button* b, Uint32 mouse_state, int mx, int my)
{
	if (!b)
	{
		slog("ui_button_listen received NULL ui_button*");
		return NONE;
	}
	if (b->click_timer > 0)
	{
		b->click_timer--;
		return NONE;
	}

	gamestate_id id;

	if (mouse_state == 1 &&
		mx > b->click_box.x &&
		mx < b->click_box.x + b->click_box.w &&
		my > b->click_box.y &&
		my < b->click_box.y + b->click_box.h)
	{
		id = ui_button_click(b);
		b->click_timer = 35;

		if (id) return id;
	}
	return NONE;
}
gamestate_id ui_button_listen_alone(ui_button* b) 
{
	int mx, my;
	Uint32 mouse_state = SDL_GetMouseState(&mx, &my);

	return ui_button_listen(b, mouse_state, mx, my);
}

gamestate_id ui_button_click(ui_button* b)
{
	if (!b)
	{
		slog("ui_button_click passed NULL ui_button*");
		return NONE;
	}
	else if (!b->on_click)
	{
		slog("ui_button_click passed ui_button* with no on_click function");
		return NONE;
	}
	
	return b->on_click(b);
}
