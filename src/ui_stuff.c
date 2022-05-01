#include "ui_stuff.h"

extern Uint8 global_was_mouse_down;

typedef struct UI_MANAGER {
	Uint32 max_components;
	ui_label* label_list;
	ui_button* button_list;
	ui_sprite* sprite_list;
	ui_draggable* draggable_list;
	ui_text_input* text_input_list;
}UI_Manager;

UI_Manager ui_manager = { 0 };

void ui_manager_init(Uint32 max_components) 
{
	if (max_components == 0)
	{
		slog("ui_manager_init cannot allocate 0 max_components");
		return;
	}
	ui_manager.max_components = max_components;
	ui_manager.label_list = gfc_allocate_array(sizeof(ui_label), max_components);
	ui_manager.button_list = gfc_allocate_array(sizeof(ui_button), max_components);
	ui_manager.sprite_list = gfc_allocate_array(sizeof(ui_sprite), max_components);
	ui_manager.draggable_list = gfc_allocate_array(sizeof(ui_draggable), max_components);
	ui_manager.text_input_list = gfc_allocate_array(sizeof(ui_text_input), max_components);
}
void ui_font_info_init(void)
{
	font_info.title_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 80);
	font_info.header_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 35);
	font_info.text_font = TTF_OpenFont("assets/fonts/Swansea.ttf", 30);
	SDL_Color temp = { 255,255,255,255 }; // Why is this necessary you ask? I don't know. I get an error if I do it directly without the temporary variable
	font_info.font_color = temp;
}
void ui_sound_fx_init(void)
{
	sound_fx.button_click = gfc_sound_load("assets/sound/sound_click2.wav", 1, 2);
	if (sound_fx.button_click == NULL)slog("FUUUK");
	sound_fx.gamestate_change = gfc_sound_load("assets/sound/sound_click.wav", 1, 2);
	if (sound_fx.gamestate_change == NULL)slog("Fuk2");
}
void ui_stuff_init(void)
{
	ui_manager_init(512);
	ui_font_info_init();
	ui_sound_fx_init();
}

void ui_font_info_close(void)
{
	TTF_CloseFont(font_info.title_font);
	TTF_CloseFont(font_info.header_font);
	TTF_CloseFont(font_info.text_font);
	free(&font_info.font_color);
}
void ui_sound_fx_close(void)
{
	gfc_sound_free(sound_fx.button_click);
	gfc_sound_free(sound_fx.gamestate_change);
}
void ui_manager_clear(void)
{
	int i;
	for (i = 0; i < ui_manager.max_components; i++)
	{
		if (ui_manager.label_list[i]._inuse) { 
			ui_label_free(&ui_manager.label_list[i]); 
		}
		if (ui_manager.button_list[i]._inuse) {
			ui_button_free(&ui_manager.button_list[i]);
		}
		if (ui_manager.sprite_list[i]._inuse) {
			ui_sprite_free(&ui_manager.sprite_list[i]);
		}
		if (ui_manager.draggable_list[i]._inuse) {
			ui_draggable_free(&ui_manager.draggable_list[i]);
		}
		if (ui_manager.text_input_list[i]._inuse) {
			ui_text_input_free(&ui_manager.text_input_list[i]);
		}
	}
}
void ui_manager_close(void)
{
	ui_manager_clear();

	if (ui_manager.label_list) free(ui_manager.label_list);
	if (ui_manager.button_list) free(ui_manager.button_list);
	if (ui_manager.sprite_list) free(ui_manager.sprite_list);
	if (ui_manager.draggable_list) free(ui_manager.draggable_list);
	if (ui_manager.text_input_list) free(ui_manager.text_input_list);
	
	slog("ui_manager closed");
}
void ui_stuff_close(void)
{
	ui_font_info_close();
	ui_sound_fx_close();
	ui_manager_close();
}


ui_label* ui_create_label_helper(char* str, int x, int y, TTF_Font* font)
{
	SDL_Surface* surface;
	ui_label* label = ui_label_new();
	if (!label) {
		slog("ui_create_label_helper failed to retrieve ui_label pointer");
		return;
	}

	label->str = str;

	surface = TTF_RenderText_Solid(
		font,
		str,
		font_info.font_color
	);
	if (!surface) {
		slog("ui_create_label_helper failed to create SDL_Surface with TTF_RenderText_Solid");
		return NULL;
	}

	label->texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(),
		surface
	);
	label->render_rect.x = x;
	label->render_rect.y = y;
	label->render_rect.w = 200;
	label->render_rect.h = 200;
	SDL_QueryTexture(
		label->texture,
		NULL,
		NULL,
		&label->render_rect.w,
		&label->render_rect.h
	); 

	return label;
}
ui_label* ui_create_title_label(char* str, int x, int y)
{
	return ui_create_label_helper(str, x, y, font_info.title_font);
}
ui_label* ui_create_header_label(char* str, int x, int y)
{
	return ui_create_label_helper(str, x, y, font_info.header_font);
}
ui_label* ui_create_text_label(char* str, int x, int y)
{
	return ui_create_label_helper(str, x, y, font_info.text_font);
}

void ui_label_update(ui_label* l, char* new_str)
{
	SDL_Surface* surface;

	if (!l ) return;

	surface = TTF_RenderText_Solid(
		font_info.text_font,
		new_str,
		font_info.font_color
	);
	if (!surface) {
		slog("ui_create_label_helper failed to create SDL_Surface with TTF_RenderText_Solid");
		return NULL;
	}

	l->texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(),
		surface
	);
	SDL_QueryTexture(
		l->texture,
		NULL,
		NULL,
		&l->render_rect.w,
		&l->render_rect.h
	);
}

void ui_label_render(ui_label* l)
{
	if (!l) {
		slog("ui_label_render received NULL ui_label*");
		return;
	}

	if (l->sprite) {
		ui_sprite_render(l->sprite);
	}

	SDL_RenderCopy(
		gf2d_graphics_get_renderer(),
		l->texture,
		NULL,
		&l->render_rect
	);

	if (DEBUG){ gf2d_draw_rect(l->render_rect, vector4d(55, 255, 255, 255)); }
}


ui_button* ui_create_button(int x, int y, int w, int h, char* str, void (*on_click)(void))
{
	ui_button* button = ui_button_new();
	if (!button) {
		slog("ui_create_button failed to retrieve button pointer");
		return NULL;
	}

	button->text_label = ui_create_text_label(str, x + 10, y + 10);
	button->click_box.x = x;
	button->click_box.y = y;
	button->click_box.w = w;
	button->click_box.h = h;
	button->on_click = on_click;

	return button;

}

void ui_button_render(ui_button* b)
{
	if (!b)
	{
		slog("ui_button_render cannot render NULL ui_button*");
		return;
	}
	
	if (!b->hide_click_box) { gf2d_draw_rect(b->click_box, vector4d(255, 255, 255, 255)); }
	
	if (b->sprite_current) { ui_sprite_render(b->sprite_current); }
	
	ui_label_render(b->text_label);
}

gamestate_id ui_button_listen(ui_button* b, Uint32 mouse_state, int mx, int my)
{
	gamestate_id id;

	if (!b)
	{
		slog("ui_button_listen received NULL ui_button*");
		return NONE;
	}

	/* Is mouse over button b */
	if (mx > b->click_box.x &&
		mx < b->click_box.x + b->click_box.w &&
		my > b->click_box.y &&
		my < b->click_box.y + b->click_box.h) {

		if (mouse_state == 1) 
		{
			b->sprite_current = b->sprite_pressed;
		}
		else if (global_was_mouse_down == 1)
		{
			global_was_mouse_down = 0;

			id = ui_button_click(b);
			if (id) return id;
		}
		else 
		{
			b->sprite_current = b->sprite_hover;
		}
	}
	else 
	{
		b->sprite_current = b->sprite_default;
	}
	return NONE;
}
gamestate_id ui_button_listen_alone(ui_button* b) 
{
	int mx, my;
	Uint32 mouse_state = SDL_GetMouseState(&mx, &my);

	return ui_button_listen(b, mouse_state, mx, my);
}

gamestate_id ui_button_click(ui_button* b)
{
	gamestate_id ret;

	if (!b)
	{
		slog("ui_button_click passed NULL ui_button*");
		return NONE;
	}
	else if (!b->on_click)
	{
		slog("ui_button_click passed ui_button* with no on_click function");
		return NONE;
	}
	
	gfc_sound_play(sound_fx.button_click, 0, 0.3, -1, -1);
	
	ret = b->on_click(b);
	if (ret != NONE) {
		gfc_sound_play(sound_fx.gamestate_change, 0, 0.3, -1, -1);
	}
	return ret;
}

ui_sprite* ui_create_sprite(Sprite* sprite, Vector2D	position, Vector2D scale, Vector2D scale_center, Vector3D rotation, Uint32 frame_count)
{
	ui_sprite* s = ui_sprite_new();
	if (!s) {
		slog("ui_create_sprite failed to retrieve a ui_sprite pointer");
		return NULL;
	}

	s->sprite = sprite;
	vector2d_copy(s->position, position);
	vector2d_copy(s->scale, scale);
	vector2d_copy(s->scale_center, scale_center);
	vector3d_copy(s->rotation, rotation);
	s->frame_count = frame_count;

	return s;
}

void ui_sprite_render(ui_sprite* s)
{
	if (!s) { slog("ui_sprite_render cannot render NULL ui_sprite*"); return; }

	if (s->frame_count == 1) {
		gf2d_sprite_draw(
			s->sprite, 
			s->position, 
			&s->scale, 
			&s->scale_center, 
			&s->rotation,
			NULL,
			NULL,
			0
			);
	}
}

ui_draggable* ui_create_draggable(Vector2D position, Vector2D size)
{
	ui_draggable* d = ui_draggable_new();
	if(!d){
		slog("ui_create_draggable faile to retrieve new ui_draggable*");
		return;
	}

	d->is_held = 0;
	vector2d_copy(d->position, position);
	vector2d_copy(d->prev_position, position);
	d->click_box.x = position.x;
	d->click_box.y = position.y;
	d->click_box.w = size.x;
	d->click_box.h = size.y;

	return d;
}

void ui_draggable_listen(ui_draggable* d, Uint32 mouse_state, int mx, int my)
{
	if (mx > d->click_box.x &&
		mx < d->click_box.x + d->click_box.w &&
		my > d->click_box.y &&
		my < d->click_box.y + d->click_box.h) {

		if (mouse_state == 1 && global_was_mouse_down == 0) 
		{
			global_was_mouse_down = 1;

			d->is_held = 1;
			vector2d_copy(d->mouse_anchor, vector2d(mx,my));
		}
		else if (mouse_state == 0 && global_was_mouse_down == 1)
		{
			d->is_held = 0;
			global_was_mouse_down = 0;
			vector2d_copy(d->prev_position, d->position);
		}
		else if(d->is_held)
		{
			d->position.x	= d->prev_position.x - (d->mouse_anchor.x - mx);
			d->position.y	= d->prev_position.y - (d->mouse_anchor.y - my);
			d->click_box.x	= d->prev_position.x - (d->mouse_anchor.x - mx);
			d->click_box.y	= d->prev_position.y - (d->mouse_anchor.y - my);
		}
	}
}

void ui_draggable_render(ui_draggable* d)
{
	if (!d)
	{
		slog("ui_draggable_render cannot render NULL ui_draggable*");
		return;
	}
	gf2d_draw_rect(d->click_box, vector4d(255, 255, 255, 255));
	//ui_label_render(&d->text_label);
}


ui_text_input* ui_create_text_input(Vector2D position,void (*on_enter)(void))
{
	ui_text_input* t = ui_text_input_new();
	if (!t) { 
		slog("ui_create_text_input could not retrieve ui_text_input pointer"); 
		return; 
	}
	
	t->text_label = ui_create_text_label("w",position.x, position.y);
	vector2d_copy(t->position, position);
	t->click_box.x = position.x;
	t->click_box.y = position.y;
	t->click_box.w = WINDOW_WIDTH - 400;
	t->click_box.h = 40;
	t->button_enter = ui_create_button(
		t->click_box.x + t->click_box.w,
		t->click_box.y,
		150,
		40,
		"Enter",
		on_enter
		);

	return t;
}

gamestate_id ui_text_input_listen(ui_text_input* t, Uint32 mouse_state, int mx, int my, Uint8* keys)
{
	gamestate_id id;
	char str[128];

	if (!t) return; 
	
	if (global_was_mouse_down == 1 && mouse_state == 0)
	{
		if (mx > t->click_box.x &&
			mx < t->click_box.x + t->click_box.w &&
			my > t->click_box.y &&
			my < t->click_box.y + t->click_box.h)
		{
			global_was_mouse_down = 0;
			t->is_active = 1;
		}
		else /*clicked outside the box*/
		{
			t->is_active = 0;
		}
	}

	if (keys[SDL_SCANCODE_O])
	{
		ui_label_update(t->text_label, "hmmmm");
	}
	/*
	if(keys[SDL_SCANCODE_RSHIFT] || keys[SDL_SCANCODE_LSHIFT])
	{
		if (keys[SDL_SCANCODE_Q]) { sprintf(str, t->text_label->str, 'Q'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_W]) { sprintf(str, t->text_label->str, 'W'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_E]) { sprintf(str, t->text_label->str, 'E'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_R]) { sprintf(str, t->text_label->str, 'R'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_T]) { sprintf(str, t->text_label->str, 'T'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_Y]) { sprintf(str, t->text_label->str, 'Y'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_U]) { sprintf(str, t->text_label->str, 'U'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_I]) { sprintf(str, t->text_label->str, 'I'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_O]) { sprintf(str, t->text_label->str, 'O'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_P]) { sprintf(str, t->text_label->str, 'P'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_A]) { sprintf(str, t->text_label->str, 'A'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_S]) { sprintf(str, t->text_label->str, 'S'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_D]) { sprintf(str, t->text_label->str, 'D'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_F]) { sprintf(str, t->text_label->str, 'F'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_G]) { sprintf(str, t->text_label->str, 'G'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_H]) { sprintf(str, t->text_label->str, 'H'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_J]) { sprintf(str, t->text_label->str, 'J'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_K]) { sprintf(str, t->text_label->str, 'K'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_L]) { sprintf(str, t->text_label->str, 'L'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_Z]) { sprintf(str, t->text_label->str, 'Z'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_X]) { sprintf(str, t->text_label->str, 'X'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_C]) { sprintf(str, t->text_label->str, 'C'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_V]) { sprintf(str, t->text_label->str, 'V'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_B]) { sprintf(str, t->text_label->str, 'B'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_N]) { sprintf(str, t->text_label->str, 'N'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_M]) { sprintf(str, t->text_label->str, 'M'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_MINUS]) { sprintf(str, t->text_label->str, '_'); ui_label_update(t->text_label, str); }
	}
	else
	{
		if (keys[SDL_SCANCODE_Q]) { sprintf(str, t->text_label->str, 'q'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_W]) { sprintf(str, t->text_label->str, 'w'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_E]) { sprintf(str, t->text_label->str, 'e'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_R]) { sprintf(str, t->text_label->str, 'r'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_T]) { sprintf(str, t->text_label->str, 't'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_Y]) { sprintf(str, t->text_label->str, 'y'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_U]) { sprintf(str, t->text_label->str, 'u'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_I]) { sprintf(str, t->text_label->str, 'i'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_O]) { sprintf(str, t->text_label->str, 'o'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_P]) { sprintf(str, t->text_label->str, 'p'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_A]) { sprintf(str, t->text_label->str, 'a'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_S]) { sprintf(str, t->text_label->str, 's'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_D]) { sprintf(str, t->text_label->str, 'd'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_F]) { sprintf(str, t->text_label->str, 'f'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_G]) { sprintf(str, t->text_label->str, 'g'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_H]) { sprintf(str, t->text_label->str, 'h'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_J]) { sprintf(str, t->text_label->str, 'j'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_K]) { sprintf(str, t->text_label->str, 'k'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_L]) { sprintf(str, t->text_label->str, 'l'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_Z]) { sprintf(str, t->text_label->str, 'z'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_X]) { sprintf(str, t->text_label->str, 'x'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_C]) { sprintf(str, t->text_label->str, 'c'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_V]) { sprintf(str, t->text_label->str, 'v'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_B]) { sprintf(str, t->text_label->str, 'b'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_N]) { sprintf(str, t->text_label->str, 'n'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_M]) { sprintf(str, t->text_label->str, 'm'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_0]) { sprintf(str, t->text_label->str, '0'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_1]) { sprintf(str, t->text_label->str, '1'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_2]) { sprintf(str, t->text_label->str, '2'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_3]) { sprintf(str, t->text_label->str, '3'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_4]) { sprintf(str, t->text_label->str, '4'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_5]) { sprintf(str, t->text_label->str, '5'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_6]) { sprintf(str, t->text_label->str, '6'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_7]) { sprintf(str, t->text_label->str, '7'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_8]) { sprintf(str, t->text_label->str, '8'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_9]) { sprintf(str, t->text_label->str, '9'); ui_label_update(t->text_label, str); }
		else if (keys[SDL_SCANCODE_DELETE]) {}
		else if (keys[SDL_SCANCODE_SPACE]) {}
	}
	*/
	return ui_button_listen(t->button_enter, mouse_state, mx, my);
}

void ui_text_input_render(ui_text_input* t) 
{
	if (!t)
	{
		slog("ui_text_input_render recieved NULL ui_text_input pointer");
		return;
	}

	if (t->is_active) {
		gf2d_draw_rect(t->click_box, vector4d(0, 255, 255, 255));
	}
	else {
		gf2d_draw_rect(t->click_box, vector4d(255, 255, 255, 255));
	}
	
	ui_button_render(t->button_enter);
	ui_label_render(t->text_label);
}

ui_label* ui_label_new(void)
{
	int i;
	for (i = 0; i < ui_manager.max_components; i++)
	{
		if (ui_manager.label_list[i]._inuse) continue;
		ui_manager.label_list[i]._inuse = 1;
		return &ui_manager.label_list[i];
	}
	return NULL;
}
ui_button* ui_button_new(void)
{
	int i;
	for (i = 0; i < ui_manager.max_components; i++)
	{
		if (ui_manager.button_list[i]._inuse) continue;
		ui_manager.button_list[i]._inuse = 1;
		return &ui_manager.button_list[i];
	}
	return NULL;
}
ui_sprite* ui_sprite_new(void)
{
	int i;
	for (i = 0; i < ui_manager.max_components; i++)
	{
		if (ui_manager.sprite_list[i]._inuse) continue;
		ui_manager.sprite_list[i]._inuse = 1;
		return &ui_manager.sprite_list[i];
	}
	return NULL;
}
ui_draggable* ui_draggable_new(void)
{
	int i;
	for (i = 0; i < ui_manager.max_components; i++)
	{
		if (ui_manager.draggable_list[i]._inuse) continue;
		ui_manager.draggable_list[i]._inuse = 1;
		return &ui_manager.draggable_list[i];
	}
	return NULL;
}
ui_text_input* ui_text_input_new(void)
{
	int i;
	for (i = 0; i < ui_manager.max_components; i++)
	{
		if (ui_manager.text_input_list[i]._inuse) continue;
		ui_manager.text_input_list[i]._inuse = 1;
		return &ui_manager.text_input_list[i];
	}
	return NULL;
}

void ui_label_free(ui_label* l)
{
	if (!l) { return; }
	ui_sprite_free(l->sprite);
	memset(l, 0, sizeof(ui_label));
}
void ui_button_free(ui_button* b)
{
	if (!b) return;
	ui_label_free(b->text_label);
	ui_sprite_free(b->sprite_default);
	ui_sprite_free(b->sprite_hover);
	ui_sprite_free(b->sprite_pressed);
	memset(b, 0, sizeof(ui_button));
}
void ui_sprite_free(ui_sprite* s)
{
	if (!s) return;
	gf2d_sprite_free(s->sprite);
	memset(s, 0, sizeof(ui_sprite));
}
void ui_draggable_free(ui_draggable* d)
{
	if (!d) return;
	memset(d, 0, sizeof(ui_draggable));
}
void ui_text_input_free(ui_text_input* t)
{
	if (!t) return;

	ui_button_free(t->button_enter);
	ui_label_free(t->text_label);
	memset(t, 0, sizeof(ui_text_input));
}