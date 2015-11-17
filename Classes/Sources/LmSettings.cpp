/*
 * LmSettings.cpp
 *
 *  Created on: 3 nov. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmSettings.h"

#include "../Include/LmGameManager.h"

#include "../Include/LmJsonParser.h"

#include "../../cocos2d/external/json/document.h"

using namespace cocos2d;

LmSettings::LmSettings(LmUser* l_pUser)
{

	//ludo object
	m_pUser = l_pUser;

	m_pLmCredits = nullptr;

	m_pLmStatsView = nullptr;

	//pointer
	m_pBackGroundSprite = nullptr;
}

LmSettings::~LmSettings()
{
	delete m_pLmCredits;
	delete m_pLmStatsView;
}

bool LmSettings::init()
{

	//init credit/stats scene
	m_pLmCredits = new LmCredits;
	m_pLmStatsView = new LmStatsView;

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

	//back to dashboard button
	auto l_pMenuItemImageBack = MenuItemImage::create(
			"Ludomuse/Content/backToDashboard.png",
			"Ludomuse/Content/backToDashboardpressed.png",
			CC_CALLBACK_1(LmSettings::back, this));
	l_pMenuItemImageBack->setPosition(
			Vec2(l_oVisibleSize.width * 0.9, l_oVisibleSize.height * 0.9));
	l_pMenu->addChild(l_pMenuItemImageBack);

	//label associate to slider background
	auto l_pLabelBackgroundSlider = Label::createWithTTF("Volume Principal",
			"Fonts/JosefinSans-Bold.ttf", l_oVisibleSize.height * 0.05);
	l_pLabelBackgroundSlider->setPosition(l_oVisibleSize.width * 0.5,
			l_oVisibleSize.height * 0.75);
	l_pLabelBackgroundSlider->setColor(Color3B::WHITE);
	m_pBackGroundSprite->addChild(l_pLabelBackgroundSlider);

	//slider to control background volume
	auto l_pVolumeBackgroundSlider = cocos2d::extension::ControlSlider::create(
			"Ludomuse/Content/sliderred.png",
			"Ludomuse/Content/slidergreen.png",
			"Ludomuse/Content/circlegreen.png");
	l_pVolumeBackgroundSlider->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.7));
	l_pVolumeBackgroundSlider->setMinimumValue(0);
	l_pVolumeBackgroundSlider->setMaximumValue(1);
	l_pVolumeBackgroundSlider->setValue(1.0);
	l_pVolumeBackgroundSlider->addTargetWithActionForControlEvents(this,
			cccontrol_selector(
					LmSettings::valueChangedCallbackBackgroundVolume),
			cocos2d::extension::Control::EventType::VALUE_CHANGED);
	m_pBackGroundSprite->addChild(l_pVolumeBackgroundSlider);

	//label associate to slider
	auto l_pLabelEffectSlider = Label::createWithTTF("Volume Effets",
			"Fonts/JosefinSans-Bold.ttf", l_oVisibleSize.height * 0.05);
	l_pLabelEffectSlider->setPosition(l_oVisibleSize.width * 0.5,
			l_oVisibleSize.height * 0.6);
	l_pLabelEffectSlider->setColor(Color3B::WHITE);
	m_pBackGroundSprite->addChild(l_pLabelEffectSlider);

	//slider to control effect volume
	auto l_pVolumeEffectSlider = cocos2d::extension::ControlSlider::create(
			"Ludomuse/Content/sliderred.png",
			"Ludomuse/Content/slidergreen.png",
			"Ludomuse/Content/circlegreen.png");
	l_pVolumeEffectSlider->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.55));
	l_pVolumeEffectSlider->setMinimumValue(0);
	l_pVolumeEffectSlider->setMaximumValue(1);
	l_pVolumeEffectSlider->setValue(1.0);
	l_pVolumeEffectSlider->addTargetWithActionForControlEvents(this,
			cccontrol_selector(LmSettings::valueChangedCallbackEffectVolume),
			cocos2d::extension::Control::EventType::VALUE_CHANGED);
	m_pBackGroundSprite->addChild(l_pVolumeEffectSlider);

	//stats button child
	auto l_pMenuItemImageStatsChild = MenuItemImage::create(
			"Ludomuse/Content/rejouer.png", "Ludomuse/Content/rejouerpress.png",
			CC_CALLBACK_1(LmSettings::displayStatsChild, this));
	l_pMenuItemImageStatsChild->setPosition(
			Vec2(l_oVisibleSize.width * 0.33, l_oVisibleSize.height * 0.4));
	l_pMenu->addChild(l_pMenuItemImageStatsChild);

	//stats button parent
	auto l_pMenuItemImageStatsParent = MenuItemImage::create(
			"Ludomuse/Content/fin.png", "Ludomuse/Content/finpress.png",
			CC_CALLBACK_1(LmSettings::displayStatsParent, this));
	l_pMenuItemImageStatsParent->setPosition(
			Vec2(l_oVisibleSize.width * 0.66, l_oVisibleSize.height * 0.4));
	l_pMenu->addChild(l_pMenuItemImageStatsParent);

	m_pLmStatsView->initStats();

	//credits button
	auto l_pMenuItemImageCredits = MenuItemImage::create(
			"Ludomuse/Content/credits.png", "Ludomuse/Content/creditspress.png",
			CC_CALLBACK_1(LmSettings::runCredits, this));
	l_pMenuItemImageCredits->setPosition(
			Vec2(l_oVisibleSize.width * 0.33, l_oVisibleSize.height * 0.2));
	l_pMenu->addChild(l_pMenuItemImageCredits);

	m_pLmCredits->initCredits();

	//end button
	auto l_pMenuItemImageEndApplication = MenuItemImage::create(
			"Ludomuse/Content/quitterludomuse.png",
			"Ludomuse/Content/quitterludomusepress.png",
			CC_CALLBACK_1(LmSettings::endApplicationCallbackButton, this));
	l_pMenuItemImageEndApplication->setPosition(
			Vec2(l_oVisibleSize.width * 0.66, l_oVisibleSize.height * 0.2));
	l_pMenu->addChild(l_pMenuItemImageEndApplication);

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

void LmSettings::displayStatsChild(Ref* p_Sender)
{
	//play button clicked sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	FILENAME_BUTTON_CLICKED);

	//display stats

	m_pLmStatsView->runStats(false);
}

void LmSettings::displayStatsParent(Ref* p_Sender)
{
	//play button clicked sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	FILENAME_BUTTON_CLICKED);

	//display stats
	m_pLmStatsView->runStats(true);
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
	//clear and close
	WIFIFACADE->clean();

	//merge stats of this session to json file
	m_pUser->getPLmStatistics()->writeToJson();

	//test
	m_pUser->getPLmStatistics()->printUserFileToLogcat();

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

void LmSettings::runCredits(Ref* p_Sender)
{

	//play button clicked sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	FILENAME_BUTTON_CLICKED);

	m_pLmCredits->run();
}

LmCredits::LmCredits()
{
	//prmitive type
	m_bLayerTouched = false;

	m_pSpeed = 60;

	//pointer
	m_pListener = nullptr;

	m_pLabel = nullptr;
}

LmCredits::~LmCredits()
{

}

void LmCredits::initCredits()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();

	//Creates a fullscreen black layer
	auto l_pLayerBackground = LayerColor::create();
	addChild(l_pLayerBackground);

	//init the background
	auto l_pSpriteBackground = Sprite::create(
			"Ludomuse/Background/creditsbg.png");
	l_pSpriteBackground->setPosition(l_oVisibleSize.width * 0.5f,
			l_oVisibleSize.height * 0.5f);
	l_pLayerBackground->addChild(l_pSpriteBackground);

	//init text
	initText();

	//init label
	m_pLabel = Label::createWithTTF(m_sCreditsText,
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.05);
	m_pLabel->setAnchorPoint(Vec2(0.5, 1));
	m_pLabel->setPosition(Vec2(l_oVisibleSize.width * 0.5, 0));
	m_pLabel->setColor(Color3B::BLACK);
	m_pLabel->setAlignment(TextHAlignment::CENTER);
	m_pLabel->setMaxLineWidth(l_oVisibleSize.width * 0.8);
	l_pLayerBackground->addChild(m_pLabel);

	//menu
	auto l_pMenu = Menu::create();
	l_pMenu->setPosition(Vec2::ZERO);
	l_pSpriteBackground->addChild(l_pMenu);

	//stats button parent
	auto l_pBackButton = MenuItemImage::create("Ludomuse/Content/fin.png",
			"Ludomuse/Content/finpress.png",
			CC_CALLBACK_1(LmCredits::back, this));
	l_pBackButton->setPosition(
			Vec2(l_oVisibleSize.width * 0.9, l_oVisibleSize.height * 0.9));
	l_pMenu->addChild(l_pBackButton);

}

void LmCredits::initText()
{
	//ludomuse object
	auto l_pLmJsonParser = new LmJsonParser;

	if (!l_pLmJsonParser->initJsonDocument("Json/strings.json"))
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

	float l_fDurationAction = m_pLabel->getContentSize().height / m_pSpeed;

	auto l_pMoveDown = MoveBy::create(l_fDurationAction,
			Vect(0, m_pLabel->getContentSize().height));

	m_pLabel->setPosition(Vec2(l_oVisibleSize.width * 0.5, 0));
	m_pLabel->runAction(l_pMoveDown);

}

void LmCredits::back(Ref* p_Sender)
{

	Director::getInstance()->popScene();

}

LmStatsView::LmStatsView()
{
	//pointer
	m_pLabel = nullptr;
	m_pScrollView = nullptr;

}

LmStatsView::~LmStatsView()
{

}

void LmStatsView::initStats()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();

	//Creates a fullscreen black layer
	auto l_pLayerBackground = LayerColor::create();
	addChild(l_pLayerBackground);

	//init the background
	auto l_pSpriteBackground = Sprite::create("Ludomuse/Background/menu.png");
	l_pSpriteBackground->setPosition(l_oVisibleSize.width * 0.5f,
			l_oVisibleSize.height * 0.5f);
	l_pLayerBackground->addChild(l_pSpriteBackground);

	//menu
	auto l_pMenu = Menu::create();
	l_pMenu->setPosition(Vec2::ZERO);
	l_pSpriteBackground->addChild(l_pMenu);

	//stats button parent
	auto l_pBackButton = MenuItemImage::create("Ludomuse/Content/fin.png",
			"Ludomuse/Content/finpress.png",
			CC_CALLBACK_1(LmStatsView::back, this));
	l_pBackButton->setPosition(
			Vec2(l_oVisibleSize.width * 0.9, l_oVisibleSize.height * 0.9));
	l_pMenu->addChild(l_pBackButton);

	//init the scroll view
	m_pScrollView = ui::ScrollView::create();
	Size l_oScrollFrameSize = Size(l_oVisibleSize.width * 0.7,
			l_oVisibleSize.height);
	m_pScrollView->setContentSize(l_oScrollFrameSize);
	m_pScrollView->setAnchorPoint(Vec2(0, 1));
	m_pScrollView->setPosition(Vec2(l_oVisibleSize.width*0.1,l_oVisibleSize.height*0.95 ));
	m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);

	//for now the container is the frame size
	m_pScrollView->setInnerContainerSize(l_oScrollFrameSize);

	//add a layer to it
	auto l_pLayerScrollView = Layer::create();
	m_pScrollView->addChild(l_pLayerScrollView);

	m_pScrollView->setBounceEnabled(true);
	m_pScrollView->setTouchEnabled(true);
	m_pScrollView->setSwallowTouches(false);
	l_pLayerBackground->addChild(m_pScrollView);

	//init label
	m_pLabel = Label::createWithTTF("", "Fonts/JosefinSans-Regular.ttf",
			l_oVisibleSize.width * 0.02);
	m_pLabel->setAnchorPoint(Vec2(0,1));
	m_pLabel->setPosition(
			Vec2(0, m_pScrollView->getInnerContainerSize().height));
	m_pLabel->setColor(Color3B::WHITE);
	m_pLabel->setAlignment(TextHAlignment::LEFT);
	m_pLabel->setMaxLineWidth(l_oVisibleSize.width * 0.7);
	l_pLayerScrollView->addChild(m_pLabel);

}

void LmStatsView::back(Ref* p_Sender)
{
	Director::getInstance()->popScene();
}

void LmStatsView::runStats(bool l_bIsParent)
{
	std::string l_sFullPath;

	if (l_bIsParent)
	{
		l_sFullPath =
				FileUtils::getInstance()->getWritablePath() + FILENAME_USER_PARENT;

	}
	else
	{
		l_sFullPath =
				FileUtils::getInstance()->getWritablePath() + FILENAME_USER_CHILD;

	}

	if (FileUtils::getInstance()->isFileExist(l_sFullPath))
	{
		m_sTextJson = FileUtils::getInstance()->getStringFromFile(l_sFullPath);
		m_pLabel->setString(parseTextJson());

	}
	else
	{
		m_pLabel->setString("Il n'y pas encore de statistique disponible");
	}

	//updateScrollViewContainer
	Size l_oContainerSize = m_pScrollView->getInnerContainerSize();
	if (m_pLabel->getContentSize().height > l_oContainerSize.height)
	{

		l_oContainerSize = Size(l_oContainerSize.width,
				m_pLabel->getContentSize().height);
		m_pScrollView->setInnerContainerSize(l_oContainerSize);
		m_pLabel->setPosition(
					Vec2(0, m_pScrollView->getInnerContainerSize().height));
	}

	Director::getInstance()->pushScene(TransitionFade::create(0.5, this));

}

std::string LmStatsView::parseTextJson()
{
	std::string l_sResult;
	std::string l_sBuffer;
	std::string l_sEqualTo = ": ";
	std::string l_sNextLine = "\n";
	std::string l_sSeparator = "--------------------------------------------\n";

	char l_cBufferChar[20];

	CCLOG("pareser");
	rapidjson::Document l_oDocument;
	l_oDocument.Parse<0>(m_sTextJson.c_str());

	//nombre d'utilisateur
	assert(l_oDocument.HasMember(USER_NUMBER_KEY));
	assert(l_oDocument[USER_NUMBER_KEY].IsInt());
	sprintf(l_cBufferChar, "%d", l_oDocument[USER_NUMBER_KEY].GetInt());
	l_sBuffer = l_cBufferChar;
	l_sResult += USER_NUMBER_KEY + l_sEqualTo + l_sBuffer + l_sNextLine;
	l_sResult += l_sNextLine;
	l_sResult += l_sNextLine;


	//date
	assert(l_oDocument.HasMember(ARRAY_DATE_KEY));
	assert(l_oDocument[ARRAY_DATE_KEY].IsArray());
	l_sResult += ARRAY_DATE_KEY + l_sEqualTo + l_sNextLine;
	for (int i = 0; i < l_oDocument[ARRAY_DATE_KEY].Size(); i++)
	{
		assert(l_oDocument[ARRAY_DATE_KEY][i].IsString());
		l_sResult += l_oDocument[ARRAY_DATE_KEY][i].GetString() + l_sNextLine;
	}
	l_sResult += l_sNextLine;
	l_sResult += l_sNextLine;

	//array game
	assert(l_oDocument.HasMember(ARRAY_GAME_KEY));
	assert(l_oDocument[ARRAY_GAME_KEY].IsArray());
	l_sResult += ARRAY_GAME_KEY + l_sEqualTo + l_sNextLine;
	l_sResult += l_sNextLine;
	l_sResult += l_sSeparator;
	for (int i = 0; i < l_oDocument[ARRAY_GAME_KEY].Size(); i++)
	{
		//id game
		assert(l_oDocument[ARRAY_GAME_KEY][i].HasMember(ID_GAME));
		assert(l_oDocument[ARRAY_GAME_KEY][i][ID_GAME].IsInt());
		sprintf(l_cBufferChar, "%d",
				l_oDocument[ARRAY_GAME_KEY][i][ID_GAME].GetInt());
		l_sBuffer = l_cBufferChar;
		l_sResult += ID_GAME + l_sEqualTo + l_sBuffer + l_sNextLine;

		//title
		assert(l_oDocument[ARRAY_GAME_KEY][i].HasMember(TITLE_GAME));
		assert(l_oDocument[ARRAY_GAME_KEY][i][TITLE_GAME].IsString());
		l_sResult += TITLE_GAME + l_sEqualTo
				+ l_oDocument[ARRAY_GAME_KEY][i][TITLE_GAME].GetString()
				+ l_sNextLine;

		//scene map
		assert(l_oDocument[ARRAY_GAME_KEY][i].HasMember(SCENE_MAP));
		assert(l_oDocument[ARRAY_GAME_KEY][i][SCENE_MAP].IsArray());
		if (l_oDocument[ARRAY_GAME_KEY][i][SCENE_MAP].Size())
		{
			l_sResult += SCENE_MAP + l_sEqualTo + l_sNextLine;
			const rapidjson::Value& l_oSCENE_MAP =
					l_oDocument[ARRAY_GAME_KEY][i][SCENE_MAP];
			l_sResult += l_sNextLine;
			for (int j = 0; j < l_oSCENE_MAP.Size(); j++)
			{
				//id scene
				assert(l_oSCENE_MAP[j].HasMember(ID_SCENE));
				assert(l_oSCENE_MAP[j][ID_SCENE].IsInt());
				sprintf(l_cBufferChar, "%d",
						l_oSCENE_MAP[j][ID_SCENE].GetInt());
				l_sBuffer = l_cBufferChar;
				l_sResult += ID_SCENE + l_sEqualTo + l_sBuffer + l_sNextLine;

				//description
				assert(l_oSCENE_MAP[j].HasMember(DESCRIPTION_SCENE));
				assert(l_oSCENE_MAP[j][DESCRIPTION_SCENE].IsString());
				l_sResult += DESCRIPTION_SCENE + l_sEqualTo
						+ l_oSCENE_MAP[j][DESCRIPTION_SCENE].GetString()
						+ l_sNextLine;

				//time game
				//id scene
				assert(l_oSCENE_MAP[j].HasMember(TIME_GAME));
				assert(l_oSCENE_MAP[j][TIME_GAME].IsInt());
				sprintf(l_cBufferChar, "%d",
						l_oSCENE_MAP[j][TIME_GAME].GetInt());
				l_sBuffer = l_cBufferChar;
				l_sResult += TIME_GAME + l_sEqualTo + l_sBuffer + l_sNextLine;

				//time interaction
				//id scene
				assert(l_oSCENE_MAP[j].HasMember(TIME_INTERACTION));
				assert(l_oSCENE_MAP[j][TIME_INTERACTION].IsInt());
				sprintf(l_cBufferChar, "%d",
						l_oSCENE_MAP[j][TIME_INTERACTION].GetInt());
				l_sBuffer = l_cBufferChar;
				l_sResult += TIME_INTERACTION + l_sEqualTo + l_sBuffer
						+ l_sNextLine;
				l_sResult += l_sNextLine;

			}
		}
		l_sResult += l_sSeparator;
		l_sResult += l_sNextLine;

		//buttons
		std::vector<std::string> l_aKeyButtonToListen = KEY_BUTTONS;
		for (int i = 0; i < l_aKeyButtonToListen.size(); i++)
		{
			assert(l_oDocument.HasMember(l_aKeyButtonToListen.at(i).c_str()));
			assert(l_oDocument[l_aKeyButtonToListen.at(i).c_str()].IsInt());
			sprintf(l_cBufferChar, "%d",
					l_oDocument[l_aKeyButtonToListen.at(i).c_str()].GetInt());
			l_sBuffer = l_cBufferChar;
			l_sResult += l_aKeyButtonToListen.at(i).c_str() + l_sEqualTo
					+ l_sBuffer + l_sNextLine;
		}
		l_sResult += l_sNextLine;
		l_sResult += l_sNextLine;


	}

	return l_sResult.c_str();
}

