/*
 * LmUser.h
 *
 *  Created on: 9 sept. 2015
 *      Author: IHMTEKDev4
 *
 *      where are stocked user's data
 */

#ifndef CLASSES_SOURCES_LMUSER_H_
#define CLASSES_SOURCES_LMUSER_H_

#include <string>
#include <sstream>

//for debug
#include "cocos2d.h"


class LmUser
{

public:

	LmUser();
	~LmUser();

	//use for the serialization
	const char m_cSeparator = '|';

	std::string getPUserName() const
	{
		return m_pUserName;
	}

	void setPUserName(std::string pUserName)
	{
		m_pUserName = pUserName;
	}

	std::string getPUserTabletName() const
	{
		return m_pUserTabletName;
	}

	void setPUserTabletName(std::string pUserTabletName)
	{
		m_pUserTabletName = pUserTabletName;
	}

	int getPScore() const
	{
		return m_iScore;
	}

	void setPScore(int pScore)
	{
		m_iScore = pScore;
	}

	bool isBParent() const
	{
		return m_bParent;
	}

	void setBParent(bool bParent)
	{
		m_bParent = bParent;
	}

	void addToScore(int);

	void setBMale(bool bMale)
	{
		m_bMale = bMale;
	}

	std::string getUserSerialized();

	void setUser(std::string l_sUserSerialized);

	bool isBMale() const
	{
		return m_bMale;
	}

private:

	//ATTRIBUTES
	std::string m_pUserName;
	std::string m_pUserTabletName;
	int m_iScore;
	bool m_bParent;
	bool m_bMale;

	//METHODS
	inline std::string boolToString(bool b)
	{
		return b ? "true" : "false";
	}

	bool stringToBool(std::string);
	int stringToInt(std::string );


};

bool operator==(const LmUser& user1, const LmUser& user2)
{

	if(strcmp(user1.m_pUserName.c_str(),user2.m_pUserName.c_str()))
	{
		return false;
	}
	else if(strcmp(user1.m_pUserTabletName.c_str(),user2.m_pUserTabletName.c_str()))
	{
		return false;
	}
	else if(user1.m_iScore!=user2.m_iScore)
	{
		return false;
	}
	else if(user1.m_bParent != user2.m_bParent)
	{
		return false;
	}
	else if(user1.m_bMale!=user2.m_bMale)
	{
		return false;
	}

    return true;
}

#endif /* CLASSES_SOURCES_LMUSER_H_ */
