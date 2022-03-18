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
//#include "game_state.h"

const Uint32 WINDOW_HEIGHT = 720;
const Uint32 WINDOW_WIDTH  = 1200;

int main(int argc, char * argv[])
{
    /*variable declarations*/
    Uint8 done = 0;
    const Uint8 * keys;
    float mf = 0;
    
    game_state_id current_state, new_state;
    Sprite *bg_current, *bg_default;

    int mx,my;
    Uint32 mouse_state;

    event_id current_event_id, new_event_id;
    Event* event_object;

    mini_id current_mini_id = NO_GAME;
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

    /*event initialization*/
    code_vomit_add_all_events();
    
    /*set default background*/
    bg_default = gf2d_sprite_load_image("assets/images/backgrounds/bg_black.png");
    bg_current = bg_default;

    current_event_id = ASTEROIDS_AHEAD;
    current_state = NONE;
    current_mini_id = NO_GAME;

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
        entity_manager_think_mini(current_mini_id);
        
        event_object = get_event_by_id(current_event_id);
        
        new_state = event_listen(event_object, mouse_state, &mx, &my);
        
        if (new_state != NONE) {
            current_state = new_state;
        }

        /*if (mouse_state == 1 && !mini_asteroid->is_running)
        {
            current_mini_id = mini_asteroid->id;
            bg_current = mini_asteroid->background;
            mini_thread = SDL_CreateThread(mini_asteroid->run, "Asteroid Dodge Game Thread", mini_asteroid);
            slog("Mini Game Thread Started");
        }*/

        gf2d_graphics_clear_screen();// clears drawing buffers
        
        // all drawing should happen betweem clear_screen and next_frame
        //Draw backgrounds first
        gf2d_sprite_draw_image(bg_current,vector2d(0,0));
            
        //Draw game elements

        entity_manager_draw_mini(current_mini_id);
            
        //Draw UI elements last
        
        event_draw(event_object);
        
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    TTF_Quit();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
