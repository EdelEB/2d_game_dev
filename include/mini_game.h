#ifndef __MINI_GAME_H__
#define __MINI_GAME_H__

#include <SDL.h>


typedef struct MINI_GAME
{
	char	*title;
	Uint8	code;
	void	(*start)();
	void	(*end)();
};

void mini_init(Uint8 code);


#endif