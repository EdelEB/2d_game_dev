#ifndef __MENU_CREW_SELECT_H__
#define __MENU_CREW_SELECT_H__

#include "menu.h"
#include "crew.h"

// this is the only way it worked. I don't know why
struct MENU* menu_crew_select_init(void);

void next_crew_member(void);

void select_crew_member(void);


#endif