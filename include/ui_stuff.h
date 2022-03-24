#ifndef __UI_STUFF_H__
#define __UI_STUFF_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"


struct UI_FONT_INFO {
	TTF_Font	*title_font, *header_font, *text_font;
	SDL_Color	font_color;
}font_info;

typedef struct UI_LABEL {
	char*			str;
	SDL_Texture*	texture;
	SDL_Rect		render_rect;
}ui_label;

typedef struct UI_BUTTON{
	ui_label	text_label;
	SDL_Rect	click_box;
	void		(*on_click)();
}ui_button;

void ui_font_stuff_init();

ui_label ui_create_label_helper(char* str, int x, int y, TTF_Font* font);

ui_label ui_create_title_label(char* str, int x, int y);
ui_label ui_create_header_label(char* str, int x, int y);
ui_label ui_create_text_label(char* str, int x, int y);

ui_button ui_create_button(int x, int y, int w, int h, char* str, void (*on_click)() );

void ui_render_label(ui_label* l);

void ui_render_button(ui_button* b);

Uint8 ui_listen_button(ui_button* b, Uint32 mouse_state, int mx, int my);
Uint8 ui_listen_button_alone(ui_button* b);

void ui_click_button(ui_button* b);

#endif