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

#include "string"

class LmUser
{

public:

	LmUser();
	~LmUser();

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

private:

	std::string m_pUserName;
	std::string m_pUserTabletName;
	int m_iScore;
	bool m_bParent;
	bool m_bMale;

};

#endif /* CLASSES_SOURCES_LMUSER_H_ */
