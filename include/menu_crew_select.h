#ifndef __MENU_CREW_SELECT_H__
#define __MENU_CREW_SELECT_H__

#include "menu.h"
#include "crew.h"
#include "menu.h"

// "struct MENU*" instead of "Menu*" is the only way this works. I don't know why
struct MENU* menu_crew_select_init(void);

/*
* @brief goes to the next crew member in options and updates the menu labels to reflect it in the render
* @returns gamestate_id NONE always
*/
gamestate_id next_crew_member(void);


/*
* @brief adds current crew member displayed on screen to current_crew
* @returns gamestate_id CREW_VIEW when max crew members is reached, NONE otherwise
*/
gamestate_id select_crew_member(void);


#endif