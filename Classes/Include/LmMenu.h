/*
 * LmMenu.h
 *
 *  Created on: 9 sept. 2015
 *      Author: IHMTEKDev4
 *
 *      this is the log screen and the menu screen
 */

#ifndef CLASSES_INCLUDE_LMMENU_H_
#define CLASSES_INCLUDE_LMMENU_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "LmUser.h"
#include "LmJniCppFacade.h"
#include "LmWifiObserver.h"

//message of the feedback label
static const std::string s_sUserIsReady = "En attente de ton partenaire."; //ok
static const std::string s_sDeviceNotConnected =
		"Tu dois choisir une tablette avec qui te connecter !"; //ok
static const std::string s_sRoleNotChoose = "Choisis le role que tu vas jouer pendant cette partie."; //ok
static const std::string s_sBegining = "Choisis une tablette avec qui jouer"; //ok
static const std::string s_sUserNotCompatible =
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const std::string s_sYouAreNotReady =
		"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
static const std::string s_sError =
		"ERRRRROOROROR";

//Parameters
static const float s_fSplashScreenDuration = 0.5f;
static const int s_iMaxLenghtUserName = 10;
static const float s_fMarginLeftMenu = 50.0f;

//filename sprite
static const std::string s_sFilenameSpriteBackground =
		"Ludomuse/Background/menu.png";
static const std::string s_sFilenameSplash = "Ludomuse/Background/splash.png";

class LmMenu: public LmWifiObserver
{
public:

	LmMenu();
	~LmMenu();

	//display a sprite during s_fSplashScreenDuration then call logscreen method call from AppDelegate
	void splashScreen();

	LmUser* getPUser1() const
	{
		return m_pUser1;
	}

	LmUser* getPUser2() const
	{
		return m_pUser2;
	}

	//return a vector of string with device name
	virtual void onGettingPeers(std::vector<std::string>);

	//callback method when msg is received
	virtual void onReceivingMsg(bytes);

private:

	//********************ATTRIBUTES********************

	//the scene of the menu (log + wifi)
	cocos2d::Scene* m_pLmMenuScene;

	//sprite of the splashscreen
	cocos2d::Sprite* m_pSpriteSplashScreen;

	//both users (1=local & 2=2nd user)
	LmUser* m_pUser1;
	LmUser* m_pUser2;

	/*
	 * LOG SCREEN
	 */
	//layer
	cocos2d::Layer* m_pLogLayer;
	//sprite
	cocos2d::Sprite* m_pSpriteLogBackground;
	//textfield
	cocos2d::ui::EditBox* m_pLogEditBox;
	//checkbox
	cocos2d::MenuItemImage* m_pMenuItemImageMale;
	cocos2d::MenuItemImage* m_pMenuItemImageFemale;

	/*
	 * WIFI DIRECT SCREEN
	 */
	//layer
	cocos2d::Layer* m_pWifiLayer;
	//sprite
	cocos2d::Sprite* m_pSpriteWifiBackground;
	cocos2d::Sprite* m_pSpriteReadyIndicator; //to indicate when ready
	//feedback user
	cocos2d::Label* m_pLabelFeedback;
	//menu
	cocos2d::Menu* m_pMenu;
	//menuitem image
	cocos2d::MenuItemImage* m_pReadyButton;
	std::map<cocos2d::ui::CheckBox*, cocos2d::Label*> m_aMenuItemUserTabletName;
	//checkbox
	cocos2d::ui::CheckBox* m_pCheckBoxParent;
	cocos2d::ui::CheckBox* m_pCheckBoxChild;
	//the window to display the list of device
	cocos2d::Rect m_oWindowListOfDevice;

	/*
	 * BOOLEAN
	 */
	bool m_bMenuIsFinished;
	//to knwo when a role have been picked parent or child
	bool m_bRoleSelected;
	//to know when we are connected to someone
	bool m_bConnected;
	//to know when ready or not
	bool m_bReady;

	//to stop listening
	int m_iIdWifiObserver;

	//********************METHODS********************

	//remove splashscreen and init log screen
	bool logScreen();
	void logFinished(cocos2d::Ref*);
	//callback method of checkbox role
	void parentSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);
	void childSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);
	//remove logscreen and init wifi direct screen
	bool wifiDirectScreen();
	//get the tablet name of the user 1
	std::string getUser1TabletName();
	//callback method of checkbox gender
	void maleSelected(cocos2d::Ref*);
	void femaleSelected(cocos2d::Ref*);
	//launch discover peers method in java
	void scan(cocos2d::Ref*);
	//make menuitemiamge with the vector of string of wifi direct names
	void makeCheckboxUserTabletName(std::vector<std::string>);
	//set user 2 according to the menuitemiamge pressed
	void updateUser2NameTablet(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);
	//setvisible when role are choosen
	void setReadyVisible(bool);
	//callback method of the ready button
	void ready(cocos2d::Ref*);
	//check if user are compatible return true if yes
	bool usersAreCompatible();
	//block input of the wifi direct screen
	void inputEnabled(bool);
	//function to end menu by sending a custom event
	void menuIsFinished();
	//callback methods of events
	void onUserIsReadyEvent(bytes);
	void onCompatibleToPlayEvent(bytes);
	void onPlayEvent(bytes);

};

#endif /* CLASSES_INCLUDE_LMMENU_H_ */
