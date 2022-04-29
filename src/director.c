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

	code_vomit_create_crew_member_options();

	event_manager_init(20);
	note_manager_init(50);
	
	map_init();

	menu_manager_init(1024);
	menu_start_init();
	menu_crew_select_init();
	// menu_crew_view is not initialized here because the crew needs to be selected first
	// therefore, it is initialized after load game is click in menu_start or after menu_crew_select is complete
	
	entity_manager_init(1024);
	mini_holder.asteroid_dodge = mini_asteroid_init();
	mini_holder.mouse_hunt = mini_mouse_init();
	mini_holder.ration_split = mini_ration_init();
}

gamestate_id director_think(gamestate_id id, Uint32 mouse_state, int *mx, int *my)
{
	Menu* current_menu;

	switch (get_state_type(id))
	{
		case MENU:
			switch(id) 
			{
				case MAP: 
					return map_listen(mouse_state, *mx, *my); 
				default:
					return menu_listen(menu_get_by_id(id), mouse_state, mx, my);
			}
			slog("director_think miss 1");
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
					mini_end(mini_holder.current_mini);
					return mini_holder.current_mini->end_state;
				}
				return NONE;
			}
			slog("director_think miss 2");
			break; // This should never be hit
		case NO_STATE:
			break;
		default:
			break;
	}

	return PROBLEM_STATE;
}

void director_draw(gamestate_id id)
{
	switch (get_state_type(id))
	{
	case MENU:
		switch (id)
		{ 
			case MAP:
				map_draw();
				break;
			default:
				return menu_draw(menu_get_by_id(id));
		}
		break;
	case EVENT:
		event_draw(get_event_by_id(id));
		break;
	case NOTE:
		note_draw(get_note_by_id(id));
		break;
	case MINI:
		if (mini_holder.current_mini) {
			gf2d_sprite_draw_image(mini_holder.current_mini->background, vector2d(0, 0));
		}
		if (id == MINI_RATION_SPLIT) {
			mini_ration_draw(); // The background gets drawn over the UI without this. I will fix this later. It's also because ration_split has no entities
		}
		entity_manager_draw_all();
		break;
	case NO_STATE:
		break;
	default:
		break;
	}
}

state_type get_state_type(gamestate_id id)
{
	if (id > THRESHOLD_MINI) {
		return MINI;	// MINIGAME
	}
	else if (id > THRESHOLD_DECISION_START) {
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