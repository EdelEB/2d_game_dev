#include <SDL.h>
#include <SDL_ttf.h>

#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "mini_asteroid_dodge.h"
#include "director.h"
//#include "game_state.h"

const Uint32 WINDOW_HEIGHT = 720;
const Uint32 WINDOW_WIDTH  = 1200;

int main(int argc, char * argv[])
{
    /*variable declarations*/
    Uint8 done = 0;
    const Uint8 * keys;
    float mf = 0;
    
    
    Sprite *bg_current, *bg_default;

    int mx,my;
    Uint32 mouse_state;
    
    game_state_id current_game_state_id, new_game_state_id;
    Event* event_object;

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
    note_manager_init(50);
    director_init();
    
    /*minigame intializations*/
    //mini_asteroid = mini_asteroid_init();

    /*event initialization*/
    code_vomit_add_all_events();
    add_all_notes();

    /*set default background*/
    bg_default = gf2d_sprite_load_image("assets/images/backgrounds/bg_black.png");
    bg_current = bg_default;

    current_game_state_id = EVENT_ASTEROIDS_AHEAD;

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        mouse_state = SDL_GetMouseState(&mx,&my);
                                     
        mf += 0.1;
        if (mf >= 16.0)mf = 0;
        
        /*update things here*/
        new_game_state_id = director_think(current_game_state_id, mouse_state, &mx, &my);        
        
        if (new_game_state_id) {
            current_game_state_id = new_game_state_id;
        }

        /* clear drawing buffer */
        gf2d_graphics_clear_screen(); 

        /* Draw backgrounds first */
        //gf2d_sprite_draw_image(bg_current,vector2d(0,0));
            
        /* Draw game elements */
        director_draw(current_game_state_id); // director handles everything elements, UI, and backgrounds

        /* Draw UI elements last */
        
        /* render current draw frame and skip to the next frame */
        gf2d_grahics_next_frame();
        
        /*exit condition*/
        if (keys[SDL_SCANCODE_ESCAPE])done = 1;
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    TTF_Quit();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
