#include <SDL.h>
#include <SDL_ttf.h>

#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "director.h"

Uint8 DEBUG = 0;
const int CLICK_COOLDOWN = 20;
int debug_toggle_cooldown = 0;
int mouse_click_cooldown = 0;

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
    
    gamestate_id current_gamestate_id, new_gamestate_id;
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

    /*Initialize pretty much everything*/
    ui_font_info_init();
    director_init();
    
    /* Set default background */
    //bg_default = gf2d_sprite_load_image("assets/images/backgrounds/bg_map.png");
    //bg_current = bg_default;

    /* Set game start state */
    current_gamestate_id = MENU_START;

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        mouse_state = SDL_GetMouseState(&mx,&my);
        if (DEBUG && mouse_state == 1)
        { 
            if (mouse_click_cooldown > 0) {
                mouse_click_cooldown--;
                mouse_state = 0;
            }
            else {
                slog("mouse_clicked(%d, %d)", mx, my);
                mouse_click_cooldown = CLICK_COOLDOWN;
            }
        } 
        /*Toggle DEBUG*/
        if (keys[SDL_SCANCODE_P]) {
            if (debug_toggle_cooldown > 0) { debug_toggle_cooldown--; }
            else {
                if (DEBUG) DEBUG = 0;
                else DEBUG = 1;
                debug_toggle_cooldown = CLICK_COOLDOWN;
            }
        } 

        mf += 0.1;
        if (mf >= 16.0)mf = 0;
        
        /*This should only hit after a state change. (Although when DEBUG is on this will be weird)*/
        if (mouse_click_cooldown > 0) {
            mouse_state = 0;
            mouse_click_cooldown--;
        }

        /*update things here*/
        new_gamestate_id = director_think(current_gamestate_id, mouse_state, &mx, &my);        
        
        if (new_gamestate_id && new_gamestate_id != current_gamestate_id) 
        { 
            slog("STATE CHANGE %d -> %d", current_gamestate_id, new_gamestate_id);
            current_gamestate_id = new_gamestate_id;
            mouse_click_cooldown = CLICK_COOLDOWN; // makes sure there is no double click when a new state opens
        }

        /* clear drawing buffer */
        gf2d_graphics_clear_screen(); 

        /* Draw backgrounds first */
        //gf2d_sprite_draw_image(bg_current,vector2d(0,0));
            
        /* Draw game elements */
        director_draw(current_gamestate_id); // director handles everything elements, UI, and backgrounds

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
