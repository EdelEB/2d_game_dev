#include "ui_stuff.h"

Uint8 was_mouse_pressed = 0;

typedef struct UI_MANAGER {
	Uint32 max_components;
	ui_label* label_list;
	ui_button* button_list;
	ui_sprite* sprite_list;
	ui_draggable* draggable_list;
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
	}
}

void ui_manager_close(void)
{
	ui_manager_clear();

	if (ui_manager.label_list) free(ui_manager.label_list);
	if (ui_manager.button_list) free(ui_manager.button_list);
	if (ui_manager.sprite_list) free(ui_manager.sprite_list);
	if (ui_manager.draggable_list) free(ui_manager.draggable_list);
	
	slog("ui_manager closed");
}

void ui_stuff_close(void)
{
	ui_font_info_close();
	ui_sound_fx_close();
	ui_manager_close();
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

ui_label ui_create_label_helper(char* str, int x, int y, TTF_Font* font)
{
	SDL_Surface* surface;
	ui_label	 label;

	label._inuse = 1;

	surface = TTF_RenderText_Solid(
		font,
		str,
		font_info.font_color
	);
	label.texture = SDL_CreateTextureFromSurface(
		gf2d_graphics_get_renderer(),
		surface
	);
	label.render_rect.x = x;
	label.render_rect.y = y;
	label.render_rect.w = 200;
	label.render_rect.h = 200;
	SDL_QueryTexture(
		label.texture,
		NULL,
		NULL,
		&label.render_rect.w,
		&label.render_rect.h
	); 

	return label;
}
ui_label ui_create_title_label(char* str, int x, int y)
{
	return ui_create_label_helper(str, x, y, font_info.title_font);
}
ui_label ui_create_header_label(char* str, int x, int y)
{
	return ui_create_label_helper(str, x, y, font_info.header_font);
}
ui_label ui_create_text_label(char* str, int x, int y)
{
	return ui_create_label_helper(str, x, y, font_info.text_font);
}

void ui_label_render(ui_label* l)
{
	if (!l) {
		slog("ui_label_render received NULL ui_label*");
		return;
	}

	SDL_RenderCopy(
		gf2d_graphics_get_renderer(),
		l->texture,
		NULL,
		&l->render_rect
	);

	if (DEBUG){ gf2d_draw_rect(l->render_rect, vector4d(55, 255, 255, 255)); }
}


ui_button ui_create_button(int x, int y, int w, int h, char* str, void (*on_click)(void))
{
	ui_button button;

	button._inuse = 1;
	button.text_label = ui_create_text_label(str, x + 10, y + 10);
	button.click_box.x = x;
	button.click_box.y = y;
	button.click_box.w = w;
	button.click_box.h = h;
	button.on_click = on_click;

	return button;

}

void ui_button_render(ui_button* b)
{
	if (!b)
	{
		slog("ui_button_render cannot render NULL ui_button*");
		return;
	}
	gf2d_draw_rect(b->click_box, vector4d(255, 255, 255, 255));
	ui_label_render(&b->text_label);
}

gamestate_id ui_button_listen(ui_button* b, Uint32 mouse_state, int mx, int my)
{
	gamestate_id id;
	Uint8 was_clicked = 0;

	if (!b)
	{
		slog("ui_button_listen received NULL ui_button*");
		return NONE;
	}

	if (was_mouse_pressed == 1 && mouse_state == 0) { was_clicked = 1; }

	/* Is mouse over button b */
	if (mx > b->click_box.x &&
		mx < b->click_box.x + b->click_box.w &&
		my > b->click_box.y &&
		my < b->click_box.y + b->click_box.h) {

		if (mouse_state == 1) {
			was_mouse_pressed = 1;
			
			// TODO : set sprite pressed
		}
		else if (was_clicked) 
		{
			id = ui_button_click(b);
			was_mouse_pressed = 0;

			if (id) return id;
		}
		
		// TODO : set sprite hover
	}
	else 
	{
		// TODO : set sprite default
	}

	if (!was_clicked && mouse_state == 0) { was_mouse_pressed = 0; }
	
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

ui_sprite ui_create_sprite(Sprite* sprite, Vector2D	position, Vector2D scale, Vector2D scale_center, Vector3D rotation, Uint32 frame_count)
{
	ui_sprite s;

	s._inuse = 1;
	s.sprite = sprite;
	vector2d_copy(s.position, position);
	vector2d_copy(s.scale, scale);
	vector2d_copy(s.scale_center, scale_center);
	vector3d_copy(s.rotation, rotation);
	s.frame_count = frame_count;

	return s;
}

void ui_sprite_render(ui_sprite* s)
{
	if (!s) { slog("ui_sprite_render cannot render NULL ui_sprite*"); return; }

	if (s->frame_count == 1) {
		gf2d_sprite_draw_image(s->sprite, s->position);
	}
}

ui_draggable ui_create_draggable(Vector2D position, Vector2D size)
{
	ui_draggable d;

	d._inuse = 1;
	d.is_held = 0;
	vector2d_copy(d.position, position);
	vector2d_copy(d.prev_position, position);
	d.click_box.x = position.x;
	d.click_box.y = position.y;
	d.click_box.w = size.x;
	d.click_box.h = size.y;

	return d;
}

void ui_draggable_listen(ui_draggable* d, Uint32 mouse_state, int mx, int my)
{
	if (mx > d->click_box.x &&
		mx < d->click_box.x + d->click_box.w &&
		my > d->click_box.y &&
		my < d->click_box.y + d->click_box.h) {

		if (mouse_state == 1 && was_mouse_pressed == 0) 
		{
			was_mouse_pressed = 1;

			d->is_held = 1;
			vector2d_copy(d->mouse_anchor, vector2d(mx,my));
		}
		else if (mouse_state == 0 && was_mouse_pressed == 1)
		{
			d->is_held = 0;
			was_mouse_pressed = 0;
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


void ui_label_free(ui_label* l)
{
	if (!l) { return; }
	ui_sprite_free(&l->sprite);
	memset(l, 0, sizeof(ui_label));
}
void ui_button_free(ui_button* b)
{
	if (!b) return;
	ui_label_free(&b->text_label);
	ui_sprite_free(&b->sprite_default);
	ui_sprite_free(&b->sprite_hover);
	ui_sprite_free(&b->sprite_pressed);
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