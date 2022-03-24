#include "ui_stuff.h"

void ui_font_stuff_init()
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

ui_button ui_create_button(int x, int y, int w, int h, char* str, void (*on_click)())
{
	ui_button button;
	ui_label label;
	SDL_Rect click_box;

	label = ui_create_header_label(str, x, y);
	button.text_label = label;

	click_box.x = x;
	click_box.y = y;
	click_box.w = w;
	click_box.h = h;
	button.click_box = click_box;

	button.on_click = on_click;

	return button;

}

void ui_render_label(ui_label* l)
{
	SDL_RenderCopy(
		gf2d_graphics_get_renderer(),
		l->texture,
		NULL,
		&l->render_rect
	);
}

void ui_render_button(ui_button* b)
{
	if (!b)
	{
		slog("ui_render_button cannot render NULL ui_button*");
		return;
	}

	ui_render_label(&b->text_label);

	gf2d_draw_rect(b->click_box, vector4d(255, 255, 255, 255));
}

Uint8 ui_listen_button(ui_button* b, Uint32 mouse_state, int mx, int my)
{
	if (!b)
	{
		slog("ui_listen_button received NULL ui_button*");
		return 0;
	}

	if (mouse_state == 1 &&
		mx > b->click_box.x &&
		mx < b->click_box.x + b->click_box.w &&
		my > b->click_box.y &&
		my < b->click_box.y + b->click_box.h)
	{
		ui_click_button(b);
		return 1;
	}
	return 0;
}

Uint8 ui_listen_button_alone(ui_button* b) 
{
	int mx, my;
	Uint32 mouse_state = SDL_GetMouseState(&mx, &my);

	ui_listen_button(b, mouse_state, mx, my);
}

void ui_click_button(ui_button* b)
{
	if (!b)
	{
		slog("ui_click_button passed NULL ui_button*");
		return;
	}
	else if (!b->on_click)
	{
		slog("ui_click_button passed ui_button* with no on_click function");
		return;
	}

	b->on_click();
}
