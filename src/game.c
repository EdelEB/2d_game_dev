#include <SDL.h>
#include <SDL_ttf.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "tools.h"
#include "entity.h"
#include "mini_asteroid_dodge.h"
#include "event.h"

const Uint32 WINDOW_HEIGHT = 720;
const Uint32 WINDOW_WIDTH  = 1200;

int main(int argc, char * argv[])
{
    /*variable declarations*/
    Uint8 done = 0;
    const Uint8 * keys;
    Sprite *bg_current, *bg_asteroid, *bg_default;
    
    int mx,my;
    float mf = 0;
    Uint32 mouse_state;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

    mini_code current_mini_code = NO_GAME;
    MiniGame* mini_asteroid;
    SDL_Thread* mini_thread;

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        (WINDOW_WIDTH + 3000),
        (WINDOW_HEIGHT + 3000),
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    
    if (TTF_Init() == -1) //TTF is what allows for font rendering
    { 
        slog("TTF Initialization failed with ERROR:  %s", TTF_GetError()); 
    }

    //tile_set_manager_init(16); // This game does not really need a tile manager
    entity_manager_init(1024);
    event_manager_init(20);
    
    /*minigame intializations*/
    mini_asteroid = mini_asteroid_init();
    code_vomit_add_all_events();

    /*preloads background images for later*/
    bg_default = gf2d_sprite_load_image("assets/images/backgrounds/bg_flat.png");
    bg_asteroid = gf2d_sprite_load_image("assets/images/backgrounds/bg_space.png");
    //mouse = gf2d_sprite_load_all("assets/images/pointer.png",32,32,16);
    //SDL_ShowCursor(SDL_DISABLE);

    bg_current = bg_default;
    

    TTF_Font* title_font = TTF_OpenFont("assets/fonts/SwanseaBold.ttf", 20);
    SDL_Color title_font_color = { 0,255,0,255 };
    SDL_Surface* title_text_surface = TTF_RenderText_Solid(title_font, "Playing Asteroid Dodge", title_font_color);
    SDL_Texture* title_text = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), title_text_surface);
    SDL_Rect title_rect;
    title_rect.x = 110;
    title_rect.y = 110;
    title_rect.w = 20;
    title_rect.h = 20;
    SDL_QueryTexture(title_text, NULL, NULL, &title_rect.w, &title_rect.h);

    SDL_Rect rect;
    rect.x = WINDOW_WIDTH/4;
    rect.y = WINDOW_HEIGHT/3;
    rect.w = WINDOW_WIDTH/2;
    rect.h = WINDOW_HEIGHT/8;
    SDL_FillRect(gf2d_graphics_create_surface(1000, 1000), &rect, 16);
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 8, 0, 0, 0);
    SDL_RenderPresent(gf2d_graphics_get_renderer());

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        mouse_state = SDL_GetMouseState(&mx,&my);
        //slog("%i", mouse_state);
                                     
        /*update things here*/
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        entity_manager_think_mini(current_mini_code);
        
        /*if (mouse_state == 1 && mx > rect.x && mx < rect.x + rect.w && my > rect.y && my < rect.y + rect.h)
        {
            slog("IN :  (%i, %i)", mx, my);
        }
        else if (mouse_state) {
            slog("OUT:  (%i, %i)", mx, my);
        }*/
        if (mouse_state == 1 && !mini_asteroid->is_running)
        {
            current_mini_code = ASTEROID_DODGE;
            bg_current = bg_asteroid;
            mini_thread = SDL_CreateThread(mini_asteroid->run, "Asteroid Dodge Game Thread", mini_asteroid);
            slog("Mini Game Thread Started");
        }

        gf2d_graphics_clear_screen();// clears drawing buffers
        
        // all drawing should happen betweem clear_screen and next_frame
        //Draw backgrounds first
        gf2d_sprite_draw_image(bg_current,vector2d(0,0));
            
        //Draw game elements
        
        SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &rect);
        SDL_RenderCopy(gf2d_graphics_get_renderer(), title_text, NULL, &title_rect);

        entity_manager_draw_mini(current_mini_code);
            
        //Draw UI elements last
        /* gf2d_sprite_draw(
            mouse,
            vector2d(mx,my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseColor,
            (int)mf);*/
        
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    TTF_Quit();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
