#include <SDL.h>
#include <SDL_ttf.h>
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
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
    Sprite *bg_current, *bg_default;
    
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

    
    bg_default = gf2d_sprite_load_image("assets/images/backgrounds/bg_black.png");
    //mouse = gf2d_sprite_load_all("assets/images/pointer.png",32,32,16);
    //SDL_ShowCursor(SDL_DISABLE);

    /*set default background*/
    bg_current = bg_default;

    MiniGame* AD = get_event_by_id(ASTEROIDS_AHEAD);

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

        /*if (mouse_state == 1 && !mini_asteroid->is_running)
        {
            current_mini_code = mini_asteroid->code;
            bg_current = mini_asteroid->background;
            mini_thread = SDL_CreateThread(mini_asteroid->run, "Asteroid Dodge Game Thread", mini_asteroid);
            slog("Mini Game Thread Started");
        }*/

        gf2d_graphics_clear_screen();// clears drawing buffers
        
        // all drawing should happen betweem clear_screen and next_frame
        //Draw backgrounds first
        gf2d_sprite_draw_image(bg_current,vector2d(0,0));
            
        //Draw game elements

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
        
        event_draw(AD);
        event_listen(AD, mouse_state, &mx, &my);

        
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    TTF_Quit();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
