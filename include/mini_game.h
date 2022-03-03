#ifndef __MINI_GAME_H__
#define __MINI_GAME_H__

#include <SDL.h>


typedef enum {
	NO_GAME,
	ASTEROID_DODGE,
	MOUSE_HUNT,
	RATION_CUT
}mini_code;

typedef struct MINI_GAME
{
	char		*title;				/**<name of the mini game*/
	Sprite		background;			/**<background image*/
	mini_code	code;				/**<number that represents the game*/
	Uint8		is_running;			/**<true when mini game is being played, false when closed*/
	void		(*mini_init)();		/**<function creates instance renders game*/
	void		(*end)();			/**<function clears the game*/
}MiniGame;

MiniGame* mini_new();

void mini_free();

#endif