#ifndef __CREW_H__
#define __CREW_H__

#include <SDL.h>
#include "simple_logger.h"
#include "enum_declarations.h"

typedef	struct CREW_MEMBER{
	char		   *name;			/**<name of crew member*/
	char		   *title;			/**<job, for example : pilot, engineer, phsycologist*/
	event_clearance clearance;		/**<code used to determine which options are provided during events*/
	Uint8			gender;			/**<  woman : 0 ; man : 1 */
	Uint16			hunger;			/**< 0-10 where 10 is the best*/
	Uint16			morale;			/**< 0-10, where 10 is the best*/
	Uint8			is_alive;		/**< alive : 1, dead : 0*/
	Uint8		   _inuse;			/**<flag used to determine if a slot is open in crew_manager*/
}crew_member;

gamestate_id crew_lower_morale();
gamestate_id crew_raise_morale();
gamestate_id crew_lower_hunger();
gamestate_id crew_raise_hunger();

#endif