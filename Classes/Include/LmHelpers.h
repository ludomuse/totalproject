/*
 * LmHelpers.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#define PTR_TO_OBJ(PTR, OBJ_TYPE) *(static_cast<OBJ_TYPE*>(PTR))
#define WIFIFACADE LmJniCppFacade::getWifiFacade()

#include "LmBytesMessage.h"


typedef byte event;

struct LmEvent
{
	/*
	 * parameter is the user serialized it's indicate that he is ready to begin a session
	 */
	static const event UserIsReady = 0;

	/*
	 * parameters are user1 and user2 wait the Play event to begin then
	 */
	static const event CompatibleToPlay = 1;

	/*
	 * parameter is a bool true the game begin
	 */
	static const event Play = 2;


	/*
	 *
	 */
	static const event E1 = 89;

	static const event E2 = 66;
};

typedef struct LmEvent LmEvent;

/*struct bytes
{
	byte* data;
	int len;
};

typedef struct bytes bytes;*/

#endif /* HELPERS_H_ */
