#ifndef __MENU_CREW_VIEW_H__
#define __MENU_CREW_VIEW_H__

#include "menu.h"
#include "crew.h"

struct MENU* menu_crew_view_init(void);

/*
* @brief updates the label for crew_member->hunger at gamestate.crew[i]
* @note that this is only intended for use inside menu_crew_view.c
*/
void update_crew_member_hunger(int i);

/*
* @brief this used as the on_click function for the buttons displayed
* @note this is not directly assigned because it takes a param
* @param i is the index of the crew_member in gamestate.crew
*/
void feed_crew_member(int i);

/*
* @brief this is used to assign a given function pointer to each button
* @note I believe there is better to do this but I don't know how to right now
* @param i is the index of the crew_member in gamestate.crew
* @returns a pointer to a function that runs feed_crew_member(i)
*/
void* get_feed_crew_member(int i);



#endif