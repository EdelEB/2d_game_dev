#include "director.h"

struct MINIHOLDER{
	MiniGame *current_mini, *asteroid_dodge, *ration_split, *mouse_hunt;
	SDL_Thread *mini_thread;
}mini_holder;

void director_init()
{
	map_init();
	mini_holder.asteroid_dodge = mini_asteroid_init();
	//mini_holder.mouse_hunt = mini_mouse_init();
}

state_type get_state_type(gamestate_id id)
{
	if (id > THRESHOLD_MINI){
		return MINI;	// MINIGAME
	}
	else if (id > THRESHOLD_DECISION_START){
		return EVENT;	// DECISION EVENT
	}
	else if (id > THRESHOLD_NOTE_START) {
		return NOTE;	// NOTIFICATION EVENT
	}
	else if (id) {
		return MENU;	// MENU
	}
	else {
		return NO_STATE;	// NONE
	}
}

void director_draw(gamestate_id id)
{
	switch (get_state_type(id))
	{
		case MENU:
			if (id == MAP) {
				map_draw();
			}
			break;
		case EVENT:
			event_draw( get_event_by_id(id) );
			break;
		case NOTE:
			note_draw( get_note_by_id(id) );
			break;
		case MINI:
			if (mini_holder.current_mini) {
				gf2d_sprite_draw_image(mini_holder.current_mini->background, vector2d(0, 0));
			}
			entity_manager_draw_all();
			break;
		case NO_STATE:
			break;
		default:
			break;
	}
}

gamestate_id director_think(gamestate_id id, Uint32 mouse_state, int *mx, int *my)
{
	switch (get_state_type(id))
	{
		case MENU:
			if (MAP) { return NONE; } 
			break;
		
		case EVENT:
			return event_listen(get_event_by_id(id), mouse_state, mx, my);
		
		case NOTE:
			return note_listen(mouse_state);

		case MINI:
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
					return mini_holder.current_mini->end_state;
				}
				return NONE;
			}
			break; // This should never be hit

		case NO_STATE:
			break;
		default:
			break;
	}
	

}
