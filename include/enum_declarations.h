#ifndef __ENUM_DECLARATIONS_H__
#define __ENUM_DECLARATIONS_H__


typedef enum {

	NONE,
	PROBLEM_STATE,

	/*Menu States*/
	PAUSE,
	CREW_SELECT,
	CREW_VIEW,
	MAP,

	/* Notification States */
	THRESHOLD_NOTE_START,  // used to divide gamestates in game loop
	NOTE_RESULT,
	NOTE_CREW_DEATH,

	SUCCESS_ASTEROID_DODGE,
	FAIL_ASTEROID_DODGE,
	AD_LONG_WAY,
	SUCCESS_RATION_SPLIT,
	FAIL_RATION_SPLIT,
	RS_OMNOMNOM,
	RS_FIGHT,
	SUCCESS_MOUSE_HUNT,
	FAIL_MOUSE_HUNT,
	MOUSE_FRIEND,
	MOUSE_TRAPPED,
	AI_ACCEPT_DEFEAT,
	AI_PUNCH,
	AI_FIXED,
	RM_DEAL_WITH_IT,
	RM_BLAME,

	/* Decision States */
	THRESHOLD_DECISION_START, // used to divide gamestates in game loop
	EVENT_CREW_STARVING,
	EVENT_CREW_BREAKDOWN,
	EVENT_ASTEROIDS_AHEAD,
	EVENT_RATIONS_LOW,
	EVENT_RATIONS_MISSING,
	EVENT_MOUSE_FOUND,
	EVENT_AI_TAKEOVER,
	
	/* Mini Game States */	
	THRESHOLD_MINI,  // used to divide gamestates in game loop
	MINI_ASTEROID_DODGE,
	MINI_RATION_SPLIT,
	MINI_MOUSE_HUNT,

}gamestate_id;

typedef enum {
	DEFAULT,
	
	PILOT,
	DOCTOR,
	ENGINEER,
	PHYSICOLOGIST,
	PROGRAMMER,
	PAINTER,
	COMEDIAN,
	MUSICIAN,
	NUTRITIONIST
}event_clearance;


#endif