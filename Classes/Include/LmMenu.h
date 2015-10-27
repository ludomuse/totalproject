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
#include "../Include/LmJniCppFacade.h"
#include "../Include/LmWifiObserver.h"

//message of the feedback label
static const std::string s_sUserIsReady = "En attente de ton partenaire.";//ok
static const std::string s_sDeviceNotConnected = "Tu dois choisir une tablette avec qui te connecter !";//ok
static const std::string s_sRoleNotChoose = "Tu dois choisir un role.";//ok
static const std::string s_sBegining = "Choisis un role et un partenaire.";//ok
static const std::string s_sUserNotCompatible = "Message reçu, mais vos role doivent etre different !";
static const std::string s_sYouAreNotReady = "Message reçu, mais tu n'es pas pret...";
static const std::string s_sError = "Erreur lors de la synchronisation, veuillez recommencer.";


static const float s_fSplashScreenDuration = 0.5f;
static const int s_iMaxLenghtUserName = 10;
static const float s_fMarginLeftMenu = 70.0f;
//filename sprite
static const std::string s_sFilenameSpriteBackground = "Ludomuse/Background/menu.png";
static const std::string s_sFilenameSplash = "Ludomuse/Background/splash.png";

class LmMenu : public LmWifiObserver
{
public:

	LmMenu();
	~LmMenu();

	//display a sprite during s_fSplashScreenDuration then call logscreen method cll from AppDelegate
	void splashScreen();

	LmUser* getPUser1() const
	{
		return m_pUser1;
	}

	LmUser* getPUser2() const
	{
		return m_pUser2;
	}

	virtual void onGettingPeers(std::vector<std::string>);

	virtual void onReceivingMsg(bytes);

private:

	//ATTRIBUTES

	//feedback user
	cocos2d::Label* m_pLabelFeedback;

	int m_iIdWifiObserver;

	bool m_bMenuIsFinished;


	//the scene of the menu (log + wifi)
	cocos2d::Scene* m_pLmMenuScene;

	//our two layer one handle the loging the other the wifidirect
	cocos2d::Layer* m_pLogLayer;
	cocos2d::Layer* m_pWifiLayer;

	//background img
	cocos2d::Sprite* m_pSpriteLogBackground;
	cocos2d::Sprite* m_pSpriteWifiBackground;

	cocos2d::Sprite* m_pSpriteSplashScreen;

	//textfield
	cocos2d::ui::EditBox* m_pLogEditBox;

	//both users (1=local & 2=2nd user)
	LmUser* m_pUser1;
	LmUser* m_pUser2;

	//to knwo when a role have been picked parent or child
	bool m_bRoleSelected;
	//to know when we are connected to someone
	bool m_bConnected;
	//to know when ready or not
	bool m_bReady;

	//sprite to indicate when ready
	cocos2d::Sprite* m_pSpriteReadyIndicator;

	//to know gender

	cocos2d::ui::CheckBox* m_pCheckBoxMale;
	cocos2d::ui::CheckBox* m_pCheckBoxFemale;
	cocos2d::ui::CheckBox* m_pCheckBoxParent;
	cocos2d::ui::CheckBox* m_pCheckBoxChild;

	cocos2d::MenuItemImage* m_pReadyButton;

	//stock menuitemlabel
	std::map<cocos2d::MenuItemImage*,cocos2d::Label*> m_aMenuItemUserTabletName;
	cocos2d::Menu* m_pMenu;
	cocos2d::Menu* m_pMenuUserTabletName;

	bool m_bNoGenderSelected;

	//METHODS

	//callback button methods
	bool wifiDirectScreen(cocos2d::Ref*);

	//function to end menu by sending a custom event
	void ready(cocos2d::Ref*);
	void menuIsFinished();


	//callback method of splashscreen
	bool logScreen();

	//callback method of answer box
	void maleSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);
	void femaleSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);

	void parentSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);
	void childSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);

	void scan(cocos2d::Ref*);
	void updateUser2NameTablet(cocos2d::Ref*);

	void makeMenuItemUserTabletName(std::vector<std::string> );

	//block input
	void inputEnabled(bool );

	//get the tablet name of the user 1
	std::string getUser1TabletName();

	bool usersAreCompatible();

	//callback methods of events
	void onUserIsReadyEvent(bytes);
	void onCompatibleToPlayEvent(bytes);
	void onPlayEvent(bytes);











};

#endif /* CLASSES_INCLUDE_LMMENU_H_ */
