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
	menu_editor.editor_menu->object_list[1] = ui_create_button(200, 0, 200, 40, "New Label", NULL);
	menu_editor.editor_menu->object_list[2] = ui_create_button(400, 0, 200, 40, "New Button", prompt_button);
	ui_button_set_images(
		menu_editor.editor_menu->object_list[2]->button,
		"button_test",
		vector2d(0.30, 0.25),
		vector2d(1, 50),
		vector3d(0, 0, 0)
	);
	menu_editor.editor_menu->object_list[3] = ui_create_button(600, 0, 200, 40, "New Text_Box", NULL);
	menu_editor.editor_menu->object_list[4] = ui_create_button(800, 0, 200, 40, "New Image", NULL);
	menu_editor.editor_menu->object_list[5] = ui_create_button(1000, 0, 200, 40, "Save Menu", NULL);

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




gamestate_id prompt_button(void) 
{
	slog("Create Button");

	menu_editor.editor_menu->object_list[MAX_MENU_OBJECTS - 1] = ui_create_text_input(
		vector2d(100, 100), editable_create_button
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

void editable_create_label( char* str, Vector2D position, ui_label_type type)
{
	Editable* e = editable_new();

	if (!e) return;

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

void editable_create_image(char* filename, Vector2D position) 
{
	Editable* e = editable_new();

	if (!e) return;

	e->object = ui_create_image(filename, position, vector2d(0,0), vector2d(0,0), vector3d(0,0,0));
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = ui_create_sizable(position, vector2d(200,200))->sizable;
}

void editable_create_text_input(char* str, Vector2D position, ui_label_type type)
{
	Editable* e = editable_new();

	if (!e) return;

	e->object = ui_create_text_input(position, NULL);
	e->draggable = ui_create_draggable(position, vector2d(20, 20))->draggable;
	e->sizable = NULL;// ui_create_sizable(position, vector2d())->sizable;
}

void editable_listen(Editable* e, Uint32 mouse_state, int mx, int my)
{
	if (!e) return;

	//if (e->object) ui_object_listen(e->object);
	if (e->draggable) 
	{
		ui_draggable_listen(e->draggable, mouse_state, mx, my);
		
		if (e->draggable->is_held)
		{
			e->sizable->rect.x = e->draggable->position.x;
			e->sizable->rect.y = e->draggable->position.y;
			e->sizable->left->click_box.x	= e->sizable->rect.x;
			e->sizable->left->click_box.y	= e->sizable->rect.y + e->sizable->rect.h / 2 - SLIDER_POINT_DIM / 2;
			e->sizable->right->click_box.x	= e->sizable->rect.x + e->sizable->rect.w - SLIDER_POINT_DIM;
			e->sizable->right->click_box.y	= e->sizable->rect.y + e->sizable->rect.h / 2 - SLIDER_POINT_DIM / 2;
			e->sizable->top->click_box.x	= e->sizable->rect.x + e->sizable->rect.w / 2 - SLIDER_POINT_DIM / 2;
			e->sizable->top->click_box.y	= e->sizable->rect.y;
			e->sizable->bottom->click_box.x	= e->sizable->rect.x + e->sizable->rect.w / 2 - SLIDER_POINT_DIM / 2;
			e->sizable->bottom->click_box.y	= e->sizable->rect.y + e->sizable->rect.h - SLIDER_POINT_DIM;
			e->sizable->left->prev_position = vector2d(e->sizable->left->click_box.x, e->sizable->left->click_box.y);
			e->sizable->right->prev_position = vector2d(e->sizable->right->click_box.x, e->sizable->right->click_box.y);
			e->sizable->bottom->prev_position = vector2d(e->sizable->bottom->click_box.x, e->sizable->bottom->click_box.y);
			e->sizable->top->prev_position = vector2d(e->sizable->top->click_box.x, e->sizable->top->click_box.y);

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
					e->object->button->text_label->render_rect.x = e->draggable->position.x;
					e->object->button->text_label->render_rect.y = e->draggable->position.y;
					break;
				case IMAGE:
					vector2d_copy(e->object->image->position, e->draggable->position);
					break;
				case  TEXT_INPUT:
					vector2d_copy(e->object->text_input->position, e->draggable->position);
					break;
				default:
					break;
			}
		}

	}
		
	if (e->sizable)
	{
		ui_sizable_listen(e->sizable, mouse_state, mx, my);
			
		if (e->sizable->is_held)
		{
			e->draggable->position = vector2d(e->sizable->rect.x, e->sizable->rect.y);
			e->draggable->click_box.x = e->sizable->rect.x;
			e->draggable->click_box.y = e->sizable->rect.y;

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
				e->object->button->text_label->render_rect.x = e->sizable->rect.x;
				e->object->button->text_label->render_rect.y = e->sizable->rect.y;
				break;							  
			case IMAGE:
				//vector2d_copy(e->object->image->position, vector2d(e->sizable->rect.x, e->sizable->rect.y));
				//e->object->image->scale;
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