/*
 * LmStatistics.h
 *
 *  Created on: 9 nov. 2015
 *      Author: IHMTEKDev4
 */

#ifndef CLASSES_INCLUDE_LMSTATISTICS_H_
#define CLASSES_INCLUDE_LMSTATISTICS_H_

#include "../../cocos2d/external/json/document.h"
#include "../../cocos2d/external/json/stringbuffer.h"
#include "../../cocos2d/external/json/writer.h"

class LmStatistics
{

public:

	static LmStatistics* getInstance();

private:

	LmStatistics();
	~LmStatistics();

	//ATTRIBUTES

	static LmStatistics* m_pLmStatistics;

	// document is the root of a json message
	rapidjson::Document m_oDocument;

	// must pass an allocator when the object may need to allocate memory
	rapidjson::Document::AllocatorType& m_oAllocator;

	//METHODS
	void init();

	//write in json key - value
	void write(std::string,int);
};

#endif /* CLASSES_INCLUDE_LMSTATISTICS_H_ */
