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

LmMenu::LmMenu()
{

	//register to direct wifi
	listenWifiFacade();

	//ludomuse object
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
	m_pMenu = nullptr;
	m_pMenuItemImageFemale = nullptr;
	m_pMenuItemImageMale = nullptr;
	m_pSpriteReadyIndicator = nullptr;
	m_pCheckBoxChild = nullptr;
	m_pCheckBoxParent = nullptr;
	m_pLabelFeedback = nullptr;
	m_pReadyButton = nullptr;
	m_pRefreshButton = nullptr;

	//primitive type
	m_bRoleSelected = false;
	m_bConnected = false;
	m_bReady = false;
	m_bMenuIsFinished = false;
	m_iIdWifiObserver = 0;
	m_bFirstGettingPeers = true;

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

void LmMenu::onGettingPeers(std::vector<std::string> peers)
{

	if (m_bFirstGettingPeers)
	{
		m_bFirstGettingPeers = false;
		ON_CC_THREAD(LmMenu::setLabelFeedBack, this);
	}

	//unclivk ready button
	if(m_bReady)
	{
		ready(nullptr);
	}


	ON_CC_THREAD(LmMenu::makeCheckboxUserTabletName, this, peers);

}

void LmMenu::onReceivingMsg(bytes l_oMsg)
{
	CCLOG("lm menu_event is %d", _event);
	switch (_event)
	{
	case LmEvent::UserIsReady:
		CCLOG("onUserIsReadyEvent");
		ON_CC_THREAD(LmMenu::onUserIsReadyEvent, this, l_oMsg)
		;
		break;
	case LmEvent::CompatibleToPlay:
		CCLOG("onCompatibleToPlayEvent");
		ON_CC_THREAD(LmMenu::onCompatibleToPlayEvent, this, l_oMsg)
		;
		break;
	case LmEvent::Play:
		CCLOG("onPlayEvent");
		ON_CC_THREAD(LmMenu::onPlayEvent, this, l_oMsg)
		;
		break;
	default:
		break;
	}

}

bool LmMenu::logScreen()
{

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//remove the splash screen
	m_pLogLayer->removeChild(m_pSpriteSplashScreen);

	//preload sounds
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(
			FILENAME_BUTTON_CLICKED);

	//add a background img for the log layer
	m_pSpriteLogBackground = Sprite::create(s_sFilenameSpriteBackground);
	m_pSpriteLogBackground->setPosition(l_oVisibleSize.width *0.5,
			l_oVisibleSize.height*0.5);
	m_pLogLayer->addChild(m_pSpriteLogBackground);

	//add the sprite of the form
	auto l_pFormSprite = Sprite::create("Ludomuse/GUIElements/form.png");
	l_pFormSprite->setPosition(l_oVisibleSize.width / 2 + l_oOrigin.x,
			l_oVisibleSize.height / 2 + l_oOrigin.y);
	m_pLogLayer->addChild(l_pFormSprite);

	//log button
	auto l_oLogButton = MenuItemImage::create(
			"Ludomuse/GUIElements/logNormal.png",
			"Ludomuse/GUIElements/logPressed.png",
			CC_CALLBACK_1(LmMenu::logFinished, this));
	l_oLogButton->setAnchorPoint(Point(0.5, 0.5));
	l_oLogButton->setPosition(
			Vect(
					(l_oVisibleSize.width
							+ l_pFormSprite->getContentSize().width * 0.5)
							* 0.5, l_oVisibleSize.height * 0.5f));

	// create menu, it's an autorelease object
	auto l_oMenu = Menu::create(l_oLogButton, nullptr);
	l_oMenu->setPosition(Vec2::ZERO);
	m_pLogLayer->addChild(l_oMenu, 1);

	// Create the textfield
	m_pLogEditBox = EditBox::create(
			Size(l_pFormSprite->getContentSize().width * 0.4,
					l_oVisibleSize.height * 0.1),
			Scale9Sprite::create(
					"Ludomuse/GUIElements/textfieldBackground.png"));
	m_pLogEditBox->setPosition(
			Vect(
					(l_oVisibleSize.width
							- l_pFormSprite->getContentSize().width * 0.5)
							* 0.5, l_oVisibleSize.height * 0.5f));
	m_pLogEditBox->setPlaceHolder("Pseudo");
	m_pLogEditBox->setFontSize(l_oVisibleSize.width * 0.03);
	m_pLogEditBox->setFontName("Fonts/JosefinSans-Regular.ttf");
	m_pLogEditBox->setFontColor(Color3B::BLACK);
	m_pLogEditBox->setMaxLength(s_iMaxLenghtUserName);
	m_pLogEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pLogEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pLogEditBox->retain();
	m_pLogLayer->addChild(m_pLogEditBox, 1);

	//test
	//m_pLogEditBox->setText("UserName");

	return true;
}

void LmMenu::logFinished(Ref* p_Sender)
{
	//check if the user enter smthg
	if (strcmp(m_pLogEditBox->getText(), "") != 0)
	{

		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
					FILENAME_BUTTON_CLICKED);

		Size l_oVisibleSize = Director::getInstance()->getVisibleSize();

		//remove the log layer
		m_pLmMenuScene->removeChild(m_pLogLayer);

		//create wifi layer
		m_pWifiLayer = Layer::create();

		//add the wifilayer
		m_pLmMenuScene->addChild(m_pWifiLayer);

		// create menu
		m_pMenu = Menu::create();
		m_pMenu->setPosition(Vec2::ZERO);
		m_pWifiLayer->addChild(m_pMenu, 1);

		//add a background img for the wifi layer
		m_pSpriteWifiBackground = Sprite::create(s_sFilenameSpriteBackground);
		m_pSpriteWifiBackground->setPosition(l_oVisibleSize.width * 0.5,
				l_oVisibleSize.height * 0.5);
		m_pWifiLayer->addChild(m_pSpriteWifiBackground);

		//init checkbox male
		m_pMenuItemImageMale = MenuItemImage::create(
				"Ludomuse/GUIElements/checkoutmale2.png",
				"Ludomuse/GUIElements/checkoutmalepress2.png",
				CC_CALLBACK_1(LmMenu::maleSelected, this));
		m_pMenuItemImageMale->setPosition(
				Vec2(l_oVisibleSize.width * 0.25, l_oVisibleSize.height * 0.5));
		m_pMenuItemImageMale->setScaleX(7);
		m_pMenuItemImageMale->setScaleY(7);
		m_pMenu->addChild(m_pMenuItemImageMale);

		//init checkbox female
		m_pMenuItemImageFemale = MenuItemImage::create(
				"Ludomuse/GUIElements/checkoutfemale2.png",
				"Ludomuse/GUIElements/checkoutfemalepress2.png",
				CC_CALLBACK_1(LmMenu::femaleSelected, this));
		m_pMenuItemImageFemale->setPosition(
				Vec2(l_oVisibleSize.width * 0.75, l_oVisibleSize.height * 0.5));
		m_pMenuItemImageFemale->setScaleX(7);
		m_pMenuItemImageFemale->setScaleY(7);
		m_pMenu->addChild(m_pMenuItemImageFemale);
	}
}

bool LmMenu::wifiDirectScreen()
{
	ON_CC_THREAD(LmMenu::removeMenuElement, this);

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//m_oWindowListOfDevice = Rect()
	auto l_pSpriteCheckBoxDevices = Sprite::create(
			"Ludomuse/GUIElements/backgroundDevice.png");
	Size l_oButtonDevicesSize = l_pSpriteCheckBoxDevices->getContentSize();
	m_oWindowListOfDevice = Rect(s_fMarginLeftMenu, l_oVisibleSize.height * 0.5,
			l_oVisibleSize.width - (s_fMarginLeftMenu * 2),
			l_oButtonDevicesSize.height * 3);

	//feedback label init
	m_pLabelFeedback = Label::createWithTTF("", "Fonts/JosefinSans-Bold.ttf",
			l_oVisibleSize.height * 0.05);
	m_pLabelFeedback->setAnchorPoint(Vec2(0.5, 1));
	m_pLabelFeedback->setPosition(l_oVisibleSize.width * 0.5,
			m_oWindowListOfDevice.origin.y);
	m_pLabelFeedback->setColor(Color3B::WHITE);
	m_pLabelFeedback->setMaxLineWidth(l_oVisibleSize.width * 0.8);
	m_pSpriteWifiBackground->addChild(m_pLabelFeedback);

	m_pLabelFeedback->setString(s_sSearching);

	//sprite to indicate if ready or not
	m_pSpriteReadyIndicator = Sprite::create("Ludomuse/GUIElements/cross.png");
	m_pSpriteReadyIndicator->setAnchorPoint(Vec2(1, 0.5));
	m_pSpriteReadyIndicator->setPosition(
			Vec2(l_oVisibleSize.width - s_fMarginLeftMenu,
					l_oVisibleSize.height * 0.25f));
	m_pWifiLayer->addChild(m_pSpriteReadyIndicator);

	//ready button
	m_pReadyButton = MenuItemImage::create("Ludomuse/GUIElements/prete.png",
			"Ludomuse/GUIElements/pretepress.png",
			CC_CALLBACK_1(LmMenu::ready, this));
	m_pReadyButton->setAnchorPoint(Point(1, 0.5));
	m_pReadyButton->setPosition(
			Vec2(
					l_oVisibleSize.width - s_fMarginLeftMenu
							- m_pSpriteReadyIndicator->getContentSize().width,
					l_oVisibleSize.height * 0.25f));
	m_pMenu->addChild(m_pReadyButton);

	//label to know what is the name of our tablet
	std::string l_sTextLabelDeviceName = "Nom tablette : "
			+ getUser1TabletName();
	CCLOG("%s", l_sTextLabelDeviceName.c_str());
	auto l_pLabelDeviceName = Label::createWithTTF(l_sTextLabelDeviceName,
			"Fonts/JosefinSans-Bold.ttf", l_oVisibleSize.height * 0.05);
	l_pLabelDeviceName->setAnchorPoint(Vec2(1, 0));
	l_pLabelDeviceName->setPosition(l_oVisibleSize.width - s_fMarginLeftMenu,
			l_oVisibleSize.height * 0.15);
	l_pLabelDeviceName->setColor(Color3B::WHITE);
	m_pSpriteWifiBackground->addChild(l_pLabelDeviceName);

	//refresh button
	m_pRefreshButton = MenuItemImage::create(
			"Ludomuse/GUIElements/refreshbutton.png",
			"Ludomuse/GUIElements/refreshbuttonpress.png",
			CC_CALLBACK_1(LmMenu::scan, this));
	m_pRefreshButton->setAnchorPoint(Point(1, 0));
	m_pRefreshButton->setPosition(
			Vect(m_oWindowListOfDevice.getMaxX(),
					m_oWindowListOfDevice.origin.y));
	m_pMenu->addChild(m_pRefreshButton);

	//init checkbox parent
	if (m_pUser1->isBMale())
	{
		m_pCheckBoxParent = CheckBox::create(
				"Ludomuse/GUIElements/parentcheckbutton.png",
				"Ludomuse/GUIElements/parentpressbutton.png");

		m_pCheckBoxChild = CheckBox::create(
				"Ludomuse/GUIElements/childcheckbutton.png",
				"Ludomuse/GUIElements/childpressbutton.png");

	}
	else
	{
		m_pCheckBoxParent = CheckBox::create(
				"Ludomuse/GUIElements/momcheckbutton.png",
				"Ludomuse/GUIElements/mompressbutton.png");

		m_pCheckBoxChild = CheckBox::create(
				"Ludomuse/GUIElements/daughtercheckbutton.png",
				"Ludomuse/GUIElements/daughterpressbutton.png");
	}

	m_pCheckBoxParent->setTouchEnabled(true);
	m_pCheckBoxParent->setSwallowTouches(false);
	m_pCheckBoxParent->setAnchorPoint(Vec2(0, 0.5));
	m_pCheckBoxParent->setPosition(
			Vec2(s_fMarginLeftMenu, l_oVisibleSize.height * 0.25));
	m_pCheckBoxParent->setScaleX(2.3);
	m_pCheckBoxParent->setScaleY(2.3);
	m_pCheckBoxParent->addEventListener(
			CC_CALLBACK_2(LmMenu::parentSelected, this));
	m_pWifiLayer->addChild(m_pCheckBoxParent);

	//init checkbox child

	m_pCheckBoxChild->setTouchEnabled(true);
	m_pCheckBoxChild->setSwallowTouches(false);
	m_pCheckBoxChild->setAnchorPoint(Vec2(0, 0.5));
	m_pCheckBoxChild->setPosition(
			Vec2(s_fMarginLeftMenu + m_pCheckBoxParent->getCustomSize().width*2.3,
					l_oVisibleSize.height * 0.25));
	m_pCheckBoxChild->addEventListener(
			CC_CALLBACK_2(LmMenu::childSelected, this));
	m_pCheckBoxChild->setScaleX(2.3);
	m_pCheckBoxChild->setScaleY(2.3);
	m_pWifiLayer->addChild(m_pCheckBoxChild);

	//init user1
	m_pUser1->setPScore(0);
	m_pUser1->setPUserName(m_pLogEditBox->getText());
	m_pUser1->setPUserTabletName(getUser1TabletName());

	setReadyVisible(false);
	//set role button visible
	m_pCheckBoxChild->setVisible(false);
	m_pCheckBoxParent->setVisible(false);

	CCLOG("<font color='blue'>%s</font>", getUser1TabletName().c_str());
	_wifiFacade->discoverPeers();

	return true;

}

void LmMenu::ready(cocos2d::Ref* l_oSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	if (!m_bReady)
	{
		//role are selected and user 2 has a tablet name
		m_bReady = true;

		m_pSpriteReadyIndicator->setTexture("Ludomuse/GUIElements/check.png");

		//send the msg
		bytes msg(50);
		msg << LmEvent::UserIsReady << *m_pUser1;
		WIFIFACADE->sendBytes(msg);

		CCLOG("m_pLabelFeedback->setString(s_sUserIsReady);");
		m_pLabelFeedback->setString(s_sUserIsReady);

	}
	else
	{
		//not ready anymore
		m_bReady = false;

		m_pSpriteReadyIndicator->setTexture("Ludomuse/GUIElements/cross.png");

		CCLOG("m_pLabelFeedback->setString(s_sBegining);");
		m_pLabelFeedback->setString(s_sDeviceAndRoleSlected);

	}

}

void LmMenu::menuIsFinished()
{

	if (!m_bMenuIsFinished)
	{
		CCLOG("menu is finished");

		m_bMenuIsFinished = true;
		stopListenWifiFacade();
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
				"MenuFinished");
	}

}

void LmMenu::maleSelected(cocos2d::Ref*)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	CCLOG("male");
	m_pUser1->setBMale(true);
	wifiDirectScreen();
}

void LmMenu::femaleSelected(cocos2d::Ref*)
{

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	CCLOG("female");
	m_pUser1->setBMale(false);
	wifiDirectScreen();

}

void LmMenu::parentSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType)
{

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	if (m_bReady && !m_pUser1->isBParent())
	{
		//unclick the ready button
		ready(nullptr);
	}

	//update state of checkbox
	m_pCheckBoxParent->setSelected(true);
	m_pCheckBoxChild->setSelected(false);
	//update user role
	m_pUser1->setBParent(true);
	m_bRoleSelected = true;

	setReadyVisible(true);
	m_pLabelFeedback->setString(s_sDeviceAndRoleSlected);

}

void LmMenu::childSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType)
{

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	if (m_bReady && m_pUser1->isBParent())
	{
		//unclick the ready button
		ready(nullptr);
	}

	//update state of checkbox
	m_pCheckBoxChild->setSelected(true);
	m_pCheckBoxParent->setSelected(false);
	m_pUser1->setBParent(false);
	m_bRoleSelected = true;

	setReadyVisible(true);
	m_pLabelFeedback->setString(s_sDeviceAndRoleSlected);

}

void LmMenu::scan(cocos2d::Ref* l_pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
				FILENAME_BUTTON_CLICKED);

	_wifiFacade->discoverPeers();
}

void LmMenu::makeCheckboxUserTabletName(
		std::vector<std::string> l_aVectorOfTabletName)
{
	CCLOG("makeCheckboxUserTabletName");
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//clear
	for (std::map<ui::CheckBox*, Label*>::iterator it =
			m_aMenuItemUserTabletName.begin();
			it != m_aMenuItemUserTabletName.end(); ++it)
	{
		it->first->removeChild(it->second);
		m_pWifiLayer->removeChild(it->first);
	}
	m_aMenuItemUserTabletName.clear();

	//make
	int l_iIndex = 0;
	int l_iLine = 0;
	int l_iSize = l_aVectorOfTabletName.size();
	for (std::vector<std::string>::iterator it = l_aVectorOfTabletName.begin();
			it != l_aVectorOfTabletName.end(); ++it)
	{
		//init label and menuitem associated
		auto l_pLabel = Label::createWithTTF((*it),
				"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.02);
		l_pLabel->setColor(Color3B::BLACK);

		auto l_pCheckBox = ui::CheckBox::create(
				"Ludomuse/GUIElements/backgroundDevice.png",
				"Ludomuse/GUIElements/backgroundDevicePressed.png");
		l_pCheckBox->setTouchEnabled(true);
		l_pCheckBox->setSwallowTouches(false);
		l_pCheckBox->addEventListener(
				CC_CALLBACK_2(LmMenu::updateUser2NameTablet, this));

		float l_fWidthButton = l_pCheckBox->getContentSize().width;
		float l_fHeightButton = l_pCheckBox->getContentSize().height;

		l_pLabel->setAnchorPoint(Vec2(0.5, 0.5));
		l_pLabel->setPosition(
				Vec2(l_pCheckBox->getContentSize().width * 0.5,
						l_pCheckBox->getContentSize().height * 0.5));
		l_pLabel->setColor(Color3B::WHITE);
		l_pLabel->setMaxLineWidth(l_fWidthButton * 0.9);
		l_pCheckBox->addChild(l_pLabel);
		m_aMenuItemUserTabletName.insert(
		{ l_pCheckBox, l_pLabel });

		l_pCheckBox->setAnchorPoint(Vec2(0, 0.5));

		if (m_oWindowListOfDevice.origin.x + (l_fWidthButton * l_iIndex + 1)
				> m_oWindowListOfDevice.getMaxX() - l_fWidthButton)
		{
			l_iLine++;
			l_iIndex = 0;
		}
		l_pCheckBox->setPosition(
				Vec2(
						m_oWindowListOfDevice.origin.x
								+ (l_fWidthButton * l_iIndex),
						m_oWindowListOfDevice.getMaxY()
								- (l_iLine * l_fHeightButton)));
		m_pWifiLayer->addChild(l_pCheckBox);
		l_iIndex++;
	}

}

void LmMenu::updateUser2NameTablet(cocos2d::Ref* p_Sender,
		cocos2d::ui::CheckBox::EventType)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	auto l_pCheckBoxPressed = dynamic_cast<ui::CheckBox*>(p_Sender);
	auto l_pLabel = m_aMenuItemUserTabletName.find(l_pCheckBoxPressed)->second;
	const char* l_sDeviceName = l_pLabel->getString().c_str();

	//if we don't click the same device name
	if (strcmp(l_sDeviceName, m_pUser2->getPUserTabletName().c_str()))
	{
		//uncheck every checkbox except the one clicked
		for (std::map<cocos2d::ui::CheckBox*, cocos2d::Label*>::iterator it =
				m_aMenuItemUserTabletName.begin();
				it != m_aMenuItemUserTabletName.end(); ++it)
		{
			it->first->setSelected(false);
		}

		//set user 2 tablet name
		m_pUser2->setPUserTabletName(l_sDeviceName);

		CCLOG("connect to  %s", m_pUser2->getPUserTabletName().c_str());
		_wifiFacade->connectTo(m_pUser2->getPUserTabletName());

		m_bConnected = true;

		if (m_bReady)
		{
			ready(nullptr);
		}
	}

	l_pCheckBoxPressed->setSelected(true);

	if (!m_bRoleSelected)
	{
		//set role button visible
		m_pCheckBoxChild->setVisible(true);
		m_pCheckBoxParent->setVisible(true);

		m_pLabelFeedback->setString(s_sRoleNotChoose);
	}


}

std::string LmMenu::getUser1TabletName()
{
	WIFIFACADE->askTabletName();

	return WIFIFACADE->getTabletName();
}

void LmMenu::inputEnabled(bool enabled)
{
	//menuitemtabletname
	for (std::map<cocos2d::ui::CheckBox*, cocos2d::Label*>::iterator it =
			m_aMenuItemUserTabletName.begin();
			it != m_aMenuItemUserTabletName.end(); ++it)
	{
		it->first->setEnabled(enabled);
	}

	//checkbox parent child
	m_pCheckBoxChild->setEnabled(enabled);
	m_pCheckBoxParent->setEnabled(enabled);

	m_pReadyButton->setEnabled(enabled);

	m_pRefreshButton->setEnabled(enabled);
}

void LmMenu::onUserIsReadyEvent(bytes l_oMsg)
{
	auto l_pUserBuffer = l_oMsg.read<LmUser>(); //the other player
	CCLOG("onUserIsReadyEvent user receive = %s",
			m_pUser2->getUserSerialized().c_str());

	if (l_pUserBuffer)
	{
		//check if user are compatible
		if (m_bReady)
		{

			if (usersAreCompatible(l_pUserBuffer))
			{
				delete m_pUser2;
				m_pUser2 = l_pUserBuffer;

				//disable input to be sure to not change his state
				inputEnabled(false);

				//send the msg
				bytes msg(100);
				msg << LmEvent::CompatibleToPlay << *m_pUser2 << *m_pUser1;
				WIFIFACADE->sendBytes(msg);
			}
			else
			{
				delete l_pUserBuffer;
			}

		}

	}

}

void LmMenu::onCompatibleToPlayEvent(bytes l_oMsg)
{
	//init user to compare

	LmUser* l_pUserBuffer = l_oMsg.read<LmUser>();

	CCLOG("user buffer = %s, user1 = %s",
			l_pUserBuffer->getUserSerialized().c_str(),
			m_pUser1->getUserSerialized().c_str());

	//it's an answer from the guy we send UserIsReady
	if (m_bReady)
	{
		if (l_pUserBuffer->getUserSerialized().compare(
				m_pUser1->getUserSerialized()) == 0)
		{
			CCLOG("on compatible true");
			//set second user
			delete m_pUser2;
			m_pUser2 = l_oMsg.read<LmUser>();

			//send the msg
			bytes msg(10);
			msg << LmEvent::Play;
			msg.write(true);
			WIFIFACADE->sendBytes(msg);

			ON_CC_THREAD(LmMenu::menuIsFinished, this);

		}
		else
		{
			CCLOG("on compatible false");
			//send the msg
			bytes msg(10);
			msg << LmEvent::Play;
			msg.write(false);
			WIFIFACADE->sendBytes(msg);

			CCLOG("m_pLabelFeedback->setString(s_sError);");
			m_pLabelFeedback->setString(s_sError);
			if (m_bReady)
			{
				ready(nullptr);

			}
		}
	}
	else
	{
		//if unclick debloque l'autre tablette
		CCLOG("on compatible false");
		//send the msg
		bytes msg(10);
		msg << LmEvent::Play;
		msg.write(false);
		WIFIFACADE->sendBytes(msg);
	}

	delete l_pUserBuffer;
}

void LmMenu::onPlayEvent(bytes l_oMsg)
{

	if (l_oMsg.readBool())
	{
		menuIsFinished();
	}
	else
	{
		//unblock input
		inputEnabled(true);
		CCLOG("m_pLabelFeedback->setString(s_sError);");
		m_pLabelFeedback->setString(s_sError);
		if (m_bReady)
		{
			ready(nullptr);

		}

	}
}

bool LmMenu::usersAreCompatible(LmUser* userToCompare)
{
	CCLOG("user received %s", userToCompare->getUserSerialized().c_str());
	CCLOG("user 1 %s", m_pUser1->getUserSerialized().c_str());
	CCLOG("user 2 %s", m_pUser2->getUserSerialized().c_str());

	if ((userToCompare->isBParent() && m_pUser1->isBParent())
			|| (!userToCompare->isBParent() && !m_pUser1->isBParent()))
	{
		m_pLabelFeedback->setString(s_sUserNotCompatibleRole);
		return false;
	}
	else if (userToCompare->getPUserTabletName().compare(
			m_pUser2->getPUserTabletName()) != 0)
	{
		m_pLabelFeedback->setString(s_sUserNotCompatibleTabletName);
		return false;
	}

	CCLOG("USER COMPATIBLE");
	return true;
}

void LmMenu::setReadyVisible(bool visible)
{
	m_pReadyButton->setVisible(visible);
	m_pSpriteReadyIndicator->setVisible(visible);
}

void LmMenu::removeMenuElement()
{
	m_pMenu->removeChild(m_pMenuItemImageFemale, true);
	m_pMenu->removeChild(m_pMenuItemImageMale, true);
}

void LmMenu::setLabelFeedBack()
{
	m_pLabelFeedback->setString(s_sBegining);
}



