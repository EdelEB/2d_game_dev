#ifndef __MINI_RATION_SPLIT_H__
#define __MINI_RATION_SPLIT_H__

#include "mini_game.h"
#include "crew.h"

MiniGame* mini_ration_init();

void mini_ration_start(MiniGame* self);
 
void mini_ration_run(MiniGame* self);
 
void mini_ration_end(MiniGame* self);

#endif