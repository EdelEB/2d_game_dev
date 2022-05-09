#include "menu_editor.h"

MenuEditor menu_editor = {0};

void menu_editor_init()
{
	menu_editor.id = EDITOR_MENU;
	menu_editor.editor_menu = menu_new();
	menu_editor.working_menu = menu_new();

	if (!menu_editor.editor_menu || !menu_editor.working_menu){
		slog("Couldnt' get Menu* to initialize menu_editor");
		return;
	}
	
	menu_editor.editor_menu->id = EDITOR_EDITOR;
	menu_editor.working_menu->id = EDITOR_WORKING;

	menu_editor.editor_menu->object_list[0] = ui_create_button_standard(0,    5, "Restart", menu_editor_new_menu);
	menu_editor.editor_menu->object_list[1] = ui_create_button_standard(200,  5, "Label", prompt_label);
	menu_editor.editor_menu->object_list[2] = ui_create_button_standard(400,  5, "Button", prompt_button);
	menu_editor.editor_menu->object_list[3] = ui_create_button_standard(600,  5, "Text_Box", editable_create_text_input);
	menu_editor.editor_menu->object_list[4] = ui_create_button_standard(800,  5, "Image", prompt_image);
	menu_editor.editor_menu->object_list[5] = ui_create_button_standard(1000, 5, "Save Menu", prompt_save);

	menu_editor.editor_menu->object_list[6] = ui_create_button_standard(0, WINDOW_HEIGHT - 60, "Return", NULL);
	if (menu_editor.editor_menu->object_list[6]) menu_editor.editor_menu->object_list[0]->button->simple_nav = MENU_START;

	menu_editor.editor_menu->object_list[7] = ui_create_button_standard(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT - 60, "Background", prompt_background);

	slog("menu editor initialized");
}

gamestate_id menu_editor_listen(Uint32 mouse_state, int mx, int my, Uint8* keys)
{
	int i;
	gamestate_id id;

	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (!menu_editor.editable_list[i]._inuse) continue;
		editable_listen(&menu_editor.editable_list[i], mouse_state, mx, my, keys);
	}

	id = menu_listen(menu_editor.editor_menu, mouse_state, &mx, &my, keys);
	if (id > 100) slog("%d", id);
	return id;
}

void menu_editor_render() 
{
	int i;

	if (menu_editor.background) 
		ui_image_render(menu_editor.background);

	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (!menu_editor.editable_list[i]._inuse) continue;
		editable_render(&menu_editor.editable_list[i]);
	}

	menu_draw(menu_editor.editor_menu);
	return;
}

void menu_editor_new_menu()
{
	int i;

	menu_free(menu_editor.working_menu);

	menu_editor.working_menu = menu_new();
	if (!menu_editor.working_menu) { slog("Failed to create new menu"); return; }
	menu_editor.working_menu->id = EDITOR_WORKING;

	for (i = 0; i < MAX_MENU_OBJECTS; i++) {
		editable_free(&menu_editor.editable_list[i]);
	}
}

void menu_editor_save_menu() 
{
	char filename[64];
	ui_object* tbox;
	SJson* json;
	int i;

	tbox = menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1];
	if (tbox->id == TEXT_INPUT) {
		sprintf(filename, "assets/json/working/%s.json", tbox->text_input->str);
		ui_object_free(tbox);
	}
	
	menu_editor.working_menu->title = calloc(64, sizeof(char));
	strcpy(menu_editor.working_menu->title, filename);
	menu_editor.working_menu->id = UNFINISHED_MENU;

	if(menu_editor.background) menu_editor.working_menu->background = menu_editor.background;
	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (!menu_editor.editable_list[i]._inuse) continue;
		menu_editor.working_menu->object_list[i] = menu_editor.editable_list[i].object;
	}

	json = menu_to_json(menu_editor.working_menu);
	if (!json) return;

	sj_save(json, filename);

	menu_editor_new_menu();

	return 0;
}

menu_editor_set_background(void) {
	char filename[64];
	ui_object* tbox;
	SJson* json;
	int i;

	tbox = menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1];
	if (tbox->id == TEXT_INPUT) {
		sprintf(filename, "assets/images/backgrounds/%s.png", tbox->text_input->str);
		ui_object_free(tbox);
	}

	menu_editor.background = ui_create_image( 
		filename, 
		vector2d(0,0), 
		vector2d(1,1), 
		vector2d(0, 0), 
		vector3d(0, 0, 0)
	)->image;

	return 0;
}

void prompt_save(void)
{
	menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_text_input(
		vector2d(100, 100), menu_editor_save_menu
	);
	return 0;
}

void prompt_label(void) 
{
	menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_text_input(
		vector2d(100, 100), editable_create_label
	);
	return 0;
}

void prompt_button(void)
{
	menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_text_input(
		vector2d(100, 100), editable_create_button
	);
	return 0;
}

void prompt_image(void) {
	menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_text_input(
		vector2d(100, 100), editable_create_image
	);
	return 0;
}

void prompt_background(void) {
	menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_text_input(
		vector2d(100, 100), menu_editor_set_background
	);
	return 0;
}


///////* Editable *///////////////////////////////////////////////////////////////////////

Editable* editable_new()
{
	int i;
	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (menu_editor.editable_list[i]._inuse) continue;
		menu_editor.editable_list[i]._inuse = 1;
		return &menu_editor.editable_list[i];
	}
	return NULL;
}

void editable_create_label()
{
	Editable* e = editable_new();
	char str[128];
	Vector2D position;
	ui_object* tbox;
	ui_label_type type = TEXT;

	if (!e) {
		slog("NULL Editable* retrieved in editable_create_label");
		return;
	}
	if (!menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1]) return;

	position = vector2d(WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1);
	tbox = menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1];

	sprintf(str, "%s", tbox->text_input->str);

	e->object = ui_create_label(str, position.x, position.y, type);
	ui_object_free(tbox);
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = ui_create_sizable(position, vector2d(100,100))->sizable;
	return 0;
}

void editable_create_button()
{
	Editable* e = editable_new();
	char str[128];
	Vector2D position;
	ui_object* tbox;

	if (!e) {
		slog("NULL Editable* retrieved in editable_create_button");
		return; 
	}
	if (!menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS-1]) return;
	
	position = vector2d(WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1);
	tbox = menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS-1];
	if (tbox->id == TEXT_INPUT) {
		sprintf(str, "%s", tbox->text_input->str);
		ui_object_free(tbox);
	}
	else {
		slog("editable_create_button failed");
		return;
	}
	
	e->object = ui_create_button(position.x, position.y, 200, 200, str, NULL);
	if (!e->object) { editable_free(e); return 0; }
	e->draggable = ui_create_draggable(position, vector2d(50, 50))->draggable;
	e->sizable = ui_create_sizable(position, vector2d(200,200))->sizable;

	return 0;
}

void editable_create_image() 
{
	Editable* e = editable_new();
	char filename[128];
	Vector2D position;
	ui_object* tbox;

	if (!e) {
		slog("NULL Editable* retrieved in editable_create_image");
		return;
	}
	if (!menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1]) return;

	position = vector2d(WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1);
	tbox = menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1];
	if (tbox->id == TEXT_INPUT) {
		sprintf(filename, "assets/images/%s.png", tbox->text_input->str);
		ui_object_free(tbox);
	}
	else {
		slog("editable_create_image failed");
		return 0;
	}


	e->object = ui_create_image(filename, position, vector2d(0.2,0.2), vector2d(1,1), vector3d(0,0,0));
	if (!e->object) { editable_free(e); return 0; }
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = ui_create_sizable(position, vector2d(100,100))->sizable;

	return 0;
}

void editable_create_text_input()
{
	Vector2D position = position = vector2d(WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1);
	Editable* e = editable_new();

	if (!e) return;

	e->object = ui_create_text_input(position, NULL);
	if (!e->object) { editable_free(e); return 0; }
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = NULL;// ui_create_sizable(position, vector2d())->sizable;
}

void editable_listen(Editable* e, Uint32 mouse_state, int mx, int my)
{
	if (!e) return;

	if (e->object && e->draggable)
	{
		ui_draggable_listen(e->draggable, mouse_state, mx, my);

		if (e->draggable->is_held)
		{
			if (e->sizable) {
				e->sizable->rect.x = e->draggable->position.x;
				e->sizable->rect.y = e->draggable->position.y;

				e->sizable->left->click_box.x = e->sizable->rect.x;
				e->sizable->left->click_box.y = e->sizable->rect.y + e->sizable->rect.h / 2 - SLIDER_POINT_DIM / 2;
				e->sizable->right->click_box.x = e->sizable->rect.x + e->sizable->rect.w - SLIDER_POINT_DIM;
				e->sizable->right->click_box.y = e->sizable->rect.y + e->sizable->rect.h / 2 - SLIDER_POINT_DIM / 2;
				e->sizable->top->click_box.x = e->sizable->rect.x + e->sizable->rect.w / 2 - SLIDER_POINT_DIM / 2;
				e->sizable->top->click_box.y = e->sizable->rect.y;
				e->sizable->bottom->click_box.x = e->sizable->rect.x + e->sizable->rect.w / 2 - SLIDER_POINT_DIM / 2;
				e->sizable->bottom->click_box.y = e->sizable->rect.y + e->sizable->rect.h - SLIDER_POINT_DIM;

				e->sizable->left->prev_position = vector2d(e->sizable->left->click_box.x, e->sizable->left->click_box.y);
				e->sizable->right->prev_position = vector2d(e->sizable->right->click_box.x, e->sizable->right->click_box.y);
				e->sizable->bottom->prev_position = vector2d(e->sizable->bottom->click_box.x, e->sizable->bottom->click_box.y);
				e->sizable->top->prev_position = vector2d(e->sizable->top->click_box.x, e->sizable->top->click_box.y);
			}
			// I know this is disgusting. Objects should all have a position but I can't do that right now
			switch (e->object->id)
			{
				case LABEL:
					e->object->label->render_rect.x = e->draggable->position.x;
					e->object->label->render_rect.y = e->draggable->position.y;
					break;
				case BUTTON:
					e->object->button->click_box.x = e->draggable->position.x;
					e->object->button->click_box.y = e->draggable->position.y;
					e->object->button->text_label->render_rect.x = e->draggable->position.x + 10;
					e->object->button->text_label->render_rect.y = e->draggable->position.y + 10;
					if(e->object->button->image_default)
						vector2d_copy(e->object->button->image_default->position, e->draggable->position);
					if (e->object->button->image_hover)
						vector2d_copy(e->object->button->image_hover->position, e->draggable->position);
					if (e->object->button->image_pressed)
						vector2d_copy(e->object->button->image_pressed->position, e->draggable->position);
					break;
				case IMAGE:
					vector2d_copy(e->object->image->position, e->draggable->position);
					break;
				case  TEXT_INPUT:
					e->object->text_input->click_box.x = e->draggable->position.x;
					e->object->text_input->click_box.y = e->draggable->position.y;
					e->object->text_input->text_label->render_rect.x = e->draggable->position.x + 5;
					e->object->text_input->text_label->render_rect.y = e->draggable->position.y + 7;
					
					e->object->text_input->button_enter->click_box.x = e->draggable->position.x + e->object->text_input->click_box.w;
					e->object->text_input->button_enter->click_box.y = e->draggable->position.y;
					e->object->text_input->button_enter->text_label->render_rect.x = e->draggable->position.x + e->object->text_input->click_box.w + 10;
					e->object->text_input->button_enter->text_label->render_rect.y = e->draggable->position.y + 10;
					if (e->object->text_input->button_enter->image_default)
						vector2d_copy(e->object->text_input->button_enter->image_default->position, e->draggable->position);
					if (e->object->text_input->button_enter->image_hover)
						vector2d_copy(e->object->text_input->button_enter->image_hover->position, e->draggable->position);
					if (e->object->text_input->button_enter->image_pressed)
						vector2d_copy(e->object->text_input->button_enter->image_pressed->position, e->draggable->position);
					vector2d_copy(e->object->text_input->position, e->draggable->position);
					break;
				default:
					break;
			}
		}
	}
	if(e->object && e->sizable)
	{
		ui_sizable_listen(e->sizable, mouse_state, mx, my);

		if (e->sizable->is_held)
		{
			if (e->draggable)
			{
				e->draggable->position = vector2d(e->sizable->rect.x, e->sizable->rect.y);
				vector2d_copy(e->draggable->prev_position, e->draggable->position);
				e->draggable->click_box.x = e->sizable->rect.x;
				e->draggable->click_box.y = e->sizable->rect.y;
			}

			switch (e->object->id)
			{
				case LABEL:
					e->object->label->render_rect.x = e->sizable->rect.x;
					e->object->label->render_rect.y = e->sizable->rect.y;
					e->object->label->render_rect.w = e->sizable->rect.w;
					e->object->label->render_rect.h = e->sizable->rect.h;
					break;
				case BUTTON:
					e->object->button->click_box.x = e->sizable->rect.x;
					e->object->button->click_box.y = e->sizable->rect.y;
					e->object->button->click_box.w = e->sizable->rect.w;
					e->object->button->click_box.h = e->sizable->rect.h;
					e->object->button->text_label->render_rect.x = e->sizable->rect.x + 10;
					e->object->button->text_label->render_rect.y = e->sizable->rect.y + 10;
					e->object->button->text_label->render_rect.w = e->sizable->rect.w - 10;
					e->object->button->text_label->render_rect.h = e->sizable->rect.h - 10;
					break;							  
				case IMAGE:
					vector2d_copy(e->object->image->position, vector2d(e->sizable->rect.x, e->sizable->rect.y));
					e->object->image->scale = vector2d(e->sizable->rect.w/500.0, e->sizable->rect.h/500.0);
					break;
				case  TEXT_INPUT:
					break;
				default:
					break;
			}
		}
	}
}

void editable_render(Editable* e)
{
	if (!e) return;

	if(e->object) ui_object_render(e->object);

	if(e->draggable) ui_draggable_render(e->draggable);
	if(e->sizable) ui_sizable_render(e->sizable);
}

void editable_free(Editable* e)
{
	ui_object_free(e->object);
	ui_draggable_free(e->draggable);
	ui_sizable_free(e->sizable);
	e->_inuse = 0;

	memset(e, 0, sizeof(Editable));
}