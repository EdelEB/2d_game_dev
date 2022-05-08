#include "menu_editor.h"

MenuEditor menu_editor = {0};

void menu_editor_init()
{
	menu_editor.editor_menu = menu_new();
	menu_editor.working_menu = menu_new();

	if (!menu_editor.editor_menu || !menu_editor.working_menu){
		slog("Couldnt' get Menu* to initialize menu_editor");
		return;
	}
	
	menu_editor.editor_menu->object_list[0] = ui_create_button(0, 0, 200, 40, "New Menu", menu_editor_new_menu);
	menu_editor.editor_menu->object_list[1] = ui_create_button(200, 0, 200, 40, "New Label", prompt_label);
	menu_editor.editor_menu->object_list[2] = ui_create_button(400, 0, 200, 40, "New Button", prompt_button);
	menu_editor.editor_menu->object_list[3] = ui_create_button(600, 0, 200, 40, "New Text_Box", editable_create_text_input);
	menu_editor.editor_menu->object_list[4] = ui_create_button(800, 0, 200, 40, "New Image", prompt_image);
	menu_editor.editor_menu->object_list[5] = ui_create_button(1000, 0, 200, 40, "Save Menu", NULL);

	ui_button_set_images(
		menu_editor.editor_menu->object_list[0]->button,
		BUTTON_STANDARD_FILE,
		vector2d(0.30, 0.25),
		vector2d(1, 50),
		vector3d(0, 0, 0)
	);
	ui_button_set_images(
		menu_editor.editor_menu->object_list[1]->button,
		BUTTON_STANDARD_FILE,
		vector2d(0.30, 0.25),
		vector2d(1, 50),
		vector3d(0, 0, 0)
	);
	ui_button_set_images(
		menu_editor.editor_menu->object_list[2]->button,
		BUTTON_STANDARD_FILE,
		vector2d(0.30, 0.25),
		vector2d(1, 50),
		vector3d(0, 0, 0)
	);
	ui_button_set_images(
		menu_editor.editor_menu->object_list[3]->button,
		BUTTON_STANDARD_FILE,
		vector2d(0.30, 0.25),
		vector2d(1, 50),
		vector3d(0, 0, 0)
	);
	ui_button_set_images(
		menu_editor.editor_menu->object_list[4]->button,
		BUTTON_STANDARD_FILE,
		vector2d(0.30, 0.25),
		vector2d(1, 50),
		vector3d(0, 0, 0)
	);
	ui_button_set_images(
		menu_editor.editor_menu->object_list[5]->button,
		BUTTON_STANDARD_FILE,
		vector2d(0.30, 0.25),
		vector2d(1, 50),
		vector3d(0, 0, 0)
	);


	slog("menu editor initialized");
}

gamestate_id menu_editor_listen(Uint32 mouse_state, int mx, int my, Uint8* keys)
{
	int i;

	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (!menu_editor.editable_list[i]._inuse) continue;
		editable_listen(&menu_editor.editable_list[i], mouse_state, mx, my, keys);
	}

	return menu_listen(menu_editor.editor_menu, mouse_state, &mx, &my, keys);
}

void menu_editor_render() 
{
	int i;

	for (i = 0; i < MAX_MENU_OBJECTS; i++)
	{
		if (!menu_editor.editable_list[i]._inuse) continue;
		editable_render(&menu_editor.editable_list[i]);
	}

	menu_draw(menu_editor.editor_menu);
	menu_draw(menu_editor.working_menu);
	return;
}

void menu_editor_save_menu() {}

void menu_editor_new_menu() 
{
	slog("New Menu");
}




void prompt_label(void) 
{
	menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_text_input(
		vector2d(100, 100), editable_create_label
	);
}

void prompt_button(void)
{
	menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_text_input(
		vector2d(100, 100), editable_create_button
	);
}

void prompt_image(void) {
	menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_text_input(
		vector2d(100, 100), editable_create_image
	);
}




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
	if (tbox->id == TEXT_INPUT) {
		sprintf(str, "%s", tbox->text_input->str);
		ui_object_free(tbox);
	}
	else {
		slog("editable_create_label failed");
		return;
	}

	e->object = ui_create_label(str, position.x, position.y, type);
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = ui_create_sizable(position, vector2d(100,100))->sizable;
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
	e->draggable = ui_create_draggable(position, vector2d(50, 50))->draggable;
	e->sizable = ui_create_sizable(position, vector2d(200,200))->sizable;
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
		return;
	}


	e->object = ui_create_image(filename, position, vector2d(0.2,0.2), vector2d(1,1), vector3d(0,0,0));
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = ui_create_sizable(position, vector2d(100,100))->sizable;
}

void editable_create_text_input()
{
	Vector2D position = position = vector2d(WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1);
	Editable* e = editable_new();

	if (!e) return;

	e->object = ui_create_text_input(position, NULL);
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

	
	if(e->draggable) ui_draggable_render(e->draggable);
	if(e->sizable) ui_sizable_render(e->sizable);
	if(e->object) ui_object_render(e->object);
}