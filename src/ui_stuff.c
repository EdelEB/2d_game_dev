#include "ui_stuff.h"

//TODO : There has to be a better way to do this
typedef struct UI_MANAGER {
	Uint32			max_components;
	ui_object*		object_list;
	ui_label*		label_list;
	ui_button*		button_list;
	ui_image*		image_list;
	ui_draggable*	draggable_list;
	ui_text_input*	text_input_list;
	ui_sizable*		sizable_list;
	ui_slider*		slider_list;
}UI_Manager;

typedef struct UI_OBJECT_MANAGER {
	Uint32 max_objects;
	ui_object* object_list;
}UI_ObjectManager;

UI_Manager ui_manager = { 0 };
UI_ObjectManager ui_object_manager = { 0 };

void ui_object_manager_init(Uint32 max_objects)
{
	if (max_objects == 0)
	{
		slog("ui_object_manager cannot allocate for 0 max_objects");
		return;
	}

	ui_object_manager.max_objects = max_objects;
	ui_object_manager.object_list = gfc_allocate_array(sizeof(ui_object), max_objects);
}
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
	ui_manager.image_list = gfc_allocate_array(sizeof(ui_image), max_components);
	ui_manager.draggable_list = gfc_allocate_array(sizeof(ui_draggable), max_components);
	ui_manager.text_input_list = gfc_allocate_array(sizeof(ui_text_input), max_components);
	ui_manager.sizable_list = gfc_allocate_array(sizeof(ui_sizable), max_components);
	ui_manager.slider_list = gfc_allocate_array(sizeof(ui_slider), max_components);

	/*This must be increased every time a new object type is added*/
	ui_object_manager_init(max_components * 7);
}
void ui_manager_clear(void)
{
	int i;
	for (i = 0; i < ui_object_manager.max_objects; i++)
	{
		if (ui_object_manager.object_list[i]._inuse)
		{
			ui_object_free(&ui_object_manager.object_list[i]);
		}
	}
}
void ui_manager_close(void)
{
	ui_manager_clear();

	if (ui_object_manager.object_list) free(ui_object_manager.object_list);

	if (ui_manager.label_list) free(ui_manager.label_list);
	if (ui_manager.button_list) free(ui_manager.button_list);
	if (ui_manager.image_list) free(ui_manager.image_list);
	if (ui_manager.draggable_list) free(ui_manager.draggable_list);
	if (ui_manager.text_input_list) free(ui_manager.text_input_list);

	slog("ui_manager closed");
}

void ui_font_info_init(void)
{
	font_info.title_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 80);
	font_info.header_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 35);
	font_info.text_font = TTF_OpenFont("assets/fonts/Swansea.ttf", 30);
	SDL_Color temp = { 255,255,255,255 }; // Why is this necessary you ask? I don't know. I get an error if I do it directly without the temporary variable
	font_info.font_color = temp;
}
void ui_font_info_close(void)
{
	TTF_CloseFont(font_info.title_font);
	TTF_CloseFont(font_info.header_font);
	TTF_CloseFont(font_info.text_font);
	//free(&font_info.font_color);
}

void ui_sound_fx_init(void)
{
	sound_fx.button_click = gfc_sound_load("assets/sound/sound_click2.wav", 1, 2);
	if (sound_fx.button_click == NULL)slog("FUUUK");
	sound_fx.gamestate_change = gfc_sound_load("assets/sound/sound_click.wav", 1, 2);
	if (sound_fx.gamestate_change == NULL)slog("Fuk2");
}
void ui_sound_fx_close(void)
{
	gfc_sound_free(sound_fx.button_click);
	gfc_sound_free(sound_fx.gamestate_change);
}

void ui_stuff_init(void)
{
	ui_manager_init(512);
	ui_font_info_init();
	ui_sound_fx_init();
}
void ui_stuff_close(void)
{
	ui_font_info_close();
	ui_sound_fx_close();
	ui_manager_close();
}



/////// UI_OBJECT ///////////////////////////////////////////////
ui_object* ui_object_new(void)
{
	int i;
	for (i = 0; i < ui_object_manager.max_objects; i++)
	{
		if (ui_object_manager.object_list[i]._inuse) continue;
		ui_object_manager.object_list[i]._inuse = 1;
		return &ui_object_manager.object_list[i];
	}
	return NULL;
}

void ui_object_free(ui_object* o)
{
	if (!o) return;

	switch (o->id)
	{
	case LABEL:
		ui_label_free(o->label);
		break;
	case BUTTON:
		ui_button_free(o->button);
		break;
	case IMAGE:
		ui_image_free(o->image);
		break;
	case DRAGGABLE:
		ui_draggable_free(o->draggable);
		break;
	case TEXT_INPUT:
		ui_text_input_free(o->text_input);
		break;
	case SIZABLE:
		ui_sizable_free(o->sizable);
		break;
	case SLIDER:
		ui_slider_free(o->slider);
		break;
	}

	memset(o, 0, sizeof(ui_object));
}

gamestate_id ui_object_listen(ui_object* o, Uint32 mouse_state, int mx, int my, Uint8* keys)
{
	if (!o) {
		slog("NULL ui_object* passed to ui_object_listen()");
		return;
	}

	switch (o->id)
	{
	case LABEL:
	case IMAGE:
		break;
	case BUTTON:
		return ui_button_listen(o->button, mouse_state, mx, my);
	case DRAGGABLE:
		ui_draggable_listen(o->draggable, mouse_state, mx, my);
		break;
	case TEXT_INPUT:
		return ui_text_input_listen(o->text_input, mouse_state, mx, my, keys);
	case SIZABLE:
		return ui_sizable_listen(o->sizable, mouse_state, mx, my);
	case SLIDER:
		return ui_slider_listen(o->slider, mouse_state, mx, my);
	}

	return NONE;
}

void ui_object_render(ui_object* o)
{
	if (!o) {
		slog("NULL ui_object* passed to ui_object_render");
		return;
	}

	switch (o->id)
	{
	case LABEL:
		ui_label_render(o->label);
		break;
	case BUTTON:
		ui_button_render(o->button);
		break;
	case IMAGE:
		ui_image_render(o->image);
		break;
	case DRAGGABLE:
		ui_draggable_render(o->draggable);
		break;
	case TEXT_INPUT:
		ui_text_input_render(o->text_input);
		break;
	case SIZABLE:
		ui_sizable_render(o->sizable);
		break;
	case SLIDER:
		ui_slider_render(o->slider);
		break;
	}
	return;
}

SJson* ui_object_to_json(ui_object* o)
{
	SJson* data;
	SJson* json = sj_object_new();

	if (!o) {
		slog("NULL ui_object* passed to ui_object_to_json()");
		return;
	}
	if (!json) return;
	
	data = sj_new_int(o->id);
	if (data) sj_object_insert(json, "id", data);

	data = sj_new_int(o->index);
	if (data) sj_object_insert(json, "index", data);

	switch (o->id)
	{
		case LABEL:
			data = ui_label_to_json(o->label);
		case IMAGE:
			data = ui_image_to_json(o->image);
		case BUTTON:
			data = ui_button_to_json(o->button);
		case TEXT_INPUT:
			data = ui_text_input_to_json(o->text_input);
		case SLIDER:
			//data = ui_slider_to_json(o->slider);
		case DRAGGABLE:
		case SIZABLE:
		default:
			data = NULL;
	}

	if (!data) return NULL;
		
	sj_object_insert(json, "object", data);
	return json;
}

ui_object* ui_object_from_json(SJson* json)
{
	ui_object* object;
	SJson* data;

	if (!json) {
		slog("NULL SJson* passed to ui_object()");
		return NULL;
	}
	
	data = sj_object_get_value(json, "id");
	if (!data) slog("cannot load ui_object without id"); return;
	sj_get_integer_value(data, &object->id);

	data = sj_object_get_value(json, "index");
	if (!data) slog("cannot load ui_object without index"); return;
	sj_get_integer_value(data, &object->index);

	switch (object->id)
	{
		case LABEL:
			return ui_label_from_json(json);
		case IMAGE:
			return ui_image_from_json(json);
		case BUTTON:
			return ui_button_from_json(json);
		case TEXT_INPUT:
			return ui_text_input_from_json(json);
		case SLIDER:
			//return ui_slider_from_json(json);
		case DRAGGABLE:
		case SIZABLE:
		default:
			break;
	}

	return NULL;
}



/////// UI_LABEL ///////////////////////////////////////////////
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

void ui_label_free(ui_label* l)
{
	if (!l) { return; }
	ui_image_free(l->image);
	memset(l, 0, sizeof(ui_label));
}

ui_object* ui_create_label(char* str, int x, int y, ui_label_type type)
{
	TTF_Font* font;
	SDL_Surface* surface;
	ui_label* label = ui_label_new();
	ui_object* object = ui_object_new();

	if (!object) {
		slog("create function failed to get ui_object pointer");
		return NULL;
	}
	if (!label) {
		slog("ui_create_label failed to retrieve ui_label pointer");
		return NULL;
	}
	font = ui_get_font_by_type(type);
	if (!font) { slog("ui_create_label failed to retrieve a font"); return NULL; }

	label->type = type;
	label->str = str;

	surface = TTF_RenderText_Solid(
		font,
		str,
		font_info.font_color
	);
	if (!surface) {
		slog("ui_create_label failed to create SDL_Surface with TTF_RenderText_Solid");
		label->texture = NULL;
	}
	else {
		label->texture = SDL_CreateTextureFromSurface(
			gf2d_graphics_get_renderer(),
			surface
		);
		SDL_QueryTexture(
			label->texture,
			NULL,
			NULL,
			&label->render_rect.w,
			&label->render_rect.h
		);
	}

	label->render_rect.x = x;
	label->render_rect.y = y;
	//label->render_rect.w = 200;
	//label->render_rect.h = 200;

	object->id = LABEL;
	object->label = label;
	return object;
}

void ui_label_update(ui_label* l, char* new_str)
{
	SDL_Surface* surface;
	TTF_Font* font;

	if (!l) { slog("NULL ui_label* passed to ui_label_update"); return; }
	font = ui_get_font_by_type(l->type);
	if (!font) { slog("ui_label with no font passed to ui_label_update"); return; }

	surface = TTF_RenderText_Solid(
		font,
		new_str,
		font_info.font_color
	);
	if (!surface) {
		slog("ui_create_label failed to create SDL_Surface with TTF_RenderText_Solid");
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

TTF_Font* ui_get_font_by_type(ui_label_type type)
{
	switch (type) {
	case TEXT:
		return font_info.text_font;
	case HEADER:
		return font_info.header_font;
	case TITLE:
		return font_info.title_font;
	}

	return NULL;
}

void ui_label_render(ui_label* l)
{
	if (!l) {
		slog("ui_label_render received NULL ui_label*");
		return;
	}

	if (l->image) {
		ui_image_render(l->image);
	}

	if (l->texture) {
		SDL_RenderCopy(
			gf2d_graphics_get_renderer(),
			l->texture,
			NULL,
			&l->render_rect
		);
	}

	if (DEBUG) { gf2d_draw_rect(l->render_rect, vector4d(55, 255, 255, 255)); }

}

SJson* ui_label_to_json(ui_label* l) 
{
	SJson* object = sj_object_new();
	SJson* data;

	if (!object) {
		slog("sj_object_new return NULL pointer in ui_label_to_json");
		return;
	}

	data = sj_new_str(l->str);
	if (data) sj_object_insert(object, "str", data);

	data = sj_new_int(l->type);
	if (data) sj_object_insert(object, "type", data);

	data = sj_new_int(l->render_rect.x);
	if (data) sj_object_insert(object, "x", data);

	data = sj_new_int(l->render_rect.y);
	if (data) sj_object_insert(object, "y", data);

	return object;
}

ui_object* ui_label_from_json(SJson* json)
{
	ui_object* ret;
	char *str;
	int x, y;
	ui_label_type type;

	if (!json) {
		slog("NULL SJson* passed to ui_label_load");
		return NULL;
	}

	str = sj_get_string_value(sj_object_get_value(json, "str"));
	sj_get_integer_value(sj_object_get_value(json, "x"), &x);
	sj_get_integer_value(sj_object_get_value(json, "y"), &y);
	sj_get_integer_value(sj_object_get_value(json, "type"), &type);

	ret = ui_create_label(str, x, y, type);
	if (ret) return ret;
	return NULL;
}



/////// UI_BUTTON ///////////////////////////////////////////////
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

void ui_button_free(ui_button* b)
{
	if (!b) return;
	ui_label_free(b->text_label);
	ui_image_free(b->image_default);
	ui_image_free(b->image_hover);
	ui_image_free(b->image_pressed);
	memset(b, 0, sizeof(ui_button));
}

ui_object* ui_create_button(int x, int y, int w, int h, char* str, void (*on_click)(void))
{
	ui_button* button = ui_button_new();
	ui_object* object = ui_object_new();
	if (!object) {
		slog("create function failed to get ui_object pointer");
		return;
	}
	if (!button) {
		slog("ui_create_button failed to retrieve button pointer");
		return NULL;
	}

	button->text_label = ui_create_label(str, x + 10, y + 10, TEXT)->label;
	button->click_box.x = x;
	button->click_box.y = y;
	button->click_box.w = w;
	button->click_box.h = h;
	button->on_click = on_click;

	object->id = BUTTON;
	object->button = button;
	return object;

}

void ui_button_set_images(ui_button* b, char* file_base_name, Vector2D scale, Vector2D scale_center, Vector3D rotation)
{
	Uint8 i;
	char filename[64];
	Sprite* sprite;
	ui_image* image;

	for (i = 0; i < 3; i++)
	{
		sprintf(filename, "assets/images/ui/%s_%d.png", file_base_name, i);

		image = ui_create_image(
			filename,
			vector2d(b->click_box.x, b->click_box.y),
			scale,
			scale_center,
			rotation
		)->image;

		if (!image) continue;
		switch (i) {
		case 0:
			b->image_default = image;
			b->image_current = b->image_default;
			break;
		case 1:
			b->image_hover = image;
			break;
		case 2:
			b->image_pressed = image;
			break;
		}
	}
}

gamestate_id ui_button_click(ui_button* b)
{
	gamestate_id ret;

	if (!b)
	{
		slog("ui_button_click passed NULL ui_button*");
		return NONE;
	}
	if (!b->on_click)
	{
		slog("ui_button_click returns simple_nav %d", b->simple_nav);
		return b->simple_nav;
	}

	gfc_sound_play(sound_fx.button_click, 0, 0.3, -1, -1);

	ret = b->on_click(b);
	if (ret != NONE) {
		gfc_sound_play(sound_fx.gamestate_change, 0, 0.3, -1, -1);
	}
	return ret;
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

		if (mouse_state == 1 && global_was_mouse_down == 0) b->is_held = 1;
		
		if (b->is_held) 
		{
			b->image_current = b->image_pressed;

			if (mouse_state == 0 && global_was_mouse_down == 1)
			{
				global_was_mouse_down = 0;
				b->is_held = 0;
				id = ui_button_click(b);
				if (id) return id;
			}
		}
		else 
		{
			b->image_current = b->image_hover;
		}
	}
	else
	{
		b->image_current = b->image_default;
	}

	if (mouse_state == 0 && global_was_mouse_down == 1) b->is_held = 0;
	
	return NONE;
}

void ui_button_render(ui_button* b)
{
	if (!b)
	{
		slog("ui_button_render cannot render NULL ui_button*");
		return;
	}

	if (!b->hide_click_box) { gf2d_draw_rect(b->click_box, vector4d(255, 255, 255, 255)); }

	if (b->image_current) { ui_image_render(b->image_current); }

	ui_label_render(b->text_label);
}

SJson* ui_button_to_json(ui_button* b)
{
	SJson* object = sj_object_new();
	SJson* data;

	if (!b) {
		slog("NULL ui_button* passed to ui_button_to_json");
		return NULL;
	}
	if (!object) {
		slog("NULL SJson* received in ui_button_to_json()");
		return NULL;
	}

	data = sj_new_str(b->text_label->str);
	if (data) sj_object_insert(object, "str", data);

	data = sj_new_int(b->click_box.x);
	if (data) sj_object_insert(object, "x", data);

	data = sj_new_int(b->click_box.y);
	if (data) sj_object_insert(object, "y", data);

	data = sj_new_int(b->click_box.w);
	if (data) sj_object_insert(object, "w", data);

	data = sj_new_int(b->click_box.h);
	if (data) sj_object_insert(object, "h", data);


	return object;
}

ui_object* ui_button_from_json(SJson* json) 
{
	int x, y, w, h;
	char* str;

	ui_object* ret;

	if (!json) {
		slog("NULL SJson* passed to ui_label_load");
		return NULL;
	}

	sj_get_integer_value(
		sj_object_get_value(json, "x"), &x );
	sj_get_integer_value(
		sj_object_get_value(json, "y"), &y );
	sj_get_integer_value(
		sj_object_get_value(json, "w"), &w );
	sj_get_integer_value(
		sj_object_get_value(json, "h"), &h );
	str = sj_get_string_value(sj_object_get_value(json, "str"));

	ret = ui_create_button( x, y, w, h, str, NULL);
	if (ret) return ret;
	return NULL;
}



/////// UI_IMAGE ///////////////////////////////////////////////
ui_image* ui_image_new(void)
{
	int i;
	for (i = 0; i < ui_manager.max_components; i++)
	{
		if (ui_manager.image_list[i]._inuse) continue;
		ui_manager.image_list[i]._inuse = 1;
		return &ui_manager.image_list[i];
	}
	return NULL;
}

void ui_image_free(ui_image* image)
{
	if (!image) return;
	gf2d_sprite_free(image->sprite);
	memset(image, 0, sizeof(ui_image));
}

ui_object* ui_create_image(char* filename, Vector2D position, Vector2D scale, Vector2D scale_center, Vector3D rotation)
{
	Sprite* sprite;
	ui_image* image = ui_image_new();
	ui_object* object = ui_object_new();
	if (!object) {
		slog("create function failed to get ui_object pointer");
		return;
	}
	if (!image) {
		slog("ui_create_image failed to retrieve a ui_image pointer");
		return NULL;
	}

	sprite = gf2d_sprite_load_image(filename);
	if (!sprite) {
		slog("failed to create a image in ui_create_image");
		return NULL;
	}

	image->sprite = sprite;
	vector2d_copy(image->position, position);
	vector2d_copy(image->scale, scale);
	vector2d_copy(image->scale_center, scale_center);
	vector3d_copy(image->rotation, rotation);

	object->id = IMAGE;
	object->image = image;
	return object;
}

void ui_image_render(ui_image* image)
{
	if (!image) { slog("ui_image_render cannot render NULL ui_image*"); return; }

	gf2d_sprite_draw(
		image->sprite,
		image->position,
		&image->scale,
		&image->scale_center,
		&image->rotation,
		NULL,
		NULL,
		0
	);
}

SJson* ui_image_to_json(ui_image* image) {}

ui_object* ui_image_from_json(SJson* json) {}



/////// UI_DRAGGABLE ///////////////////////////////////////////////
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

void ui_draggable_free(ui_draggable* d)
{
	if (!d) return;
	memset(d, 0, sizeof(ui_draggable));
}

ui_object* ui_create_draggable(Vector2D position, Vector2D size)
{
	ui_draggable* d = ui_draggable_new();
	ui_object* object = ui_object_new();
	if (!object) {
		slog("create function failed to get ui_object pointer");
		return;
	}
	if (!d) {
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

	object->id = DRAGGABLE;
	object->draggable = d;
	return object;
}

void ui_draggable_listen(ui_draggable* d, Uint32 mouse_state, int mx, int my)
{
	if (mx > d->click_box.x &&
		mx < d->click_box.x + d->click_box.w &&
		my > d->click_box.y &&
		my < d->click_box.y + d->click_box.h &&
		mouse_state == 1 && 
		global_was_mouse_down == 0)
	{
		global_was_mouse_down = 1;
		d->is_held = 1;
		vector2d_copy(d->mouse_anchor, vector2d(mx, my));
	}

	if (d->is_held)
	{
		d->position.x = d->prev_position.x - (d->mouse_anchor.x - mx);
		d->position.y = d->prev_position.y - (d->mouse_anchor.y - my);
		d->click_box.x = d->position.x;
		d->click_box.y = d->position.y;

		if (mouse_state == 0 && global_was_mouse_down == 1)
		{
			d->is_held = 0;
			global_was_mouse_down = 0;
			vector2d_copy(d->prev_position, d->position);
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



/////// UI_TEXT_INPUT ///////////////////////////////////////////////
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

void ui_text_input_free(ui_text_input* t)
{
	if (!t) return;

	ui_button_free(t->button_enter);
	ui_label_free(t->text_label);
	memset(t, 0, sizeof(ui_text_input));
}

ui_object* ui_create_text_input(Vector2D position, void (*on_enter)(void))
{
	ui_text_input* t = ui_text_input_new();
	ui_object* object = ui_object_new();
	if (!object) {
		slog("create function failed to get ui_object pointer");
		return;
	}
	if (!t) {
		slog("ui_create_text_input could not retrieve ui_text_input pointer");
		return;
	}

	t->index = 0;
	t->text_label = ui_create_label(
		" ",
		position.x + 5,
		position.y + 7,
		TEXT
	)->label;
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
	)->button;

	object->id = TEXT_INPUT;
	object->text_input = t;
	return object;
}

gamestate_id ui_text_input_listen(ui_text_input* t, Uint32 mouse_state, int mx, int my, Uint8* keys)
{
	gamestate_id id;
	char c = '%';

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

	if (t->is_active) {

		if (type_cooldown > 0) {
			type_cooldown--;
			return NONE;
		}

		// TODO : There's probably a way to make this a switch() statement which would be faster and better
		if (keys[SDL_SCANCODE_RSHIFT] || keys[SDL_SCANCODE_LSHIFT])
		{
			if (keys[SDL_SCANCODE_Q]) { c = 'Q'; }
			else if (keys[SDL_SCANCODE_W]) { c = 'W'; }
			else if (keys[SDL_SCANCODE_E]) { c = 'E'; }
			else if (keys[SDL_SCANCODE_R]) { c = 'R'; }
			else if (keys[SDL_SCANCODE_T]) { c = 'T'; }
			else if (keys[SDL_SCANCODE_Y]) { c = 'Y'; }
			else if (keys[SDL_SCANCODE_U]) { c = 'U'; }
			else if (keys[SDL_SCANCODE_I]) { c = 'I'; }
			else if (keys[SDL_SCANCODE_O]) { c = 'O'; }
			else if (keys[SDL_SCANCODE_P]) { c = 'P'; }
			else if (keys[SDL_SCANCODE_A]) { c = 'A'; }
			else if (keys[SDL_SCANCODE_S]) { c = 'S'; }
			else if (keys[SDL_SCANCODE_D]) { c = 'D'; }
			else if (keys[SDL_SCANCODE_F]) { c = 'F'; }
			else if (keys[SDL_SCANCODE_G]) { c = 'G'; }
			else if (keys[SDL_SCANCODE_H]) { c = 'H'; }
			else if (keys[SDL_SCANCODE_J]) { c = 'J'; }
			else if (keys[SDL_SCANCODE_K]) { c = 'K'; }
			else if (keys[SDL_SCANCODE_L]) { c = 'L'; }
			else if (keys[SDL_SCANCODE_Z]) { c = 'Z'; }
			else if (keys[SDL_SCANCODE_X]) { c = 'X'; }
			else if (keys[SDL_SCANCODE_C]) { c = 'C'; }
			else if (keys[SDL_SCANCODE_V]) { c = 'V'; }
			else if (keys[SDL_SCANCODE_B]) { c = 'B'; }
			else if (keys[SDL_SCANCODE_N]) { c = 'N'; }
			else if (keys[SDL_SCANCODE_M]) { c = 'M'; }
			else if (keys[SDL_SCANCODE_MINUS]) { c = '_'; }
		}
		else
		{
			if (keys[SDL_SCANCODE_Q]) { c = 'q'; }
			else if (keys[SDL_SCANCODE_W]) { c = 'w'; }
			else if (keys[SDL_SCANCODE_E]) { c = 'e'; }
			else if (keys[SDL_SCANCODE_R]) { c = 'r'; }
			else if (keys[SDL_SCANCODE_T]) { c = 't'; }
			else if (keys[SDL_SCANCODE_Y]) { c = 'y'; }
			else if (keys[SDL_SCANCODE_U]) { c = 'u'; }
			else if (keys[SDL_SCANCODE_I]) { c = 'i'; }
			else if (keys[SDL_SCANCODE_O]) { c = 'o'; }
			else if (keys[SDL_SCANCODE_P]) { c = 'p'; }
			else if (keys[SDL_SCANCODE_A]) { c = 'a'; }
			else if (keys[SDL_SCANCODE_S]) { c = 's'; }
			else if (keys[SDL_SCANCODE_D]) { c = 'd'; }
			else if (keys[SDL_SCANCODE_F]) { c = 'f'; }
			else if (keys[SDL_SCANCODE_G]) { c = 'g'; }
			else if (keys[SDL_SCANCODE_H]) { c = 'h'; }
			else if (keys[SDL_SCANCODE_J]) { c = 'j'; }
			else if (keys[SDL_SCANCODE_K]) { c = 'k'; }
			else if (keys[SDL_SCANCODE_L]) { c = 'l'; }
			else if (keys[SDL_SCANCODE_Z]) { c = 'z'; }
			else if (keys[SDL_SCANCODE_X]) { c = 'x'; }
			else if (keys[SDL_SCANCODE_C]) { c = 'c'; }
			else if (keys[SDL_SCANCODE_V]) { c = 'v'; }
			else if (keys[SDL_SCANCODE_B]) { c = 'b'; }
			else if (keys[SDL_SCANCODE_N]) { c = 'n'; }
			else if (keys[SDL_SCANCODE_M]) { c = 'm'; }
			else if (keys[SDL_SCANCODE_0]) { c = '0'; }
			else if (keys[SDL_SCANCODE_1]) { c = '1'; }
			else if (keys[SDL_SCANCODE_2]) { c = '2'; }
			else if (keys[SDL_SCANCODE_3]) { c = '3'; }
			else if (keys[SDL_SCANCODE_4]) { c = '4'; }
			else if (keys[SDL_SCANCODE_5]) { c = '5'; }
			else if (keys[SDL_SCANCODE_6]) { c = '6'; }
			else if (keys[SDL_SCANCODE_7]) { c = '7'; }
			else if (keys[SDL_SCANCODE_8]) { c = '8'; }
			else if (keys[SDL_SCANCODE_9]) { c = '9'; }
			else if (keys[SDL_SCANCODE_SPACE]) { c = ' '; }
			else if (keys[SDL_SCANCODE_SLASH]) { c = '/'; }
			else if (keys[SDL_SCANCODE_PERIOD]) { c = '.'; }
			else if (keys[SDL_SCANCODE_BACKSPACE]) {
				if (t->index == 0) {
					t->str[t->index] = ' ';
				}
				else {
					t->index--;
					if (t->index == 0) t->str[t->index] = ' ';
					else t->str[t->index] = '\0';
				}
				ui_label_update(t->text_label, t->str);
				type_cooldown = TYPE_COOLDOWN;
			}
		}
	}

	if (c != '%')
	{
		t->str[t->index] = c;
		ui_label_update(t->text_label, t->str);

		t->index++;
		if (t->index >= SENTENCE_LEN) {
			t->index = SENTENCE_LEN - 1;
		}

		type_cooldown = TYPE_COOLDOWN;
	}

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
		gf2d_draw_line(
			vector2d(t->text_label->render_rect.x + t->text_label->render_rect.w, t->text_label->render_rect.y),
			vector2d(t->text_label->render_rect.x + t->text_label->render_rect.w, t->text_label->render_rect.y + t->text_label->render_rect.h),
			vector4d(255, 255, 255, 255)
		);
	}
	else {
		gf2d_draw_rect(t->click_box, vector4d(255, 255, 255, 255));
	}

	ui_button_render(t->button_enter);
	ui_label_render(t->text_label);
}

SJson* ui_text_input_to_json(ui_text_input* t) {}

ui_object* ui_text_input_from_json(SJson* json) {}



/////// UI_SLIDER ///////////////////////////////////////////////
ui_slider* ui_slider_new(void)
{
	int i;
	for (i = 0; i < ui_manager.max_components; i++)
	{
		if (ui_manager.slider_list[i]._inuse) continue;
		ui_manager.slider_list[i]._inuse = 1;
		return &ui_manager.slider_list[i];
	}
	return NULL;
}

void ui_slider_free(ui_slider* s)
{
	if (!s) return;

	//no pointers in sliders
	memset(s, 0, sizeof(ui_slider));
}

ui_object* ui_create_slider(Vector2D position, Vector2D size, Uint8 is_vertical, Uint32 length_left, Uint32 length_right, Uint8 show_line)
{
	ui_slider* s = ui_slider_new();
	ui_object* o = ui_object_new();

	if (!s) return;
	if (!o) return;

	s->click_box_color = vector4d(255, 255, 255, 255);
	vector2d_copy(s->prev_position, position);
	s->click_box.x = position.x;
	s->click_box.y = position.y;
	s->click_box.w = size.x;
	s->click_box.h = size.y;
	s->is_vertical = is_vertical;

	if (length_left == 0 && length_right == 0) {
		s->show_line = 0;
		vector2d_copy(s->line_p1, vector2d(-100, -100));
		vector2d_copy(s->line_p1, vector2d(WINDOW_WIDTH + 100, WINDOW_HEIGHT + 100));
	}
	else {
		s->show_line = show_line;
		s->has_limit = 1;

		if (s->is_vertical)
		{
			vector2d_copy(s->line_p1, vector2d(position.x + size.x / 2, position.y + length_right));
			vector2d_copy(s->line_p2, vector2d(position.x + size.x / 2, position.y - length_right));
		}
		else
		{
			vector2d_copy(s->line_p1, vector2d(position.x + length_left, position.y + size.y / 2));
			vector2d_copy(s->line_p2, vector2d(position.x - length_right, position.y + size.y / 2));
		}
	}
	
	o->id = SLIDER;
	o->slider = s;
	return o;
}

gamestate_id ui_slider_listen(ui_slider* s, Uint32 mouse_state, int mx, int my)
{
	if (!s) {
		slog("NULL ui_slider* passed to ui_slider_listen()");
		return NONE;
	}

	if (mx > s->click_box.x &&
		mx < s->click_box.x + s->click_box.w &&
		my > s->click_box.y &&
		my < s->click_box.y + s->click_box.h)
	{
		s->click_box_color.x = 0;
		
		if (mouse_state == 1 && global_was_mouse_down == 0)
		{
			global_was_mouse_down = 1;
			s->is_held = 1;
			vector2d_copy(s->mouse_anchor, vector2d(mx, my));	
		}
	}
	else {
		s->click_box_color.x = 255;
	}

	if (s->is_held)
	{
		if (s->is_vertical &&
			my < s->line_p1.y && 
			my > s->line_p2.y)
		{
			s->click_box.y = s->prev_position.y - (s->mouse_anchor.y - my);
		}
		else if (mx < s->line_p1.x &&
				mx > s->line_p2.x)
		{
			s->click_box.x = s->prev_position.x - (s->mouse_anchor.x - mx);
		}

		if (mouse_state == 0 && global_was_mouse_down == 1)
		{
			s->is_held = 0;
			global_was_mouse_down = 0;
			s->prev_position.x = s->click_box.x;
			s->prev_position.y = s->click_box.y;
		}
	}

	return NONE;
}

void ui_slider_render(ui_slider* s)
{
	if (!s) return;

	gf2d_draw_rect(s->click_box, s->click_box_color);

	if (s->show_line) gf2d_draw_line(s->line_p1, s->line_p2, vector4d(255, 255, 255, 255));
}



/////// UI_SIZABLE ///////////////////////////////////////////////
ui_sizable* ui_sizable_new(void)
{
	int i;
	for (i = 0; i < ui_manager.max_components; i++)
	{
		if (ui_manager.sizable_list[i]._inuse) continue;
		ui_manager.sizable_list[i]._inuse = 1;
		return &ui_manager.sizable_list[i];
	}
	return NULL;
}

void ui_sizable_free(ui_sizable* s)
{
	if (!s) return;

	ui_slider_free(s->left);
	ui_slider_free(s->right);
	ui_slider_free(s->top);
	ui_slider_free(s->bottom);
	free(&s->rect);
	memset(s, 0, sizeof(ui_sizable));
}

ui_object* ui_create_sizable(Vector2D position, Vector2D size)
{
	Vector2D corner_dim = vector2d(10, 10);
	ui_sizable* s = ui_sizable_new();
	ui_object* o = ui_object_new();

	if (!s) return;
	if (!o) return;

	s->rect.x = position.x;
	s->rect.y = position.y;
	s->rect.w = size.x;
	s->rect.h = size.y;

	s->left =	ui_create_slider(
		vector2d(
			position.x, 
			position.y + size.y / 2 - corner_dim.y / 2), 
		corner_dim, 
		0,0,0,0
	)->slider;

	s->right =	ui_create_slider(
		vector2d(
			position.x + size.x - corner_dim.x, 
			position.y + size.y / 2 - corner_dim.y / 2), 
		corner_dim,
		0, 0, 0, 0
	)->slider;
	
	s->top =	ui_create_slider(
		vector2d(
			position.x + size.x / 2 - corner_dim.x / 2, 
			position.y), 
		corner_dim,
		1, 0, 0, 0
	)->slider;
	
	s->bottom = ui_create_slider(
		vector2d(
			position.x + size.x / 2 - corner_dim.x / 2, 
			position.y + size.y - corner_dim.y)
		, corner_dim,
		1, 0, 0, 0
	)->slider;

	o->id = SIZABLE;
	o->sizable = s;
	return o;
}

gamestate_id ui_sizable_listen(ui_sizable* s, Uint32 mouse_state, int mx, int my)
{
	if (!s) return NONE;

	ui_slider_listen(s->left, mouse_state, mx, my);
	ui_slider_listen(s->right, mouse_state, mx, my);
	ui_slider_listen(s->top, mouse_state, mx, my);
	ui_slider_listen(s->bottom, mouse_state, mx, my);

	s->is_held = 1; // this is here instead of in every part of the if() because the else() cancels it out if it doesn't hit anyway
	if (s->left->is_held)
	{
		s->rect.w += s->rect.x - s->left->click_box.x;
		s->rect.x = s->left->click_box.x;
	}
	else if (s->right->is_held)
	{
		s->rect.w = s->right->click_box.x + s->right->click_box.w - s->rect.x;
	}
	else if (s->top->is_held)
	{
		s->rect.h += s->rect.y - s->top->click_box.y;
		s->rect.y = s->top->click_box.y;	
	}
	else if (s->bottom->is_held)
	{
		s->rect.h = s->bottom->click_box.y + s->bottom->click_box.h - s->rect.y;
	}
	else
	{
		s->is_held = 0;
	}

	/*updates slider points*/
	if (s->is_held)
	{
		s->left->click_box.x = s->rect.x;
		s->left->click_box.y = s->rect.y + s->rect.h / 2 - SLIDER_POINT_DIM / 2;

		s->right->click_box.x = s->rect.x + s->rect.w - SLIDER_POINT_DIM;
		s->right->click_box.y = s->rect.y + s->rect.h / 2 - SLIDER_POINT_DIM / 2;

		s->top->click_box.x = s->rect.x + s->rect.w / 2 - SLIDER_POINT_DIM / 2;
		s->top->click_box.y = s->rect.y;

		s->bottom->click_box.x = s->rect.x + s->rect.w / 2 - SLIDER_POINT_DIM / 2;
		s->bottom->click_box.y = s->rect.y + s->rect.h - SLIDER_POINT_DIM;
	}

	return NONE;
}

void ui_sizable_render(ui_sizable* s)
{
	if (!s) {
		slog("NULL ui_sizable* passed to ui_sizable_render");
		return;
	}
	ui_slider_render(s->left);
	ui_slider_render(s->right);
	ui_slider_render(s->top);
	ui_slider_render(s->bottom);
	gf2d_draw_rect(s->rect, vector4d(255, 255, 255, 255));
}