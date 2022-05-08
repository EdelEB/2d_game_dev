#include "director.h"

struct MINI_HOLDER{
	MiniGame 
		*current_mini, 
		*asteroid_dodge, 
		*ration_split, 
		*mouse_hunt;
	SDL_Thread *mini_thread;
}mini_holder;

void director_init(void)
{
	init_crew_member_options();

	//event_manager_init(20);
	//note_manager_init(50);
	
	

	menu_manager_init(512);
	menu_start_init();
	menu_crew_select_init();
	
	// menu_crew_view is not initialized here because the crew needs to be selected first
	// therefore, it is initialized after load game is click in menu_start or after menu_crew_select is complete
	//event_menu_load_all("assets/json/events.json"); // events need to be loaded after crew is initialized
	
	note_menu_load_all("assets/json/notes.json");

	entity_manager_init(256);
	mini_holder.asteroid_dodge = mini_asteroid_init();
	mini_holder.mouse_hunt = mini_mouse_init();
	mini_holder.ration_split = mini_ration_init();
}

gamestate_id director_think(gamestate_id id, Uint32 mouse_state, int* mx, int* my, Uint8* keys)
{
	Menu* current_menu;

	if (id > THRESHOLD_MINI) {
		switch (id)
		{
			case MINI_ASTEROID_DODGE:
				mini_holder.current_mini = mini_holder.asteroid_dodge;
				break;
			case MINI_RATION_SPLIT:
				mini_holder.current_mini = mini_holder.ration_split;
				break;
			case MINI_MOUSE_HUNT:
				mini_holder.current_mini = mini_holder.mouse_hunt;
				break;
		}
		if (!mini_holder.current_mini) return NONE;

		if (!mini_holder.current_mini->is_running)
		{
			mini_holder.current_mini->start(mini_holder.current_mini);
			return NONE;
		}
		else
		{
			entity_manager_think_all();

			mini_holder.current_mini->run(mini_holder.current_mini);
			if (mini_holder.current_mini->end_state)
			{
				mini_end(mini_holder.current_mini);
				return mini_holder.current_mini->end_state;
			}
			return NONE;
		}
	}
	else
	{
		return menu_listen(menu_get_by_id(id), mouse_state, mx, my, keys);
	}
	return NONE;
}

void director_draw(gamestate_id id)
{
	if (id > THRESHOLD_MINI)
	{
		if (mini_holder.current_mini) 
		{
			gf2d_sprite_draw_image(mini_holder.current_mini->background, vector2d(0, 0));
		}
		if (id == MINI_RATION_SPLIT) 
		{
			mini_ration_draw(); // The background gets drawn over the UI without this. I will fix this later. It's also because ration_split has no entities
		}
		entity_manager_draw_all();
	}
	else
	{
		menu_draw(menu_get_by_id(id));
	}
}