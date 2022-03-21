#include "notification.h"


typedef struct {
	Uint32		max_notes;
	TTF_Font   *title_font, *text_font;
	SDL_Color	font_color;
	Notification *note_list;
}NoteManager;

NoteManager note_manager = { 0 };

void note_manager_init(Uint32 max_notes)
{
	if (max_notes == 0)
	{
		slog("Cannot allocate memory for zero notes. Stop wasting my time");
		return;
	}
	if (note_manager.note_list)
	{
		slog("note manager already initialized dumb dumb");
		return;
	}
	note_manager.max_notes = max_notes;
	note_manager.note_list = gfc_allocate_array(sizeof(Notification), max_notes);
	note_manager.title_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 80);
	note_manager.text_font = TTF_OpenFont("assets/fonts/Swansea.ttf", 30);
	SDL_Color temp = { 255,255,255,255 }; // Why is this necessary you ask? I don't know. I get an error if I do it directly without the temporary variable
	note_manager.font_color = temp;

	atexit(note_manager_close);
	slog("note manager initialized");
}

void note_manager_close() 
{
	slog("this should close note_manger but it doesn't");
}

Notification* note_new()
{
	int i;
	for (i = 0; i < note_manager.max_notes; i++)
	{
		if (note_manager.note_list[i]._inuse) continue;

		note_manager.note_list[i]._inuse = 1;
		return &note_manager.note_list[i];
	}
	return NULL;
}

void note_draw_by_id(game_state_id id)
{
	
	if (id > THRESHOLD_NOTE_START && id < THRESHOLD_DECISION_START)
	{
		note_draw( get_note_by_id(id) );
	}
	else
	{
		slog("note_draw_by_id received non-note event");
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
			n->line_strings[0],
			note_manager.font_color
		);
		n->title_texture = SDL_CreateTextureFromSurface(
			gf2d_graphics_get_renderer(),
			surface
		);
		n->line_rects[i].x = WINDOW_WIDTH >> 4;
		n->line_rects[i].y = (WINDOW_HEIGHT >> 3) *(i+1);
		SDL_QueryTexture(n->line_textures[i], NULL, NULL, &n->line_rects[i].w, &n->title_rect.h); // I don't understand what this does but it doesn't render the text unless I do it

	}
}

void note_create_all_render_variables()
{
	int i;
	for (int i = 0; i < note_manager.max_notes; i++)
	{
		if (note_manager.note_list[i]._inuse)
		{
			note_create_render_variables(&note_manager.note_list[i]);
		}
	}
}

Notification* get_note_by_id(game_state_id id)
{
	int i;
	for (i = 0; i < note_manager.max_notes; i++)
	{
		if (note_manager.note_list[i]._inuse && note_manager.note_list->id == id)
		{
			return &note_manager.note_list[i];
		}
	}
}

void add_all_notes()
{
	Notification* n = note_new();

	n->id = NOTE_RESULT;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	
	n = note_new();
	n->id = NOTE_CREW_DEATH;
	n->title = "Title Here";
	n->line_strings[0] = "Unfortunately, a crew member has died.";
	n->line_strings[1] = "Go into 'crew view' to see who.";
	n->line_strings[2] = "You must be a terrible captain.";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = SUCCESS_ASTEROID_DODGE;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";

	n = note_new();
	n->id = FAIL_ASTEROID_DODGE;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";

	n = note_new();
	n->id = AD_LONG_WAY;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";

	n = note_new();
	n->id = SUCCESS_RATION_SPLIT;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";

	n = note_new();
	n->id = FAIL_RATION_SPLIT;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";

	n = note_new();
	n->id = RS_OMNOMNOM;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = RS_FIGHT;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = SUCCESS_MOUSE_HUNT;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = FAIL_MOUSE_HUNT;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = MOUSE_FRIEND;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = MOUSE_TRAPPED;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = AI_ACCEPT_DEFEAT;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = AI_PUNCH;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = AI_FIXED;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = RM_DEAL_WITH_IT;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
	
	n = note_new();
	n->id = RM_BLAME;
	n->line_strings[0] = "";
	n->line_strings[1] = "";
	n->line_strings[2] = "";
	n->line_strings[3] = "";
}
