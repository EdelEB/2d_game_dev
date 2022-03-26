#ifndef __UI_STUFF_H__
#define __UI_STUFF_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"

#include "enum_declarations.h"

extern Uint8 DEBUG;
extern const Uint32 WINDOW_WIDTH, WINDOW_HEIGHT;
#define CLICK_TIMER 20

struct FONT_INFO{
	TTF_Font	*title_font, *header_font, *text_font;
	SDL_Color	font_color;
}font_info;

typedef struct UI_LABEL {
	Uint8			_inuse;
	SDL_Texture*	texture;
	SDL_Rect		render_rect;
}ui_label;

typedef struct UI_BUTTON{
	Uint8		_inuse;
	ui_label	text_label;
	SDL_Rect	click_box;
	Uint16		click_timer;
	gamestate_id (*on_click)(struct UI_BUTTON* self);
}ui_button;

/*
* @brief this initializes all information regarding fonts and colors used to create labels
*/
void ui_font_info_init(void);

/*
* @brief create a ui_label object which can be used to print text to the screen
* @param str is the text that will be displayed
* @param x is the x coordinate of the upper left corner of the text 
* @param y is the y coordinate of the upper left corner of the text
* @param font is the type of font that is printed. The creates pass this to the helper so that the programmer does not need to create of find the font
* @return ui_label structure
*/
ui_label ui_create_label_helper(char* str, int x, int y, TTF_Font* font);
ui_label ui_create_title_label(char* str, int x, int y);
ui_label ui_create_header_label(char* str, int x, int y);
ui_label ui_create_text_label(char* str, int x, int y);

/*
* @brief draws a passed in ui_label to the screen
* @param l is a pointer to the label being drawn
*/
void ui_label_render(ui_label* l);

/*
* @brief creates a new ui_button struct 
* @param x is the x	coordinate of the top left corner of the button's click_box
* @param y is the y coordinate of the top left corner of the button's click_box
* @param w is the width of the button's click box
* @param h is the height of the button's click box
* @param str is the text that is inside the button
* @param on_click is the function that is called when the button is clicked
* @return ui_button struct
*/ 
ui_button ui_create_button(int x, int y, int w, int h, char* str, void (*on_click)(void) );

/*
* @brief draws a passed in ui_button to the screen
* @param l is a pointer to the label being drawn
*/
void ui_button_render(ui_button* b);


/*
* @brief listens for when the button is clicked and calls the on_click function when it is
* @param b is the ui_button that is listening
* @param mouse_state is 1 when the mouse is clicked, 0 when not
* @param mx is the x coordinate of the mouse
* @param my is the y coordinate of the mouse
* @returns gamestate_id if it changes gamestate NONE otherwise
*/
gamestate_id ui_button_listen(ui_button* b, Uint32 mouse_state, int mx, int my);
gamestate_id ui_button_listen_alone(ui_button* b);


/*
* @brief calls the passed button's on_click function
* @param b is a pointer to the ui_button whose on_click function will be called
* @returns the gamestate_id return by b->on_click(b)
*/
gamestate_id ui_button_click(ui_button* b);

#endif