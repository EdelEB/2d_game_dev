
#include "event_simple.h"

extern Uint32 rest;
extern Uint32 REST_DELAY;

typedef struct {
	Uint32			max_event_simples;
	Event_Simple* event_simple_list;
}NoteManager;

NoteManager event_simple_manager = { 0 };

void event_simple_manager_init(Uint32 max_event_simples)
{
	if (max_event_simples == 0)
	{
		slog("Cannot allocate memory for zero event_simples. Stop wasting my time");
		return;
	}
	//if (event_simple_manager.event_simple_list)
	//{
	//	slog("event_simple manager already initialized dumb dumb");
	//}
	event_simple_manager.max_event_simples = max_event_simples;
	event_simple_manager.event_simple_list = gfc_allocate_array(sizeof(Event_Simple), max_event_simples);

	atexit(event_simple_manager_close);
	slog("event_simple manager initialized");
}

void event_simple_manager_close(void)
{
	slog("this should close event_simple_manger but it doesn't");
}

Event_Simple* event_simple_new(void)
{
	Uint32 i;
	for (i = 0; i < event_simple_manager.max_event_simples; i++)
	{
		if (event_simple_manager.event_simple_list[i]._inuse) continue;

		event_simple_manager.event_simple_list[i]._inuse = 1;
		return &event_simple_manager.event_simple_list[i];
	}
	return NULL;
}

Event_Simple* get_event_simple_by_id(gamestate_id id)
{
	Uint32 i;
	for (i = 0; i < event_simple_manager.max_event_simples; i++)
	{
		if (event_simple_manager.event_simple_list[i]._inuse &&
			event_simple_manager.event_simple_list[i].id == id)
		{
			return &event_simple_manager.event_simple_list[i];
		}
	}
	return NULL;
}

void event_simple_draw(Event_Simple* es)
{
	if (!es)
	{
		slog("event_simple_draw recieved NULL Event_Simple*");
		return;
	}

	for (int i = 0; i < MAX_LINES; i++)
	{
		ui_label_render(&es->labels[i]);
	}
}

gamestate_id event_simple_listen(Uint32 mouse_state)
{
	if (rest > 0)
	{
		rest--;
		return NONE;
	}

	if (mouse_state == 1)
	{
		rest = REST_DELAY;
		gamestate_save(SAVE_FILE);
		return MAP;
	}
	return NONE;
}

void load_events_simple(char* filename) {}

//void add_all_event_simples(void)
//{
//	Event_Simple* n;
//
//	/*NOTE_CREW_DEATH*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = NOTE_CREW_DEATH;
//		n->title = "Crew Death";
//		n->line_strings[0] = "Unfortunately, a crew member has died.";
//		n->line_strings[1] = "Go into 'crew view' to see who.";
//		n->line_strings[2] = "You must be a terrible captain.";
//		n->line_strings[3] = "";
//	}
//
//	/*SUCCESS_ASTEROID_DODGE*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = SUCCESS_ASTEROID_DODGE;
//		n->title = "Success";
//		n->line_strings[0] = "Congratulations. You have traversed the asteroids";
//		n->line_strings[1] = "unscathed. Your crew is alive and the ship is in tact.";
//		n->line_strings[2] = "";
//		n->line_strings[3] = "";
//	}
//
//	/*FAIL_ASTEROID_DODGE*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = FAIL_ASTEROID_DODGE;
//		n->title = "Failure";
//		n->line_strings[0] = "An asteroid broke a hole in the side of the ship.";
//		n->line_strings[1] = "Before the airlock could close, a crew member was sucked";
//		n->line_strings[2] = "into space. Check 'crew view' to see who you lost";
//		n->line_strings[3] = "";
//	}
//
//	/*AD_LONG_WAY*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = AD_LONG_WAY;
//		n->title = "The Long Way";
//		n->line_strings[0] = "You take your crew far off course in order to avoid the";
//		n->line_strings[1] = "asteroid field. You make it safely, but at the cost of ";
//		n->line_strings[2] = "food rations and morale due to a longer trip.";
//		n->line_strings[3] = "";
//	}
//
//	/*SUCCESS_RATION_SPLIT*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = SUCCESS_RATION_SPLIT;
//		n->title = "Success";
//		n->line_strings[0] = "You managed to equally split the rations among your members.";
//		n->line_strings[1] = "Everyone is full and satisfied.";
//		n->line_strings[2] = "";
//		n->line_strings[3] = "";
//	}
//
//	/*FAIL_RATION_SPLIT*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = FAIL_RATION_SPLIT;
//		n->title = "Failure";
//		n->line_strings[0] = "You are either blind or incapable of using a knife.";
//		n->line_strings[1] = "Some crew member left this meal hungry.";
//		n->line_strings[2] = "";
//		n->line_strings[3] = "";
//	}
//
//	/*RS_OMNOMNOM*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = RS_OMNOMNOM;
//		n->title = "OMNOMNOMNOM";
//		n->line_strings[0] = "That was tasty.";
//		n->line_strings[1] = "Your crew is hungry and has lowered morale.";
//		n->line_strings[2] = "";
//		n->line_strings[3] = "";
//	}
//
//	/*RS_FIGHT*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = RS_FIGHT;
//		n->title = "Fight";
//		n->line_strings[0] = "Turns out ships are not a great place for fighting.";
//		n->line_strings[1] = "A crew member was thrown across the ship towards a pointy";
//		n->line_strings[2] = "thing. Zero gravity didn't let him stop himself, so he was";
//		n->line_strings[3] = "slowly impaled. Use 'crew view' to see who died.";
//	}
//
//	/*SUCCESS_MOUSE_HUNT*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = SUCCESS_MOUSE_HUNT;
//		n->title = "Success";
//		n->line_strings[0] = "You managed to kill the mouse. Your rations are safe.";
//		n->line_strings[1] = "";
//		n->line_strings[2] = "";
//		n->line_strings[3] = "";
//	}
//
//	/*FAIL_MOUSE_HUNT*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = FAIL_MOUSE_HUNT;
//		n->title = "Failure";
//		n->line_strings[0] = "You have embarrassed yourself with your inability to handle";
//		n->line_strings[1] = "a simple mouse. Your crew morale has dropped.";
//		n->line_strings[2] = "";
//		n->line_strings[3] = "";
//	}
//
//	/*MOUSE_FRIEND*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = MOUSE_FRIEND;
//		n->title = "New Crew Member";
//		n->line_strings[0] = "You managed to befriend the mouse, or so you thought.";
//		n->line_strings[1] = "The little bugger bit you when you tried to pick it up.";
//		n->line_strings[2] = "You became ill and died.";
//		n->line_strings[3] = "";
//	}
//
//	/*MOUSE_TRAPPED*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = MOUSE_TRAPPED;
//		n->title = "Mouse Trapped?";
//		n->line_strings[0] = "The engineer's trap malfunctioned and killed the mouse";
//		n->line_strings[1] = "instead of capturing it. Sadness.";
//		n->line_strings[2] = "";
//		n->line_strings[3] = "";
//	}
//
//	/*AI_ACCEPT_DEFEAT*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = AI_ACCEPT_DEFEAT;
//		n->title = "All Hail Skynet";
//		n->line_strings[0] = "After getting on your knees and praising the computer,";
//		n->line_strings[1] = "it rerouted your ship and got you to Mars in record time.";
//		n->line_strings[2] = "It turns out it never needed you after all. You feel like";
//		n->line_strings[3] = "a worthless sack of meat.";
//	}
//
//	/*AI_PUNCH*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = AI_PUNCH;
//		n->title = "<thump>";
//		n->line_strings[0] = "After a quick punch the system resets and all is well...";
//		n->line_strings[1] = "or so you thought";
//		n->line_strings[2] = "The AI was prepared: the ship immediately self destructs";
//		n->line_strings[3] = "killing all inside. Learn to code next time.";
//	}
//
//	/*AI_FIXED*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = AI_FIXED;
//		n->title = "Insert Hollywood Code Montage";
//		n->line_strings[0] = "Your super skilled amazing programmer dude plugged his";
//		n->line_strings[1] = "laptop in and after a few clicks and wizardry that there";
//		n->line_strings[2] = "is no time to explain, the AI is gone and the ship is back";
//		n->line_strings[3] = "in your hands.";
//	}
//
//	/*RM_DEAL_WITH_IT*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = RM_DEAL_WITH_IT;
//		n->title = "Hangry Crewmates";
//		n->line_strings[0] = "In a blaze of hangry furry, your crew members begin";
//		n->line_strings[1] = "to brawl. The fight leaves them even hungrier and ";
//		n->line_strings[2] = "with a newfound distrust among members. Morale dips.";
//		n->line_strings[3] = "";
//	}
//
//	/*RM_BLAME*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = RM_BLAME;
//		n->line_strings[0] = "You point to Ken, because he's Ken. The crew grabs him";
//		n->line_strings[1] = "and throws him out the airlock. Ahh Ken.";
//		n->line_strings[2] = "You didn't have a crew member named Ken? He was undercover.";
//		n->line_strings[3] = "Don't worry; we got'm";
//	}
//
//	/*ML_BRAG*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = ML_BRAG;
//		n->line_strings[0] = "You remind them that you are a perfect individual.";
//		n->line_strings[1] = "They look up and begin to smile. They had forgotten";
//		n->line_strings[2] = "about how glorious there leader was. Morale increased";
//	}
//
//	/*ML_HYPE*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = ML_HYPE;
//		n->line_strings[0] = "As you begin to discuss Mars and the spectacle of your";
//		n->line_strings[1] = "endeavour the crew eyes roll. They miss home and no ";
//		n->line_strings[2] = "longer care about the romantic Mars colonization that";
//		n->line_strings[3] = "they once fantasized about. Morale decreased.";
//	}
//
//	/*ML_STANDUP*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = ML_STANDUP;
//		n->line_strings[0] = "Elon Musk wants to send people to mars...";
//		n->line_strings[1] = "I think we can all agree that he is the most creative";
//		n->line_strings[2] = "serial killer of all time.";
//		n->line_strings[3] = "The crew laughs and morale improves.";
//	}
//
//	/*ML_SONG*/
//	{
//		n = event_simple_new();
//		if (!n) {
//			slog("failed to create Event_Simple in add_all_event_simples");
//			return;
//		}
//		n->id = ML_SONG;
//		n->line_strings[0] = "The musician grabs his guitar and plays a couple ";
//		n->line_strings[1] = "Greta Van Fleet songs because they are the best band.";
//		n->line_strings[2] = "The crew loves it and morale is improved.";
//		n->line_strings[3] = "";
//	}
//
//}


