/*
 * LmSettings.cpp
 *
 *  Created on: 3 nov. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmSettings.h"

#include "../Include/LmGameManager.h"

#include "../Include/LmJsonParser.h"


using namespace cocos2d;

LmSettings::LmSettings()
{

	//primitive type
	m_pLmCredits = nullptr;

	//pointer
	m_pBackGroundSprite = nullptr;

}

LmSettings::~LmSettings()
{
	delete m_pLmCredits;
}

bool LmSettings::init()
{

	//init credit scene
	m_pLmCredits = new LmCredits;

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();

	//sprite background
	m_pBackGroundSprite = LmSprite::create("Ludomuse/Background/menu.png");
	m_pBackGroundSprite->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.5));

	//menu
	auto l_pMenu = Menu::create();
	l_pMenu->setPosition(Vec2::ZERO);
	m_pBackGroundSprite->addChild(l_pMenu);

	//end button
	auto l_pMenuItemImageEndApplication = MenuItemImage::create(
			"Ludomuse/GUIElements/fin.png", "Ludomuse/GUIElements/finpress.png",
			CC_CALLBACK_1(LmSettings::endApplicationCallbackButton, this));
	l_pMenuItemImageEndApplication->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.2));
	l_pMenu->addChild(l_pMenuItemImageEndApplication);

	//back to dashboard button
	auto l_pMenuItemImageBack = MenuItemImage::create(
			"Ludomuse/GUIElements/playNormal.png",
			"Ludomuse/GUIElements/playPressed.png",
			CC_CALLBACK_1(LmSettings::back, this));
	l_pMenuItemImageBack->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.4));
	l_pMenu->addChild(l_pMenuItemImageBack);

	//slider to control background volume
	auto l_pVolumeBackgroundSlider = cocos2d::extension::ControlSlider::create(
			"Ludomuse/Content/timer.png", "Ludomuse/Content/timer.png",
			"Ludomuse/GUIElements/redstar.png");
	l_pVolumeBackgroundSlider->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.6));
	l_pVolumeBackgroundSlider->setMinimumValue(0);
	l_pVolumeBackgroundSlider->setMaximumValue(1);
	l_pVolumeBackgroundSlider->setValue(1.0);
	l_pVolumeBackgroundSlider->addTargetWithActionForControlEvents(this,
			cccontrol_selector(
					LmSettings::valueChangedCallbackBackgroundVolume),
			cocos2d::extension::Control::EventType::VALUE_CHANGED);
	m_pBackGroundSprite->addChild(l_pVolumeBackgroundSlider);

	//slider to control effect volume
	auto l_pVolumeEffectSlider = cocos2d::extension::ControlSlider::create(
			"Ludomuse/Content/timer.png", "Ludomuse/Content/timer.png",
			"Ludomuse/GUIElements/redstar.png");
	l_pVolumeEffectSlider->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.7));
	l_pVolumeEffectSlider->setMinimumValue(0);
	l_pVolumeEffectSlider->setMaximumValue(1);
	l_pVolumeEffectSlider->setValue(1.0);
	l_pVolumeEffectSlider->addTargetWithActionForControlEvents(this,
			cccontrol_selector(LmSettings::valueChangedCallbackEffectVolume),
			cocos2d::extension::Control::EventType::VALUE_CHANGED);
	m_pBackGroundSprite->addChild(l_pVolumeEffectSlider);

	//credits button
	auto l_pMenuItemImageCredits = MenuItemImage::create(
			"Ludomuse/GUIElements/playNormal.png",
			"Ludomuse/GUIElements/playPressed.png",
			CC_CALLBACK_1(LmSettings::runCredits, this));
	l_pMenuItemImageCredits->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.9));
	l_pMenu->addChild(l_pMenuItemImageCredits);

	m_pLmCredits->initCredits();

	return true;
}

void LmSettings::valueChangedCallbackBackgroundVolume(Ref* sender,
		cocos2d::extension::Control::EventType event)
{
	float value =
			static_cast<cocos2d::extension::ControlSlider*>(sender)->getValue();

	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(
			value);
}

void LmSettings::valueChangedCallbackEffectVolume(Ref* sender,
		cocos2d::extension::Control::EventType event)
{
	float value =
			static_cast<cocos2d::extension::ControlSlider*>(sender)->getValue();

	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(value);
}

void LmSettings::endApplicationCallbackButton(Ref* p_Sender)
{
	//play button clicked sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	endApplication();
}

void LmSettings::back(Ref* p_Sender)
{
	//play button clicked sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	setVisible(false);
}

void LmSettings::endApplication()
{
	CCLOG("LudoMuse End");
	WIFIFACADE->clean();
	Director::getInstance()->end();
}

void LmSettings::addTo(Node* parent, int zOrder)
{
	parent->addChild(m_pBackGroundSprite, zOrder);
}

void LmSettings::setVisible(bool visible)
{
	m_pBackGroundSprite->setVisible(visible);
	m_pBackGroundSprite->setBTouchEnabled(visible);
}

void LmSettings::addChild(cocos2d::Node* child)
{
	m_pBackGroundSprite->addChild(child);
}

void LmSettings::runCredits(Ref* p_Sender)
{

	//play button clicked sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	m_pLmCredits->run();
}

LmCredits::LmCredits()
{

	CCLOG("a");
	//prmitive type
	m_bLayerTouched = false;
	CCLOG("b");

	m_pSpeed=60;
	CCLOG("c");

	//pointer
	m_pListener = nullptr;
	CCLOG("d");

	m_pLabel = nullptr;
}

LmCredits::~LmCredits()
{


	//remove listener
	Director::getInstance()->getEventDispatcher()->removeEventListener(
			m_pListener);
}

void LmCredits::initCredits()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();

	//Creates a fullscreen black layer
	auto l_pLayerBackground = LayerColor::create();
	addChild(l_pLayerBackground);

	//init the background
	auto l_pSpriteBackground = Sprite::create( "Ludomuse/Background/splash.png");
	l_pSpriteBackground->setPosition(l_oVisibleSize.width * 0.5f ,
			l_oVisibleSize.height * 0.5f );
	l_pLayerBackground->addChild(l_pSpriteBackground);

	//init its listener
	m_pListener = EventListenerTouchOneByOne::create();
	m_pListener->onTouchBegan = CC_CALLBACK_2(LmCredits::onTouchBeganCredits,
			this);
	m_pListener->setSwallowTouches(true);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
			m_pListener, l_pLayerBackground);

	//init text
	initText();

	//init label
	m_pLabel = Label::createWithTTF(m_sCreditsText, "Fonts/JosefinSans-Regular.ttf",
			l_oVisibleSize.width * 0.05);
	m_pLabel->setAnchorPoint(Vec2(0.5, 1));
	m_pLabel->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, 0));
	m_pLabel->setColor(Color3B::BLACK);
	m_pLabel->setAlignment(TextHAlignment::CENTER);
	m_pLabel->setMaxLineWidth(l_oVisibleSize.width * 0.8);
	l_pLayerBackground->addChild(m_pLabel);

}

void LmCredits::initText()
{
	//ludomuse object
	auto l_pLmJsonParser = new LmJsonParser;

	if(!l_pLmJsonParser->initJsonDocument("Json/strings.json"))
	{
		CCLOG("init json credit failed");
	}

	m_sCreditsText = l_pLmJsonParser->getStringValue("CreditsText");


	delete l_pLmJsonParser;

}

void LmCredits::run()
{

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();

	m_bLayerTouched = false;
	Director::getInstance()->pushScene(TransitionFade::create(0.5, this));

	float l_fDurationAction = m_pLabel->getContentSize().height/m_pSpeed;

	auto l_pMoveDown = MoveBy::create(l_fDurationAction,Vect(0,m_pLabel->getContentSize().height));

	m_pLabel->setPosition(
				Vec2(l_oVisibleSize.width * 0.5, 0));
	m_pLabel->runAction(l_pMoveDown);

}

bool LmCredits::onTouchBeganCredits(Touch* touch, Event* event)
{
	if (!m_bLayerTouched)
	{
		m_bLayerTouched = true;
		Director::getInstance()->popScene();
	}

	return true;
}
