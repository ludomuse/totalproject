/*
 * LmQuestion.cpp
 *
 *  Created on: 29 sept. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmQuestion.h"

LmQuestion::LmQuestion(const LmQuestionSeed &l_Seed)
{
	m_sQuestion = l_Seed.Question;
	m_sAnswer1 = l_Seed.Answer1;
	m_sAnswer2 = l_Seed.Answer2;
	m_sAnswer3 = l_Seed.Answer3;
	m_sAnswer4 = l_Seed.Answer4;
	m_iNumberGoodAnswer = l_Seed.NumberGoodAnswer;
}

LmQuestion::~LmQuestion()
{

}

