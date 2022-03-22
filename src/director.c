#include "director.h"

typedef struct {
	MiniGame *current_mini, *asteroid_dodge, *ration_split, *mouse_hunt;
	SDL_Thread *mini_thread;
}mini_holder;

mini_holder minis = { 0 };

void director_init()
{
	minis.asteroid_dodge = mini_asteroid_init();
}

state_type get_state_type(game_state_id id)
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

void director_draw(game_state_id id)
{
	switch (get_state_type(id))
	{
		case MENU:
			break;
		case EVENT:
			event_draw( get_event_by_id(id) );
			break;
		case NOTE:
			note_draw(get_note_by_id(id));
			break;
		case MINI:
			if (minis.current_mini) {
				gf2d_sprite_draw_image(minis.current_mini->background, vector2d(0, 0));
			}
			entity_manager_draw_all();
			break;
		case NO_STATE:
			break;
		default:
			break;
	}
}

game_state_id director_think(game_state_id id, Uint32 mouse_state, int *mx, int *my)
{
	switch (get_state_type(id))
	{
		case MENU:
			break;
		
		case EVENT:
			return event_listen(get_event_by_id(id), mouse_state, mx, my);
		
		case NOTE:
			if (mouse_state == 1){ return MAP; }
			return NONE;

		case MINI:
			switch (id) {
				case MINI_ASTEROID_DODGE:
					slog("THINKING");
					minis.current_mini = minis.asteroid_dodge;
					break;
				case MINI_RATION_SPLIT:
					minis.current_mini = minis.ration_split;
					break;
				case MINI_MOUSE_HUNT:
					minis.current_mini = minis.mouse_hunt;
					break;
			}

			if (!minis.current_mini->is_running)
			{
				minis.mini_thread = SDL_CreateThread(
					minis.current_mini->run,
					"Asteroid Dodge Game Thread",
					minis.current_mini
				);
				
			}

			if (minis.current_mini->end_state) { return minis.current_mini->end_state; }
			entity_manager_think_all();
			break;

		case NO_STATE:
			break;
		default:
			break;
	}
	

}
