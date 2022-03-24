#ifndef __MINI_GAME_H__
#define __MINI_GAME_H__

#include <SDL.h>

#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"

#include "simple_logger.h"

#include "tools.h"
#include "entity.h"
#include "enum_declarations.h"


extern const Uint32 WINDOW_WIDTH, WINDOW_HEIGHT;

#define MAX_MINI_ENTITIES 10


typedef struct MINI_GAME{
	char		   *title;			/**<name of the mini game*/
	Sprite		   *background;		/**<background image*/
	gamestate_id	id;				/**<number that represents the game name(for rules)*/
	gamestate_id	end_state;		/**<the id of the state after the game ends (this is changed to different id's depending on a  win or loss)*/
	Uint8			is_running;		/**<true when mini game is being played, false when closed*/
	struct ENTITY_S*	entities[MAX_MINI_ENTITIES];
	void			(*start)(struct MINI_GAME* self);
	void			(*run)(struct MINI_GAME* self); /**<function starts and renders game*/
	void			(*end)(struct MINI_GAME* self); /**<function ends the game and clears it from the screen*/
}MiniGame;

void mini_start(MiniGame* self);

void mini_run(MiniGame* self);

void mini_end(MiniGame* self);

#endif