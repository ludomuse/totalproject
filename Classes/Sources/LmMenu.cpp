/*
 * LmMenu.cpp
 *
 *  Created on: 9 sept. 2015
 *      Author: IHMTEKDev4
 */

//use for the debug
#define COCOS2D_DEBUG 1

#include "../Include/LmMenu.h"

USING_NS_CC;
using namespace cocos2d::ui;

LmMenu::LmMenu(WifiDirectFacade* a_wifiFacade)
{

	//register to direct wifi
	m_pWifiDirectFacade = a_wifiFacade;
	m_pWifiDirectFacade->addObserver(this);

	//object
	m_pUser1 = new LmUser; //delete in LmGameManager
	m_pUser2 = new LmUser; //delete in LmGameManager

	//pointer
	m_pLmMenuScene = nullptr;
	m_pLogEditBox = nullptr;
	m_pLogLayer = nullptr;
	m_pSpriteLogBackground = nullptr;
	m_pSpriteSplashScreen = nullptr;
	m_pSpriteWifiBackground = nullptr;
	m_pWifiLayer = nullptr;
	m_pMenuUserTabletName = nullptr;
	m_pMenu = nullptr;
	m_pCheckBoxFemale = nullptr;
	m_pCheckBoxMale = nullptr;

	//primitive type
	m_bPlayButtonClicked = false; //the play has not been clicked yet
	m_bNoGenderSelected = true;
}

LmMenu::~LmMenu()
{
	//we don't delete user here because gamemanger get pointer on them and it's him which is gonna to delete them

	m_pLogEditBox->release();
}

void LmMenu::splashScreen()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//init scene
	m_pLmMenuScene = Scene::create();

	//call director to run the scene
	Director::getInstance()->runWithScene(m_pLmMenuScene);

	//init log layer
	m_pLogLayer = Layer::create();

	//firstable we add the log layer
	m_pLmMenuScene->addChild(m_pLogLayer);

	//init splash screen and display it for a while
	m_pSpriteSplashScreen = Sprite::create(s_sFilenameSplash);
	m_pSpriteSplashScreen->setPosition(l_oVisibleSize.width / 2 + l_oOrigin.x,
			l_oVisibleSize.height / 2 + l_oOrigin.y);
	m_pLogLayer->addChild(m_pSpriteSplashScreen);

	//wait a while
	auto delay = DelayTime::create(s_fSplashScreenDuration);
	m_pLogLayer->runAction(
			Sequence::create(delay,
					CallFunc::create(std::bind(&LmMenu::logScreen, this)),
					nullptr));

}

bool LmMenu::logScreen()
{

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//remove the splash screen
	m_pLogLayer->removeChild(m_pSpriteSplashScreen);

	//add a background img for the log layer
	m_pSpriteLogBackground = Sprite::create(s_sFilenameSpriteBackground);
	m_pSpriteLogBackground->setPosition(l_oVisibleSize.width / 2 + l_oOrigin.x,
			l_oVisibleSize.height / 2 + l_oOrigin.y);
	m_pLogLayer->addChild(m_pSpriteLogBackground);

	//log button
	auto l_oLogButton = MenuItemImage::create(
			"Ludomuse/GUIElements/logNormal.png",
			"Ludomuse/GUIElements/logPressed.png",
			CC_CALLBACK_1(LmMenu::wifiDirectScreen, this));
	l_oLogButton->setAnchorPoint(Point(0.5, 0));
	l_oLogButton->setPosition(
			Vect(l_oVisibleSize.width * 0.5f, l_oVisibleSize.height * 0.2f));

	// Create the textfield
	m_pLogEditBox = EditBox::create(
			Size(l_oVisibleSize.width * 0.6, l_oVisibleSize.height * 0.2),
			Scale9Sprite::create(
					"Ludomuse/GUIElements/textfieldBackground.png"));
	m_pLogEditBox->setPosition(
			Point(l_oVisibleSize.width * 0.5f, l_oVisibleSize.height * 0.8f));
	m_pLogEditBox->setPlaceHolder("Name");
	m_pLogEditBox->setFontSize(l_oVisibleSize.width * 0.04);
	m_pLogEditBox->setFontName("Fonts/JosefinSans-Regular.ttf");
	m_pLogEditBox->setFontColor(Color3B::BLACK);
	m_pLogEditBox->setMaxLength(s_iMaxLenghtUserName);
	m_pLogEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pLogEditBox->retain();

	//test
	m_pLogEditBox->setText("P");

	m_pLogLayer->addChild(m_pLogEditBox, 1);

	// create menu, it's an autorelease object
	auto l_oMenu = Menu::create(l_oLogButton, nullptr);
	l_oMenu->setPosition(Vec2::ZERO);
	m_pLogLayer->addChild(l_oMenu, 1);

	//init checkbox male
	m_pCheckBoxMale = CheckBox::create("Ludomuse/GUIElements/logNormal.png",
			"Ludomuse/GUIElements/logPressed.png");
	m_pCheckBoxMale->setTouchEnabled(true);
	m_pCheckBoxMale->setSwallowTouches(false);
	m_pCheckBoxMale->setPosition(
			Vec2(l_oVisibleSize.width * 0.33, l_oVisibleSize.height * 0.5));
	m_pCheckBoxMale->addEventListener(
			CC_CALLBACK_2(LmMenu::maleSelected, this));
	m_pLogLayer->addChild(m_pCheckBoxMale);

	//init checkbox female
	m_pCheckBoxFemale = CheckBox::create("Ludomuse/GUIElements/playNormal.png",
			"Ludomuse/GUIElements/playPressed.png");
	m_pCheckBoxFemale->setTouchEnabled(true);
	m_pCheckBoxFemale->setSwallowTouches(false);
	m_pCheckBoxFemale->setPosition(
			Vec2(l_oVisibleSize.width * 0.66, l_oVisibleSize.height * 0.5));
	m_pCheckBoxFemale->addEventListener(
			CC_CALLBACK_2(LmMenu::femaleSelected, this));
	m_pLogLayer->addChild(m_pCheckBoxFemale);

	return true;
}

bool LmMenu::wifiDirectScreen(cocos2d::Ref* l_oSender)
{
	//check if the user enter smthg
	if (!strcmp(m_pLogEditBox->getText(), "") || m_bNoGenderSelected)
	{
		return false;
	}
	else
	{

		//use to place elements
		Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
		Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

		//remove the log layer
		m_pLmMenuScene->removeChild(m_pLogLayer);

		//create wifi layer
		m_pWifiLayer = Layer::create();

		//add the wifilayer
		m_pLmMenuScene->addChild(m_pWifiLayer);

		//add a background img for the wifi layer
		m_pSpriteWifiBackground = Sprite::create(s_sFilenameSpriteBackground);
		m_pSpriteWifiBackground->setPosition(l_oVisibleSize.width / 2,
				l_oVisibleSize.height / 2 + l_oOrigin.y);
		m_pWifiLayer->addChild(m_pSpriteWifiBackground);

		//Play button
		auto l_oPlayButton = MenuItemImage::create(
				"Ludomuse/GUIElements/playNormal.png",
				"Ludomuse/GUIElements/playPressed.png",
				CC_CALLBACK_1(LmMenu::menuIsFinished, this));
		l_oPlayButton->setAnchorPoint(Point(0.5, 0));
		l_oPlayButton->setPosition(
				Vect(l_oVisibleSize.width * 0.5f,
						l_oVisibleSize.height * 0.2f));

		//scan button
		auto l_oScanButton = MenuItemImage::create(
				"Ludomuse/GUIElements/logNormal.png",
				"Ludomuse/GUIElements/logPressed.png",
				CC_CALLBACK_1(LmMenu::scan, this));
		l_oScanButton->setAnchorPoint(Point(0.5, 0));
		l_oScanButton->setPosition(
				Vect(l_oVisibleSize.width * 0.5f,
						l_oVisibleSize.height * 0.7f));

		// create menu, it's an autorelease object
		m_pMenu = Menu::create(l_oPlayButton, l_oScanButton, nullptr);
		m_pMenu->setPosition(Vec2::ZERO);
		m_pWifiLayer->addChild(m_pMenu, 1);

		//create menu for the list of user
		m_pMenuUserTabletName = Menu::create();
		m_pMenuUserTabletName->setPosition(Vec2::ZERO);
		m_pWifiLayer->addChild(m_pMenuUserTabletName, 1);

		//TODO find peers and set attributes and instanciate socket

		//tablet are connected create both user
		m_pUser1->setPScore(0);
		m_pUser1->setPUserName(m_pLogEditBox->getText());
		m_pUser1->setPUserTabletName("tablet1_name");

		//to know what part of the game he is going to play

		//test if P alors we play parent role
		if (strcmp(m_pLogEditBox->getText(), "P"))
		{
			m_pUser1->setBParent(true);
			m_pUser2->setBParent(false);
		}
		else
		{
			m_pUser1->setBParent(false);
			m_pUser2->setBParent(true);
		}

		m_pUser2->setPScore(0);
		m_pUser2->setPUserName("2nd user");
		m_pUser2->setPUserTabletName("tablet2_name");

		//m_pWifiDirectFacade->discoverPeers();

		std::vector<std::string> vectorString;
		vectorString.push_back("fbdjh");
		vectorString.push_back("sdfsdf");
		vectorString.push_back("sdfsdggfd");
		vectorString.push_back("encore un autre");
		makeMenuItemUserTabletName(vectorString);

		return true;

	}
}

bool LmMenu::menuIsFinished(cocos2d::Ref* l_oSender)
{
	if (!m_bPlayButtonClicked)
	{
		m_bPlayButtonClicked = true;
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
				"MenuFinished");
	}

}

void LmMenu::maleSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType)
{
	//update state of checkbox
	m_pCheckBoxMale->setSelected(true);
	m_pCheckBoxFemale->setSelected(false);
	//update user gender
	m_pUser1->setBMale(true);
	//permit to go on the wifi direct screen
	m_bNoGenderSelected = false;
}

void LmMenu::femaleSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType)
{
	//update state of checkbox
	m_pCheckBoxFemale->setSelected(true);
	m_pCheckBoxMale->setSelected(false);
	m_pUser1->setBMale(false);
	m_bNoGenderSelected = false;
}

void LmMenu::scan(cocos2d::Ref* l_pSender)
{
	CCLOG("scan");
	m_pWifiDirectFacade->discoverPeers();

}

void LmMenu::makeMenuItemUserTabletName(
		std::vector<std::string> l_aVectorOfTabletName)
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//reset vector and init with the new vector of string
	for (std::map<cocos2d::MenuItemImage*, cocos2d::Label*>::iterator it =
			m_aMenuItemUserTabletName.begin();
			it != m_aMenuItemUserTabletName.end(); ++it)
	{
		m_pMenuUserTabletName->removeAllChildrenWithCleanup(true);
	}
	m_aMenuItemUserTabletName.clear();

	int l_iIndex = 0;
	int l_iSize = l_aVectorOfTabletName.size();
	for (std::vector<std::string>::iterator it = l_aVectorOfTabletName.begin();
			it != l_aVectorOfTabletName.end(); ++it)
	{

		//init label and menuitem associated
		auto l_pLabel = Label::createWithTTF((*it),
				"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
		l_pLabel->setColor(Color3B::BLACK);
		auto l_pMenuItemImage = MenuItemImage::create(
				"Ludomuse/GUIElements/logNormal.png",
				"Ludomuse/GUIElements/logPressed.png",
				CC_CALLBACK_1(LmMenu::updateUser2NameTablet, this));
		l_pMenuItemImage->addChild(l_pLabel);

		m_aMenuItemUserTabletName.insert(
		{ l_pMenuItemImage, l_pLabel });

		l_pLabel->setAnchorPoint(Vec2(0.5, 0.5));
		l_pLabel->setPosition(
				Vec2(l_pMenuItemImage->getContentSize().width * 0.5,
						l_pMenuItemImage->getContentSize().height * 0.5));
		l_pLabel->setMaxLineWidth(l_pMenuItemImage->getContentSize().width);
		l_pMenuItemImage->setAnchorPoint(Vec2(0, 0.5));
		l_pMenuItemImage->setPosition(
				Vec2(
						s_fMarginLeftMenu
								+ (l_oVisibleSize.width * l_iIndex) / l_iSize,
						l_oVisibleSize.height * 0.5));
		m_pMenuUserTabletName->addChild(l_pMenuItemImage);
		l_iIndex++;
	}

}

void LmMenu::onGettingPeers(std::vector<std::string> peers)
{
	CCLOG("I received a list of peers. List is: ");
	for (std::vector<std::string>::const_iterator i = peers.begin();
			i != peers.end(); ++i)
	{
		CCLOG("%s", (*i).c_str());
	}

	//wait a while to be sure
	auto delay =
			DelayTime::create(0);
	m_pWifiLayer->runAction(
			Sequence::create(delay,
					CallFunc::create(std::bind(&LmMenu::makeMenuItemUserTabletName, this,peers)),
					nullptr));

}

void LmMenu::updateUser2NameTablet(cocos2d::Ref* p_Sender)
{
	auto l_pMenuItemPressed = dynamic_cast<MenuItemImage*>(p_Sender);
	auto l_pLabel = m_aMenuItemUserTabletName.find(l_pMenuItemPressed)->second;
	//set user 2 tablet name
	std::string l_sBufferString = l_pLabel->getString().c_str();
	m_pUser2->setPUserTabletName(l_sBufferString);

	CCLOG("tablet name user 2 = %s", m_pUser2->getPUserTabletName().c_str());
	m_pWifiDirectFacade->connectTo(m_pUser2->getPUserTabletName());
}
