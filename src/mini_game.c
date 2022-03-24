#include "mini_game.h"



void mini_start(MiniGame* self)
{

	slog("MINI START");
	if (!self)
	{
		slog("mini_start recieved NULL MiniGame*");
		return;
	}
	if (self->is_running)
	{
		slog("mini_start cannot start MiniGame already running");
		return;
	}
	if (!self->start)
	{
		slog("mini_start cannot start MiniGame with no start function");
		return;
	}
	
	self->is_running = 1;
	self->end_state  = NONE;
	//self->start(self);
}

void mini_run(MiniGame* self)
{
	if (!self)
	{
		slog("mini_run recieved NULL MiniGame*");
		return;
	}
	if (!self->is_running)
	{
		slog("Must call mini_start before mini_run. MiniGame* passed to mini_run is not running");
	}
	if (!self->run)
	{
		slog("mini_run passed a MiniGame * with no run() function");
		return PROBLEM_STATE;
	}

	self->run(self);
	if (self->end_state)
	{
		mini_end(self);
	}
	return;
}

void mini_end(MiniGame* self)
{
	int i;

	if (!self)
	{
		slog("mini_end recieved NULL MiniGame*");
		return;
	}

	self->is_running = 0;
	
	for (i = 0; i < MAX_MINI_ENTITIES; i++)
	{
		if (self->entities[i]->_inuse)
		{
			entity_free(self->entities[i]);
		}
	}
	
	if (self->end)
	{
		self->end(self);
	}
}