/*
 * LmReward.cpp
 *
 *  Created on: 1 oct. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmReward.h"

using namespace cocos2d;

LmReward::LmReward(const LmRewardSeed& l_oSeed)
{
	m_sFilenameSpriteBackground = l_oSeed.FilenameSpriteBackground;
	m_sFilenameSpriteReward = l_oSeed.FilenameSpriteReward;
	m_iRewardScore = l_oSeed.RewardScore;
	m_sFilenameSound = l_oSeed.FilenameSound;
	m_sText = l_oSeed.Text;
	m_oColorText = l_oSeed.ColorText;
	m_fFontSize = l_oSeed.FontSize;
	m_fWidthPercent = l_oSeed.WidthPercent;
	m_fHeightPercent = l_oSeed.HeightPercent;
	m_fSizePercent = l_oSeed.SizePercent;

	//pointer
	m_pSpriteReward = nullptr;
	m_pLabekReward = nullptr;
}

LmReward::LmReward(LmReward* reward)
{
	m_sFilenameSpriteBackground =
			reward->getSFilenameSpriteBackground().c_str();
	m_sFilenameSpriteReward = reward->getSFilenameSpriteReward().c_str();
	m_sFilenameSound = reward->getSFilenameSound().c_str();
	m_iRewardScore = reward->getIRewardScore();
	m_sText = reward->getSText().c_str();
	m_oColorText = reward->getOColorText();
	m_fFontSize = reward->getFFontSize();
	m_fWidthPercent = reward->getFWidthPercent();
	m_fHeightPercent = reward->getFHeightPercent();
	m_fSizePercent = reward->getFSizePercent();

	init();
}

LmReward::~LmReward()
{

}

void LmReward::init()
{

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//init the sprite component

	//if the reward is composed with a sprite
	if (strcmp(m_sFilenameSpriteReward.c_str(), ""))
	{
		CCLOG("init sprite reward");
		m_pSpriteReward = Sprite::create(m_sFilenameSpriteReward);
		m_pSpriteReward->setAnchorPoint(Vec2(0.5, 0.5));
	}

	//we preload the sound
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(
			m_sFilenameSound.c_str());

	//init label
	//we add text to the center of the screen
	m_pLabekReward = Label::createWithTTF(m_sText,
			"Fonts/JosefinSans-Regular.ttf", m_fFontSize);
	m_pLabekReward->setPosition(l_oVisibleSize.width * m_fWidthPercent,
			l_oVisibleSize.height * m_fHeightPercent);
	m_pLabekReward->setColor(m_oColorText);
	m_pLabekReward->setMaxLineWidth(l_oVisibleSize.width * m_fSizePercent);

}

void LmReward::playRewardSound()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
			m_sFilenameSound.c_str(), false);
}
