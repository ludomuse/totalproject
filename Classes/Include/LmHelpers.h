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
#define ON_CC_THREAD(FUN, OBJ, ...) 	Director::getInstance()->getScheduler()->performFunctionInCocosThread(\
										std::bind(&FUN, OBJ, ##__VA_ARGS__));

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
	 * parameter integer = index of next interaction ; to indicate the other tablet that we are ready for the ieme interaction
	 */
	static const event ReadyForNextInteraction = 3;

	/*
	 * parameter integer = index of next interaction ; to indicate the other tablet that we are ready for the ieme interaction
	 */
	static const event GoodAnswer = 4;

	/*
	 * to indicate the other user to replay the scene
	 */
	static const event Replay = 5;

	/*
	 * parameter : int idGamecomponent, bool well placed
	 */
	static const event Win = 6;

	/*
	 * when we send a gamecomponent parameter is the id of the gamecomponent
	 */
	static const event Gamecomponent = 7;

	/*
	 * parameter : int idGamecomponent, bool well placed
	 */
	static const event GamecomponentWellPlaced = 8;

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
