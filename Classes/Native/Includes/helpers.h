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

struct bytes {
	byte* data;
	int len;
};

typedef struct bytes bytes;

#endif /* HELPERS_H_ */
