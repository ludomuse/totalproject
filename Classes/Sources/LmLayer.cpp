//use for the debug
#define COCOS2D_DEBUG 1

#include "../Include/LmLayer.h"

USING_NS_CC;

LmLayer::LmLayer(const LmLayerSeed &Seed)
{
	m_aImages = Seed.Images;
	m_sSoundURL = Seed.SoundURL;
	m_sText = Seed.Text;
	m_fFontSize = Seed.FontSize;
	m_fHeightPercent = Seed.HeightPercent;
	m_fWidthPercent = Seed.WidthPercent;
	m_fSizePercent = Seed.SizePercent;
	m_oColorText = Seed.ColorText;
}

LmLayer::~LmLayer()
{

}

bool LmLayer::init()
{

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	/*
	 * CENTER = 0
	 * TOP LEFT = 1
	 * TOP RIGHT = 2
	 * BOTTOM LEFT = 3
	 * BOTTOM RIGHT = 4
	 */

	//we add sprites
	for (std::vector<std::pair<std::string, int>>::iterator it =
			m_aImages.begin(); it != m_aImages.end(); ++it)
	{
		auto l_oSprite = Sprite::create(it->first);
		switch (it->second)
		{

		//center
		case 0:
			l_oSprite->setPosition(l_oVisibleSize.width / 2 + l_oOrigin.x,
					l_oVisibleSize.height / 2 + l_oOrigin.y);
			break;

			//top left
		case 1:
			l_oSprite->setAnchorPoint(Vec2(0, 1));
			l_oSprite->setPosition(l_oOrigin.x,
					l_oVisibleSize.height + l_oOrigin.y);
			break;

			//top right
		case 2:
			l_oSprite->setAnchorPoint(Vec2(1, 1));
			l_oSprite->setPosition(l_oVisibleSize.width + l_oOrigin.x,
					l_oVisibleSize.height + l_oOrigin.y);
			break;

			//bottom left
		case 3:
			l_oSprite->setAnchorPoint(Vec2(0, 0));
			l_oSprite->setPosition(l_oOrigin.x, +l_oOrigin.y);
			break;

			//bottom right
		case 4:
			l_oSprite->setAnchorPoint(Vec2(1, 0));
			l_oSprite->setPosition(l_oVisibleSize.width + l_oOrigin.x,
					+l_oOrigin.y);
			break;

		default:
			break;
		}
		addChild(l_oSprite, 0);
	}

	//we preload the sound
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(
			m_sSoundURL.c_str());

	//we add text to the center of the screen
	auto l_pLabel = Label::createWithTTF(m_sText,
			"Fonts/JosefinSans-Regular.ttf", m_fFontSize);
	l_pLabel->setPosition(l_oVisibleSize.width * m_fWidthPercent,
			l_oVisibleSize.height * m_fHeightPercent);
	l_pLabel->setColor(m_oColorText);
	l_pLabel->setAlignment(TextHAlignment::CENTER);
	l_pLabel->setMaxLineWidth(l_oVisibleSize.width * m_fSizePercent);
	addChild(l_pLabel);

	return true;
}

void LmLayer::playSound()
{

	//play sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
			m_sSoundURL.c_str(), false);

}

void LmLayer::pauseSound()
{
	//pause sound
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

