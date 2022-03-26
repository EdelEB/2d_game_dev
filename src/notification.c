#include "notification.h"

extern Uint32 rest;
extern Uint32 REST_DELAY;

typedef struct {
	Uint32			max_notes;
	TTF_Font	   *title_font, *text_font;
	SDL_Color		font_color;
	Notification   *note_list;
}NoteManager;

NoteManager note_manager = { 0 };

void note_manager_init(Uint32 max_notes)
{
	if (max_notes == 0)
	{
		slog("Cannot allocate memory for zero notes. Stop wasting my time");
		return;
	}
	//if (note_manager.note_list)
	//{
	//	slog("note manager already initialized dumb dumb");
	//}
	note_manager.max_notes = max_notes;
	note_manager.note_list = gfc_allocate_array(sizeof(Notification), max_notes);
	note_manager.title_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 80);
	note_manager.text_font = TTF_OpenFont("assets/fonts/Swansea.ttf", 30);
	SDL_Color temp = { 255,255,255,255 };
	note_manager.font_color = temp;

	atexit(note_manager_close);
	slog("note manager initialized");

	add_all_notes();
	note_create_all_render_variables();
}

void note_manager_close(void)
{
	slog("this should close note_manger but it doesn't");
}

Notification* note_new(void)
{
	Uint32 i;
	for (i = 0; i < note_manager.max_notes; i++)
	{
		if (note_manager.note_list[i]._inuse) continue;

		note_manager.note_list[i]._inuse = 1;
		return &note_manager.note_list[i];
	}
	return NULL;
}

Notification* get_note_by_id(gamestate_id id)
{
	Uint32 i;
	for (i = 0; i < note_manager.max_notes; i++)
	{
		if (note_manager.note_list[i]._inuse && 
			note_manager.note_list[i].id == id)
		{
			return &note_manager.note_list[i];
		}
	}
	return NULL;
}

void note_log_all(void)
{
	Uint32 i;
	for (i = 0; i < note_manager.max_notes; i++)
	{
		if (!note_manager.note_list[i]._inuse) { continue; }

		note_log(&note_manager.note_list[i]);
	}
}

void note_log(Notification* n)
{
	if (!n)
	{
		slog("note_log cannot log NULL Notification*");
		return;
	}
	slog("NOTE LOG : Title - %s", n->title);
	for (int i = 0; i < MAX_LINES; i++)
	{
		slog("Line %i - %s", i, n->line_strings[i]);
	}
}

void note_draw(Notification* n)
{
	if (!n)
	{
		slog("note_draw recieved NULL Notification*");
		return;
	}

	SDL_RenderCopy(
		gf2d_graphics_get_renderer(), 
		n->title_texture, 
		NULL, 
		&n->title_rect
	);
	
	for (int i = 0; i < MAX_LINES; i++)
	{
		if (n->line_strings[i])
		{
			SDL_RenderCopy(
				gf2d_graphics_get_renderer(),
				n->line_textures[i], 
				NULL, 
				&n->line_rects[i]
			);
		}
	}
}

gamestate_id note_listen(Uint32 mouse_state)
{
	if (rest > 0)
	{
		rest--;
		return NONE;
	}

	if (mouse_state == 1) 
	{ 
		rest = REST_DELAY;
		return MAP; 
	}
	return NONE;
}

void note_create_render_variables(Notification *n)
{
	SDL_Surface* surface;

	/* Title */

	surface = TTF_RenderText_Solid(
		note_manager.title_font,
		n->title,
		note_manager.font_color
	);
	n->title_texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(),
		surface
	);
	n->title_rect.x = WINDOW_WIDTH >> 2;
	n->title_rect.y = WINDOW_HEIGHT >> 4;
	SDL_QueryTexture(n->title_texture, NULL, NULL, &n->title_rect.w, &n->title_rect.h); // I don't understand what this does but it doesn't render the text unless I do it
	
	/* Lines */
	for (int i = 0; i < MAX_LINES; i++)
	{
		if (!n->line_strings[i]) { break; }
		
		surface = TTF_RenderText_Solid(
			note_manager.text_font,
			n->line_strings[i],
			note_manager.font_color
		);
		n->line_textures[i] = SDL_CreateTextureFromSurface(
			gf2d_graphics_get_renderer(),
			surface
		);
		n->line_rects[i].x = WINDOW_WIDTH >> 4;
		n->line_rects[i].y = (WINDOW_HEIGHT >> 3) *(i+2);
		
		SDL_QueryTexture(
			n->line_textures[i], 
			NULL, 
			NULL, 
			&n->line_rects[i].w, 
			&n->line_rects[i].h
		);

	}
}

void note_create_all_render_variables(void)
{
	Uint32 i;
	for (i = 0; i < note_manager.max_notes; i++)
	{
		if (note_manager.note_list[i]._inuse)
		{
			note_create_render_variables(&note_manager.note_list[i]);
		}
	}
}

void add_all_notes(void)
{
	Notification* n;
	
	/*NOTE_CREW_DEATH*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = NOTE_CREW_DEATH;
		n->title = "Crew Death";
		n->line_strings[0] = "Unfortunately, a crew member has died.";
		n->line_strings[1] = "Go into 'crew view' to see who.";
		n->line_strings[2] = "You must be a terrible captain.";
		n->line_strings[3] = "";
	}

	/*SUCCESS_ASTEROID_DODGE*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = SUCCESS_ASTEROID_DODGE;
		n->title = "Success";
		n->line_strings[0] = "Congratulations. You have traversed the asteroids";
		n->line_strings[1] = "unscathed. Your crew is alive and the ship is in tact.";
		n->line_strings[2] = "";
		n->line_strings[3] = "";
	}

	/*FAIL_ASTEROID_DODGE*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = FAIL_ASTEROID_DODGE;
		n->title = "Failure";
		n->line_strings[0] = "An asteroid broke a hole in the side of the ship.";
		n->line_strings[1] = "Before the airlock could close, a crew member was sucked";
		n->line_strings[2] = "into space. Check 'crew view' to see who you lost";
		n->line_strings[3] = "";
	}

	/*AD_LONG_WAY*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = AD_LONG_WAY;
		n->title = "The Long Way";
		n->line_strings[0] = "You take your crew far off course in order to avoid the";
		n->line_strings[1] = "asteroid field. You make it safely, but at the cost of ";
		n->line_strings[2] = "food rations and morale due to a longer trip.";
		n->line_strings[3] = "";
	}

	/*SUCCESS_RATION_SPLIT*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = SUCCESS_RATION_SPLIT;
		n->title = "Success";
		n->line_strings[0] = "You managed to equally split the rations among your members.";
		n->line_strings[1] = "Everyone is full and satisfied.";
		n->line_strings[2] = "";
		n->line_strings[3] = "";
	}

	/*FAIL_RATION_SPLIT*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = FAIL_RATION_SPLIT;
		n->title = "Failure";
		n->line_strings[0] = "You are either blind or incapable of using a knife.";
		n->line_strings[1] = "Some crew member left this meal hungry.";
		n->line_strings[2] = "";
		n->line_strings[3] = "";
	}

	/*RS_OMNOMNOM*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = RS_OMNOMNOM;
		n->title = "OMNOMNOMNOM";
		n->line_strings[0] = "That was tasty.";
		n->line_strings[1] = "Your crew is hungry and has lowered morale.";
		n->line_strings[2] = "";
		n->line_strings[3] = "";
	}

	/*RS_FIGHT*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = RS_FIGHT;
		n->title = "Fight";
		n->line_strings[0] = "Turns out ships are not a great place for fighting.";
		n->line_strings[1] = "A crew member was thrown across the ship towards a pointy";
		n->line_strings[2] = "thing. Zero gravity didn't let him stop himself, so he was";
		n->line_strings[3] = "slowly impaled. Use 'crew view' to see who died.";
	}

	/*SUCCESS_MOUSE_HUNT*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = SUCCESS_MOUSE_HUNT;
		n->title = "Success";
		n->line_strings[0] = "You managed to kill the mouse. Your rations are safe.";
		n->line_strings[1] = "";
		n->line_strings[2] = "";
		n->line_strings[3] = "";
	}

	/*FAIL_MOUSE_HUNT*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = FAIL_MOUSE_HUNT;
		n->title = "Failure";
		n->line_strings[0] = "You have embarrassed yourself with your inability to handle";
		n->line_strings[1] = "a simple mouse. Your crew morale has dropped.";
		n->line_strings[2] = "";
		n->line_strings[3] = "";
	}

	/*MOUSE_FRIEND*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = MOUSE_FRIEND;
		n->title = "New Crew Member";
		n->line_strings[0] = "You managed to befriend the mouse, or so you thought.";
		n->line_strings[1] = "The little bugger bit you when you tried to pick it up.";
		n->line_strings[2] = "You became ill and died.";
		n->line_strings[3] = "";
	}

	/*MOUSE_TRAPPED*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = MOUSE_TRAPPED;
		n->title = "Mouse Trapped?";
		n->line_strings[0] = "The engineer's trap malfunctioned and killed the mouse";
		n->line_strings[1] = "instead of capturing it. Sadness.";
		n->line_strings[2] = "";
		n->line_strings[3] = "";
	}

	/*AI_ACCEPT_DEFEAT*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = AI_ACCEPT_DEFEAT;
		n->title = "All Hail Skynet";
		n->line_strings[0] = "After getting on your knees and praising the computer,";
		n->line_strings[1] = "it rerouted your ship and got you to Mars in record time.";
		n->line_strings[2] = "It turns out it never needed you after all. You feel like";
		n->line_strings[3] = "a worthless sack of meat.";
	}

	/*AI_PUNCH*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = AI_PUNCH;
		n->title = "<thump>";
		n->line_strings[0] = "After a quick punch the system resets and all is well...";
		n->line_strings[1] = "or so you thought";
		n->line_strings[2] = "The AI was prepared: the ship immediately self destructs";
		n->line_strings[3] = "killing all inside. Learn to code next time.";
	}

	/*AI_FIXED*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = AI_FIXED;
		n->title = "Insert Hollywood Code Montage";
		n->line_strings[0] = "Your super skilled amazing programmer dude plugged his";
		n->line_strings[1] = "laptop in and after a few clicks and wizardry that there";
		n->line_strings[2] = "is no time to explain, the AI is gone and the ship is back";
		n->line_strings[3] = "in your hands.";
	}

	/*RM_DEAL_WITH_IT*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = RM_DEAL_WITH_IT;
		n->title = "Hangry Crewmates";
		n->line_strings[0] = "In a blaze of hangry furry, your crew members begin";
		n->line_strings[1] = "to brawl. The fight leaves them even hungrier and ";
		n->line_strings[2] = "with a newfound distrust among members. Morale dips.";
		n->line_strings[3] = "";
	}

	/*RM_BLAME*/
	{
		n = note_new();
		if (!n) {
			slog("failed to create Notification in add_all_notes");
			return;
		}
		n->id = RM_BLAME;
		n->line_strings[0] = "You point to Ken, because he's Ken. The crew grabs him";
		n->line_strings[1] = "and throws him out the airlock. Ahh Ken.";
		n->line_strings[2] = "You didn't have a crew member named Ken? He was undercover.";
		n->line_strings[3] = "Don't worry; we got'm";
	}

}
