#ifndef __MENU_START_H__
#define __MENU_START_H__

#include "menu.h"
#include "gamestate.h"
#include "ui_stuff.h"
#include "menu_crew_view.h"

struct MENU* menu_start_init(void);

gamestate_id start_new_game(void);

gamestate_id load_game(void);

#endif