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

typedef char byte;
typedef byte event;

struct LmEvent
{
	/*
	 * the parameter is the user serialized it's indicate thta he is ready to begin a session
	 */
	static const event UserIsReady = 0;
	/*
	 *
	 */
	static const event E1 = 1;

	static const event E2 = 1;
};

typedef struct LmEvent LmEvent;

struct bytes
{
	byte* data;
	int len;
};

typedef struct bytes bytes;

#endif /* HELPERS_H_ */
