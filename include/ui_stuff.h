#ifndef __UI_STUFF_H__
#define __UI_STUFF_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "gfc_audio.h"

#include "enum_declarations.h"


#define WORD_LEN 16
#define SENTENCE_LEN 64
#define BLOCK_LEN 128
#define PAGE_LEN 512
#define TYPE_COOLDOWN 11
extern Uint8 DEBUG;
extern const Uint32 WINDOW_WIDTH, WINDOW_HEIGHT;
extern Uint8 global_was_mouse_down;
extern Uint8* global_prev_keys;
Uint16 type_cooldown;

typedef enum{
	LABEL,
	BUTTON,
	SPRITE,
	DRAGGABLE,
	TEXT_INPUT,
	SIZABLE
}ui_object_id;

typedef enum {
	TEXT,
	HEADER,
	TITLE
}ui_label_type;

/*FONT_INFO holds all fonts that intend to be used*/
/*This allows all fonts to loaded once*/
struct FONT_INFO{
	TTF_Font	*title_font, *header_font, *text_font;
	SDL_Color	font_color;
}font_info;

/*SOUND_FX holds all the Sounds that intend to be used*/
/*This allows all sounds to be loaded once*/
struct SOUND_FX {
	Sound	*button_click;
	Sound	*gamestate_change;
}sound_fx;

typedef struct UI_IMAGE {
	Uint8			_inuse;
	Sprite*			sprite;
	Vector2D		position, scale, scale_center;
	Vector3D		rotation;
}ui_image;

typedef struct UI_LABEL {
	Uint8			_inuse;			/**< 1 : being used, 0 : not being used*/
	ui_label_type	type;			/**< the type of label which determines the font used*/
	char*			str;			/**< the string being displayed*/
	SDL_Texture*	texture;		/**< texture created using SDL_ttf and is essential to rendering*/
	SDL_Rect		render_rect;	/**< this rect encapsulates the text_label in the texture and is necessary for rendering*/
	ui_image	   *image;			/**< this is the label image*/
}ui_label;

typedef struct UI_BUTTON{
	Uint8		_inuse;				/**< 1 : being used, 0 : not being used*/
	ui_label   *text_label;			/**< this is the a label holding the text_label seen within the button*/
	SDL_Rect	click_box;			/**< this box defines where the button bounds are (what counts as clicking the button)*/
	Uint8		hide_click_box;		/**< 0 : render click_box, 1 : do not render click_box*/
	ui_image   *image_default;		/**< the image displayed while a button is idle*/
	ui_image   *image_hover;		/**< the image displayed while a button is hovered over*/
	ui_image   *image_pressed;		/**< the image displayed while a button is pressed*/
	ui_image   *image_current;		/**< the image currently being rendered*/
	gamestate_id (*on_click)(struct UI_BUTTON* self); /**< gamestate_id returned by the function that is called when the function is pressed*/
}ui_button;

/* UI_DRAGGABLE is an object that can be clicked on and moved around the screen */
/* UI_DRAGGABLE's are intended to be added to other ui components and allow them to be moved*/
typedef struct UI_DRAGGABLE {
	Uint8		_inuse;				/**< 1 : being used, 0 : not being used*/
	Uint8		is_held;			/**< 1 : mouse is pressed and on draggable, 0 : otherwize*/
	Vector2D	mouse_anchor;		/**< this is the position the mouse was in when it clicked on the draggable*/
	Vector2D	position;			/**< this is the position the draggable is currently being rendered at*/
	Vector2D	prev_position;		/**< this is the position the draggable was at prior to being held*/
	SDL_Rect	click_box;			/**< this box defines where the draggable's bounds are*/
}ui_draggable;

/* UI_TEXT_INPUT consists of a text box that the user can type into and an enter button*/
typedef struct UI_TEXT_INPUT {
	Uint8		_inuse;
	Uint32		index;				/**< the index of the current str[i] that can typed into*/
	char		str[SENTENCE_LEN];	/**< string that is displayed in the box*/
	Vector2D	position;			/**< position of the text box */
	Uint8		is_active;			/**< 1 : when user can type, 0 : otherwize*/
	SDL_Rect	click_box;			/**< bounds of the text_label box, when clicked input box becomes active*/
	ui_label*	text_label;			/**< text_label inside the text_label box*/
	ui_button*	button_enter;		/**< Enter button next to the text_label box*/
}ui_text_input;

typedef struct UI_OBJECT {
	Uint8			_inuse;
	ui_object_id	id;

	ui_label* label;
	ui_button* b;
	ui_image* i;
	ui_text_input* t;
	ui_draggable* d;
}ui_object;


void ui_font_info_init(void);
void ui_sound_fx_init(void);

/*
* @brief initializes the ui_component manager which handles ui_labels, ui_buttons, ui_images, ui_draggables, etc.
* @param max_components is the maximum number of each individual ui component available
*/
void ui_manager_init(Uint32 max_components);

/*
* @brief runs the other init functions
*/
void ui_stuff_init(void);


void ui_font_info_close(void);
void ui_sound_fx_close(void);
void ui_manager_close(void);
void ui_stuff_close(void);

/*
* @brief calls the appropriate listen function based on object id
* @param o is the object being listened to
* @param mouse_state says if mouse is clicked ( 1:left pressed, 4: right pressed, 0: no press ) 
* @param mx	mouse x position
* @param my mouse y position
* @param keys is an array of Uint8 that can be used to get keyboard state using keys[<SCANCODE>]
* @returns gamestate_id of that the game needs to change to or gamestate NONE otherwize
*/
gamestate_id ui_object_listen(ui_object* o, Uint32 mouse_state, int mx, int my, Uint8* keys);

/*
* @brief calls the appropriate render function based on object id
* @param o is the object whose id is being used
*/
void ui_object_render(ui_object* o);


/*
* @brief returns the appropriate font from font_info for corresponding type
*/
TTF_Font* ui_get_font_by_type(ui_label_type type);

/*
* @brief create a ui_label object which can be used to print text_label to the screen
* @param str is the text_label that will be displayed
* @param x is the x coordinate of the upper left corner of the text_label
* @param y is the y coordinate of the upper left corner of the text_label
* @param type is used to determine the type of font that is printed
* @returns ui_object pointer with non-null ui_label attribute object->l
*/
ui_object* ui_create_label(char* str, int x, int y, ui_label_type type);

/*
* @brief updates a label to have a new_str
* @param l is the ui_label being modified
* @param new_str is the new ui_label->str attribute being set for l
*/
void ui_label_update(ui_label* l, char* new_str);

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
* @param str is the text_label that is inside the button
* @param on_click is the function that is called when the button is clicked
* @returns ui_object pointer with non-null ui_button attribute object->b
*/ 
ui_object* ui_create_button(int x, int y, int w, int h, char* str, void (*on_click)(void) );

void ui_button_set_images(ui_button* b, char* file_base_name, Vector2D scale, Vector2D scale_center, Vector3D rotation);


/*
* @brief listens for when the button is clicked and calls the on_click function when it is
* @param b is the ui_button that is listening
* @param mouse_state is 1 when the mouse is clicked, 0 when not
* @param mx is the x coordinate of the mouse
* @param my is the y coordinate of the mouse
* @returns gamestate_id if it changes gamestate NONE otherwise
*/
gamestate_id ui_button_listen(ui_button* b, Uint32 mouse_state, int mx, int my);

/*
* @brief this is intended to be used only during development when mouse input is not immediatley available where it is called
*/
gamestate_id ui_button_listen_alone(ui_button* b);

/*
* @brief calls the passed button's on_click function
* @param b is a pointer to the ui_button whose on_click function will be called
* @returns the gamestate_id return by b->on_click(b)
*/
gamestate_id ui_button_click(ui_button* b);

/*
* @brief draws a passed in ui_button to the screen
* @param l is a pointer to the label being drawn
*/
void ui_button_render(ui_button* b);



/*
* @brief creates a sprite object that can be used in tandum with other ui components or alone
* @param filename name of the image file to be loaded
* @param position (x, y) of the sprite in the window
* @param scale determines sizing of sprite from top left corner
* @param scale_center determines sizing from center of sprite
* @param rotation determines orientation of the imagae
* @param frame_count is the number of images to be cycled through in order to animate it
* @return  ui_object pointer with non-null ui_image attribute object->i
*/
ui_object* ui_create_image(char* filename, Vector2D position, Vector2D scale, Vector2D scale_center, Vector3D rotation);

/*
* @brief draws ui_image to the screen
* @param s is a pointer to the ui_image being drawn
*/
void ui_image_render(ui_image* image);



/*
* @brief creates a new ui_draggable and returns a pointer to it
* @param position (x,y) of draggable on screen (for top left corner of click_box)
* @param size (width, height) of draggable click box
* @return  ui_object pointer with non-null ui_draggable attribute object->d
*/
ui_object* ui_create_draggable(Vector2D position, Vector2D size);

/*
* @brief checks to see if the user has clicked on ui_draggable and if they have moved it, then updates accordingly
* @param d ui_draggable* being checked/updated
* @param mouse_state 0 : up, 1 : left down, 4 : right down
* @param mx mouse x coordinate
* @param my mouse y coordinate
*/
void ui_draggable_listen(ui_draggable* d, Uint32 mouse_state, int mx, int my);

/*
* @brief draws ui_draggable* d to the screen
* @param d ui_draggable* being drawn
*/
void ui_draggable_render(ui_draggable* d);



/*
* @brief creates a new ui_text_input and returns a pointer to it
* @param position is the location where the ui_text_input will be rendered (top left corner of the click_box)
* @param on_enter is a pointer to the function that will be called when the ui_text_input's button_enter is clicked
* @return  ui_object pointer with non-null ui_text_input attribute object->t
*/
ui_object* ui_create_text_input(Vector2D position, void (*on_enter)(void));

/*
* @brief listens for mouse and keyboard events in order to update the ui_text_input accordingly
* @param t is a pointer to the ui_text_input that will be listened for and updated if necessary
* @param mouse_state says if mouse is clicked ( 1:left pressed, 4: right pressed, 0: no press ) 
* @param mx	mouse x position
* @param my mouse y position
* @param keys is an array of Uint8 that can be used to get keyboard state using keys[<SCANCODE>]
* @return gamestate_id of that the game needs to change to or gamestate NONE otherwize
*/
gamestate_id ui_text_input_listen(ui_text_input* t, Uint32 mouse_state, int mx, int my, Uint8* keys);

/*
* @brief draws ui_text_input* to the screen
* @param t is the ui_text_input being drawn
*/
void ui_text_input_render(ui_text_input* t);



/* All "New" functions : allocate memory from managers */
ui_object* ui_object_new();
ui_label* ui_label_new(void);
ui_button* ui_button_new(void);
ui_image* ui_image_new(void);
ui_draggable* ui_draggable_new(void);
ui_text_input* ui_text_input_new(void);

/* All Free Functions */
void ui_object_free(ui_object* o);
void ui_label_free(ui_label* l);
void ui_button_free(ui_button* b);
void ui_image_free(ui_image* image);
void ui_draggable_free(ui_draggable* d);
void ui_text_input_free(ui_text_input* t);

#endif