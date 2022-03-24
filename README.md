# Oregon Trail to Mars
-Edel Barcenas

Pick your crew members and begin your journey to Mars. Who you pick may come in handy later. 
Along the way you will have several encounters to deal with. Resolve them in 
the way that best suits you. However, be careful of crew members hunger and 
morale. There is only so much food to go around.

Controls:

Asteroid Dodge Minigame: use keys w, a, s, d to move

Mouse Hunt Minigame: try and click on the mouse as it runs across the screen

Ration Split Minigame: Using the mouse try to equally split the rectangle into 5 equal parts

Seeing Deliverables:

This stage of the project does not have all the parts connected yet, and some of the ways things are done are subpar at best. In order to view specific events/mini_games/menus you will need to change a variable in "game.c" On line 63 current_gamestate_id can be changed to the game_state_id of your choosing. A full list of the gamestates available can be seen in "enum_declarations.h" Crew members classes are not easily visible in the code or in any of the gamestates currently, however their flags are listed in "enum_declarations.h" as well; they are simple flags that determine which options show in decision events, however since the crew selection is not working yet they cannot be seen in action. Currently simple_json is still giving me issues which mean the map and resources declared in gamestate.h are still inactive and unused. 
