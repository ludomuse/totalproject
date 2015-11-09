/*
 * LmStatistics.cpp
 *
 *  Created on: 9 nov. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmStatistics.h"

LmStatistics* LmStatistics::m_pLmStatistics = nullptr;

using namespace rapidjson;

LmStatistics::LmStatistics()
{

}

LmStatistics::~LmStatistics()
{

}

LmStatistics* LmStatistics::getInstance()
{
	if (!m_pLmStatistics)
	{
		m_pLmStatistics = new LmStatistics;
		m_pLmStatistics->init();
	}

	return m_pLmStatistics;
}

void LmStatistics::init()
{
	// define the document as an object rather than an array
	m_oDocument.SetObject();

//	m_oAllocator = m_oDocument.GetAllocator();
}


