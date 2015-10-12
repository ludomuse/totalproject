/*
 * helpers.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#define PTR_TO_OBJ(PTR, OBJ_TYPE) *(static_cast<OBJ_TYPE*>(PTR))



typedef char byte;
typedef byte event;

struct LmEvent
{
	static const event E1 = 0;
	static const event E2 = 1;
};

typedef struct LmEvent LmEvent;

struct bytes {
	byte* data;
	int len;
};

typedef struct bytes bytes;

#endif /* HELPERS_H_ */
