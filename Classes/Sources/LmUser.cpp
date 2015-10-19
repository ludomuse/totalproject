/*
 * LmUser.cpp
 *
 *  Created on: 9 sept. 2015
 *      Author: IHMTEKDev4
 */

//use for the debug
#define COCOS2D_DEBUG 1

#include "../Include/LmUser.h"


using namespace std;

LmUser::LmUser()
{

	//primitive type
	m_iScore = 0;
	m_bMale = true;
	m_bParent = true;

	//pointer

}

LmUser::~LmUser()
{

}

void LmUser::addToScore(int points)
{
	m_iScore += points;
}

std::string LmUser::getUserSerialized()
{
	std::string l_sUserSerialized = "";
	std::string l_sBuffer;

	//m_pUserName
	l_sBuffer = m_pUserName.c_str();
	l_sUserSerialized += l_sBuffer;
	l_sUserSerialized += m_cSeparator;
	//m_pUserTabletName
	l_sBuffer = m_pUserTabletName.c_str();
	l_sUserSerialized += l_sBuffer;
	l_sUserSerialized += m_cSeparator;
	//m_bMale
	l_sBuffer = boolToString(m_bMale);
	l_sUserSerialized += l_sBuffer;
	l_sUserSerialized += m_cSeparator;
	//m_bParent
	l_sBuffer = boolToString(m_bParent);
	l_sUserSerialized += l_sBuffer;
	l_sUserSerialized += m_cSeparator;
	//m_iScore
	char l_cBufferChar[20];
	sprintf(l_cBufferChar, "%d", m_iScore);
	l_sBuffer = l_cBufferChar;
	l_sUserSerialized += l_sBuffer;

	return l_sUserSerialized;
}

void LmUser::setUser(std::string l_sUserSerialized)
{
	std::stringstream ss(l_sUserSerialized);
	std::string item;

	int l_iCounter=0;

	while (getline(ss, item, m_cSeparator))
	{

		switch (l_iCounter)
		{
		case 0:
			m_pUserName = item;
			break;
		case 1:
			m_pUserTabletName = item;
			break;
		case 2:
			m_bMale = stringToBool(item);
			break;
		case 3:
			m_bParent = stringToBool(item);
			break;
		case 4:
			m_iScore = stringToInt(item);
			break;
		default:
			break;
		}
		l_iCounter++;
	}
}

bool LmUser::stringToBool(std::string string)
{
	bool result;

	if (!strcmp("true", string.c_str()))
	{
		result = true;
	}
	else if (!strcmp("false", string.c_str()))
	{
		result = false;
	}
	else
	{
		CCLOG("User Parsing Error");
	}

	return result;
}

int LmUser::stringToInt(std::string string)
{
	const char* Text = string.c_str(); // string to be converted

	int Result; // number which will contain the result

	int Succeeded = sscanf(Text, "%d", &Result);

	if (!Succeeded || Succeeded == EOF) // check if something went wrong during the conversion
		Result = 0;

	return Result;
}

 void LmUser::writeOn(bytes* msg)
 {
	 msg->write(getUserSerialized());
 }

 void LmUser::readOn(bytes* msg)
 {
	 CCLOG("reading on msg");
	 setUser(msg->readString());
 }

bool LmUser::operator==(const LmUser& other)const
{

	if(strcmp(other.m_pUserName.c_str(),this->m_pUserName.c_str()))
	{
		return false;
	}
	else if(strcmp(other.m_pUserTabletName.c_str(),this->m_pUserTabletName.c_str()))
	{
		return false;
	}
	else if(other.m_iScore!=this->m_iScore)
	{
		return false;
	}
	else if(other.m_bParent != this->m_bParent)
	{
		return false;
	}
	else if(other.m_bMale!=this->m_bMale)
	{
		return false;
	}

    return true;
}

