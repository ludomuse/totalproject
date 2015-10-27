/*
 * LmReward.h
 *
 *  Created on: 1 oct. 2015
 *      Author: IHMTEKDev4
 *
 *
 */

#ifndef CLASSES_INCLUDE_LMREWARD_H_
#define CLASSES_INCLUDE_LMREWARD_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

struct LmRewardSeed
{
	std::string FilenameSpriteBackground;
	std::string FilenameSpriteReward;
	int RewardScore;
	std::string FilenameSound;
	std::string Text;
	cocos2d::Color3B ColorText;
	float FontSize;
	float WidthPercent;
	float HeightPercent;
	float SizePercent;
};

class LmReward
{
public:

	LmReward();
	LmReward(const LmRewardSeed&);
	LmReward(LmReward*);
	~LmReward();

	int getIRewardScore() const
	{
		return m_iRewardScore;
	}

	const std::string& getSFilenameSpriteBackground() const
	{
		return m_sFilenameSpriteBackground;
	}

	cocos2d::Sprite* getPSpriteReward() const
	{
		return m_pSpriteReward;
	}

	void init();

	//play the reward sound
	void playRewardSound();

	const std::string& getSFilenameSound() const
	{
		return m_sFilenameSound;
	}

	const std::string& getSFilenameSpriteReward() const
	{
		return m_sFilenameSpriteReward;
	}

	float getFFontSize() const
	{
		return m_fFontSize;
	}

	float getFHeightPercent() const
	{
		return m_fHeightPercent;
	}

	float getFSizePercent() const
	{
		return m_fSizePercent;
	}

	float getFWidthPercent() const
	{
		return m_fWidthPercent;
	}

	cocos2d::Color3B getOColorText() const
	{
		return m_oColorText;
	}

	cocos2d::Label* getPLabekReward() const
	{
		return m_pLabekReward;
	}

	const std::string& getSText() const
	{
		return m_sText;
	}

private:

	//ATTRIBUTES

	//get from json
	std::string m_sFilenameSpriteBackground;
	std::string m_sFilenameSpriteReward;
	int m_iRewardScore;
	std::string m_sFilenameSound;
	std::string m_sText;
	cocos2d::Color3B m_oColorText;
	float m_fFontSize;
	float m_fWidthPercent;
	float m_fHeightPercent;
	float m_fSizePercent;

	//sprite to add to the button
	cocos2d::Sprite* m_pSpriteReward;

	//label
	cocos2d::Label* m_pLabekReward;

	//METHODS

};

#endif /* CLASSES_INCLUDE_LMREWARD_H_ */
