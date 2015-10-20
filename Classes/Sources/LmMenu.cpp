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
	m_pSpriteReadyIndicator = nullptr;
	m_pCheckBoxChild = nullptr;
	m_pCheckBoxParent = nullptr;

	//primitive type
	m_bNoGenderSelected = true;
	m_bRoleSelected = false;
	m_bConnected = false;
	m_bReady = false;
	m_bMenuIsFinished = false;

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

	//add the sprite of the form
	auto l_pFormSprite = Sprite::create("Ludomuse/GUIElements/form.png");
	l_pFormSprite->setPosition(l_oVisibleSize.width / 2 + l_oOrigin.x,
			l_oVisibleSize.height / 2 + l_oOrigin.y);
	m_pLogLayer->addChild(l_pFormSprite);

	//log button
	auto l_oLogButton = MenuItemImage::create(
			"Ludomuse/GUIElements/logNormal.png",
			"Ludomuse/GUIElements/logPressed.png",
			CC_CALLBACK_1(LmMenu::wifiDirectScreen, this));
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
	m_pLogEditBox->setPlaceHolder("Name");
	m_pLogEditBox->setFontSize(l_oVisibleSize.width * 0.03);
	m_pLogEditBox->setFontName("Fonts/JosefinSans-Regular.ttf");
	m_pLogEditBox->setFontColor(Color3B::BLACK);
	m_pLogEditBox->setMaxLength(s_iMaxLenghtUserName);
	m_pLogEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pLogEditBox->retain();
	m_pLogLayer->addChild(m_pLogEditBox, 1);

	//test
	m_pLogEditBox->setText("UserName");

	//init checkbox male
	m_pCheckBoxMale = CheckBox::create(
			"Ludomuse/GUIElements/maleUnselected.png",
			"Ludomuse/GUIElements/maleSelected.png");
	m_pCheckBoxMale->setTouchEnabled(true);
	m_pCheckBoxMale->setSwallowTouches(false);
	m_pCheckBoxMale->setAnchorPoint(Vec2(1, 0.5));
	m_pCheckBoxMale->setPosition(
			Vec2(
					m_pLogEditBox->getPosition().x
							- m_pLogEditBox->getContentSize().width * 0.2,
					l_oVisibleSize.height * 0.5f
							- l_pFormSprite->getContentSize().height * 0.3));
	m_pCheckBoxMale->addEventListener(
			CC_CALLBACK_2(LmMenu::maleSelected, this));
	m_pLogLayer->addChild(m_pCheckBoxMale);

	//init checkbox female
	m_pCheckBoxFemale = CheckBox::create(
			"Ludomuse/GUIElements/femaleUnselected.png",
			"Ludomuse/GUIElements/femaleSelected.png");
	m_pCheckBoxFemale->setTouchEnabled(true);
	m_pCheckBoxFemale->setSwallowTouches(false);
	m_pCheckBoxFemale->setAnchorPoint(Vec2(0, 0.5));
	m_pCheckBoxFemale->setPosition(
			Vec2(
					m_pLogEditBox->getPosition().x
							+ m_pLogEditBox->getContentSize().width * 0.2,
					l_oVisibleSize.height * 0.5f
							- l_pFormSprite->getContentSize().height * 0.3));
	m_pCheckBoxFemale->addEventListener(
			CC_CALLBACK_2(LmMenu::femaleSelected, this));
	m_pLogLayer->addChild(m_pCheckBoxFemale);

	return true;
}

bool LmMenu::wifiDirectScreen(cocos2d::Ref* l_oSender)
{
	//check if the user enter smthg
	if (strcmp(m_pLogEditBox->getText(), "") == 0 || m_bNoGenderSelected)
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

		//sprite to indicate if ready or not
		m_pSpriteReadyIndicator = Sprite::create(
				"Ludomuse/GUIElements/nextButtonPressed.png");
		m_pSpriteReadyIndicator->setAnchorPoint(Vec2(1, 0.5));
		m_pSpriteReadyIndicator->setPosition(
				Vec2(l_oVisibleSize.width - s_fMarginLeftMenu,
						l_oVisibleSize.height * 0.2f));
		m_pWifiLayer->addChild(m_pSpriteReadyIndicator);

		//ready button
		m_pReadyButton = MenuItemImage::create(
				"Ludomuse/GUIElements/playNormal.png",
				"Ludomuse/GUIElements/playPressed.png",
				CC_CALLBACK_1(LmMenu::ready, this));
		m_pReadyButton->setAnchorPoint(Point(1, 0.5));
		m_pReadyButton->setPosition(
				Vec2(
						l_oVisibleSize.width - s_fMarginLeftMenu
								- m_pSpriteReadyIndicator->getContentSize().width,
						l_oVisibleSize.height * 0.2f));

		//refresh button
		auto l_pRefreshButton = MenuItemImage::create(
				"Ludomuse/GUIElements/logNormal.png",
				"Ludomuse/GUIElements/logPressed.png",
				CC_CALLBACK_1(LmMenu::scan, this));
		l_pRefreshButton->setAnchorPoint(Point(0.5, 0.5));
		l_pRefreshButton->setPosition(
				Vect(l_oVisibleSize.width * 0.5f,
						l_oVisibleSize.height * 0.2f));

		// create menu
		m_pMenu = Menu::create(m_pReadyButton, l_pRefreshButton, nullptr);
		m_pMenu->setPosition(Vec2::ZERO);
		m_pWifiLayer->addChild(m_pMenu, 1);

		//create menu for the list of name device
		m_pMenuUserTabletName = Menu::create();
		m_pMenuUserTabletName->setPosition(Vec2::ZERO);
		m_pWifiLayer->addChild(m_pMenuUserTabletName, 1);

		//init checkbox parent
		m_pCheckBoxParent = CheckBox::create(
				"Ludomuse/GUIElements/logNormal.png",
				"Ludomuse/GUIElements/logPressed.png");
		m_pCheckBoxParent->setTouchEnabled(true);
		m_pCheckBoxParent->setSwallowTouches(false);
		m_pCheckBoxParent->setAnchorPoint(Vec2(0, 0.5));
		m_pCheckBoxParent->setPosition(
				Vec2(s_fMarginLeftMenu, l_oVisibleSize.height * 0.2));
		m_pCheckBoxParent->addEventListener(
				CC_CALLBACK_2(LmMenu::parentSelected, this));
		m_pWifiLayer->addChild(m_pCheckBoxParent);

		//init checkbox child
		m_pCheckBoxChild = CheckBox::create(
				"Ludomuse/GUIElements/playNormal.png",
				"Ludomuse/GUIElements/playPressed.png");
		m_pCheckBoxChild->setTouchEnabled(true);
		m_pCheckBoxChild->setSwallowTouches(false);
		m_pCheckBoxChild->setAnchorPoint(Vec2(0, 0.5));
		m_pCheckBoxChild->setPosition(
				Vec2(
						s_fMarginLeftMenu
								+ m_pCheckBoxParent->getCustomSize().width,
						l_oVisibleSize.height * 0.2));
		m_pCheckBoxChild->addEventListener(
				CC_CALLBACK_2(LmMenu::childSelected, this));
		m_pWifiLayer->addChild(m_pCheckBoxChild);

		//init user1
		m_pUser1->setPScore(0);
		m_pUser1->setPUserName(m_pLogEditBox->getText());
		m_pUser1->setPUserTabletName(getUser1TabletName());

		_wifiFacade->discoverPeers();

		return true;

	}
}

void LmMenu::ready(cocos2d::Ref* l_oSender)
{

	if (!m_bReady)
	{
		//can't be ready if no role selected and no user 2 choose
		if (m_bRoleSelected && m_bConnected)
		{
			//wer are ready
			m_bReady = true;

			m_pSpriteReadyIndicator->setTexture(
					"Ludomuse/GUIElements/nextButtonNormal.png");

			//send the msg
			bytes msg(50);
			msg << LmEvent::UserIsReady << *m_pUser1;
			WIFIFACADE->sendBytes(msg);

		}
	}
	else
	{
		//not ready anymore
		m_bReady = false;

		m_pSpriteReadyIndicator->setTexture(
				"Ludomuse/GUIElements/nextButtonPressed.png");
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

void LmMenu::parentSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType)
{

	if (m_bReady)
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

	CCLOG("parent selected");

}

void LmMenu::childSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType)
{

	if (m_bReady)
	{
		//unclick the ready button
		ready(nullptr);
	}

	//update state of checkbox
	m_pCheckBoxChild->setSelected(true);
	m_pCheckBoxParent->setSelected(false);
	m_pUser1->setBParent(false);
	m_bRoleSelected = true;

	CCLOG("child selected");
}

void LmMenu::scan(cocos2d::Ref* l_pSender)
{
	CCLOG("scan");
	_wifiFacade->discoverPeers();
}

void LmMenu::makeMenuItemUserTabletName(
		std::vector<std::string> l_aVectorOfTabletName)
{
	CCLOG("makemenuitem");
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	m_pMenuUserTabletName->removeAllChildrenWithCleanup(true);

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
				"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.03);
		l_pLabel->setColor(Color3B::BLACK);

		auto l_pMenuItemImage = MenuItemImage::create(
				"Ludomuse/GUIElements/playNormal.png",
				"Ludomuse/GUIElements/playPressed.png",
				CC_CALLBACK_1(LmMenu::updateUser2NameTablet, this));

		float l_fWidthButton = l_pMenuItemImage->getContentSize().width;
		float l_fHeightButton = l_pMenuItemImage->getContentSize().height;

		l_pLabel->setAnchorPoint(Vec2(0.5, 0.5));
		l_pLabel->setPosition(
				Vec2(l_pMenuItemImage->getContentSize().width * 0.5,
						l_pMenuItemImage->getContentSize().height * 0.5));
		l_pLabel->setMaxLineWidth(l_fWidthButton);
		l_pMenuItemImage->addChild(l_pLabel);
		m_aMenuItemUserTabletName.insert(
		{ l_pMenuItemImage, l_pLabel });

		l_pMenuItemImage->setAnchorPoint(Vec2(0, 0.5));

		if (s_fMarginLeftMenu + (l_fWidthButton * l_iIndex + 1)
				> l_oVisibleSize.width - s_fMarginLeftMenu - l_fWidthButton)
		{
			l_iLine++;
			l_iIndex = 0;
		}
		l_pMenuItemImage->setPosition(
				Vec2(s_fMarginLeftMenu + (l_fWidthButton * l_iIndex),
						l_oVisibleSize.height * 0.9
								- (l_iLine * l_fHeightButton)));
		m_pMenuUserTabletName->addChild(l_pMenuItemImage);
		l_iIndex++;
	}

}

void LmMenu::onGettingPeers(std::vector<std::string> peers)
{

	ON_CC_THREAD(LmMenu::makeMenuItemUserTabletName, this, peers);

}

void LmMenu::updateUser2NameTablet(cocos2d::Ref* p_Sender)
{
	auto l_pMenuItemPressed = dynamic_cast<MenuItemImage*>(p_Sender);
	auto l_pLabel = m_aMenuItemUserTabletName.find(l_pMenuItemPressed)->second;
	const char* l_sDeviceName = l_pLabel->getString().c_str();

	//if we don't click the same device name
	if (strcmp(l_sDeviceName, m_pUser2->getPUserTabletName().c_str()))
	{
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

}

std::string LmMenu::getUser1TabletName()
{
	WIFIFACADE->askTabletName();

	return WIFIFACADE->getTabletName();
}

void LmMenu::inputEnabled(bool enabled)
{

	CCLOG("on input Enabled");
	//menuitemtabletname
	for (std::map<cocos2d::MenuItemImage*, cocos2d::Label*>::iterator it =
			m_aMenuItemUserTabletName.begin();
			it != m_aMenuItemUserTabletName.end(); ++it)
	{
		it->first->setEnabled(enabled);
	}

	//checkbox parent child
	m_pCheckBoxChild->setEnabled(enabled);
	m_pCheckBoxParent->setEnabled(enabled);

	m_pReadyButton->setEnabled(enabled);
	CCLOG("exit input Enabled");
}

void LmMenu::onReceivingMsg(bytes l_oMsg)
{
	CCLOG("lm menu_event is %d", _event);
	switch (_event)
	{
	case LmEvent::UserIsReady:
		CCLOG("onUserIsReadyEvent");
		onUserIsReadyEvent(l_oMsg);
		break;
	case LmEvent::CompatibleToPlay:
		CCLOG("onCompatibleToPlayEvent");
		onCompatibleToPlayEvent(l_oMsg);
		break;
	case LmEvent::Play:
		CCLOG("onPlayEvent");
		onPlayEvent(l_oMsg);
		break;
	default:
		break;
	}

}

void LmMenu::onUserIsReadyEvent(bytes l_oMsg)
{
	m_pUser2 = l_oMsg.read<LmUser>();
	CCLOG("onUserIsReadyEvent user receive = %s",
			m_pUser2->getUserSerialized().c_str());

	if (m_pUser2)
	{
		//check if user are compatible just check parent child stuff maybe add tabletname recognition aswell
		if (usersAreCompatible() && m_bReady)
		{
			//disable input to be sure to not change his state
			ON_CC_THREAD(LmMenu::inputEnabled, this, false);

			//send CompatibleToPlay user A & B as parameters TODO

			CCLOG("send CompatibleToPlay {%s;%s} to %s",
					m_pUser1->getUserSerialized().c_str(),
					m_pUser2->getUserSerialized().c_str(),
					m_pUser2->getPUserTabletName().c_str());

			//send the msg
			bytes msg(100);
			msg << LmEvent::CompatibleToPlay << *m_pUser2 << *m_pUser1;
			WIFIFACADE->sendBytes(msg);

		}
		else
		{
			CCLOG("user not compatible or user1 not ready");
		}
	}
	else
	{
		CCLOG("no user in the msg OnUserIsReady");
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

	if (l_pUserBuffer->getUserSerialized().compare(
			m_pUser1->getUserSerialized()) == 0 && m_bReady)
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
	}

	delete l_pUserBuffer;
	CCLOG("end oncompatible event");
}

void LmMenu::onPlayEvent(bytes l_oMsg)
{

	if (l_oMsg.readBool())
	{
		ON_CC_THREAD(LmMenu::menuIsFinished, this);

	}
	else
	{
		//unblock input
		ON_CC_THREAD(LmMenu::inputEnabled, this, true);
	}
}

bool LmMenu::usersAreCompatible()
{
	if (((m_pUser2->isBParent() && !m_pUser1->isBParent())
			|| (!m_pUser2->isBParent() && m_pUser1->isBParent())))
	{
		CCLOG("users are compatible");
	}

	return ((m_pUser2->isBParent() && !m_pUser1->isBParent())
			|| (!m_pUser2->isBParent() && m_pUser1->isBParent()));
}

