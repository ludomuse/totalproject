/*************************************************************************
 LmGameManager  -  Manage all steps
 *************************************************************************/
#ifndef LMGAMEMANAGER_H__
#define LMGAMEMANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "LmServerManager.h"
#include "LmUser.h"
#include "LmInteractionScene.h"
#include "LmWifiDirectFacade.h"
#include "LmJniCppFacade.h"
#include "LmWifiObserver.h"
#include "LmSettings.h"

static const float s_fMagingRatioOfSpriteBackgroundUser2Profile = 0.5f;
static const float s_fTimeCompareAction = 0.5f;
static const float s_fMarginBetweenInteraction = 260.0f;

class LmGameManager: public LmWifiObserver
{

public:

	static const char* s_sFilenameButtonClicked;

	LmGameManager();
	~LmGameManager();

	//call different method of gamemanager, init launch scene and handle layers of interactions
	void runGames();

	void setPUser1(LmUser* pUser1)
	{
		m_pUser1 = pUser1;
	}

	void setPUser2(LmUser* pUser2)
	{
		m_pUser2 = pUser2;
	}

	virtual void onReceivingMsg(bytes);

	//to init texture of the user
	static cocos2d::Sprite* makeUserAvatarSprite(LmUser*);

private:

	//ATTRIBUTES

	//menu settings
	LmSettings* m_pLmSettings;

	//checkbox to make appear setting
	cocos2d::MenuItemImage* m_pSettingsButton;

	//layer splash
	cocos2d::Layer* m_pSplashSreenLayer;

	std::vector<LmInteractionScene*> m_aInteractionSceneOfTheGame;

	//use to get the vector of interactionScene
	LmServerManager* m_pLmServerManager;

	//both users (1=local & 2=2nd user) get via LmMenu
	LmUser* m_pUser1;
	LmUser* m_pUser2;

	//scene of the dashboard
	cocos2d::Scene* m_pGameManagerScene;

	//layer of dashboard
	cocos2d::Layer* m_pBackgroundLayer;
	cocos2d::Layer* m_pPinkLayer;

	cocos2d::ui::ScrollView* m_pScrollView;
	cocos2d::Layer* m_pLayerScrollView;

	//dashboard GUI elements
	cocos2d::Sprite* m_pSpriteBackgroundBlue;
	cocos2d::Sprite* m_pSpriteBackgroundPink;

	cocos2d::Sprite* m_pSpriteBandMid;
	cocos2d::Sprite* m_pSpriteBandTop;

	cocos2d::Sprite* m_pSpriteBackgroundBlueProfile;
	cocos2d::Sprite* m_pSpriteBackgroundPinkProfile;

	cocos2d::Label* m_pLabelUser1Name;
	cocos2d::Label* m_pLabelScore;
	cocos2d::Sprite* m_pStarUser1Sprite;
	cocos2d::Label* m_pLabelInteractionDone;
	cocos2d::Sprite* m_pCheckSpriteUser1;
	cocos2d::Label* m_pLabelTitleApplication;
	std::string m_sTitleApplication;

	cocos2d::ui::Button* m_pCompareButton;
	cocos2d::Label* m_pLabelCompareButton;
	cocos2d::ui::Button* m_pBackButton;
	cocos2d::MenuItemImage* m_pPlayNextInteractionButton;

	//need to be member so we can make it visible or not
	cocos2d::Sprite* m_pAvatarSpriteUser2;

	cocos2d::Label* m_pLabelScoreUser2;
	cocos2d::Sprite* m_pStarUser2Sprite;
	cocos2d::Label* m_pLabelInteractionDoneUser2;
	cocos2d::Sprite* m_pCheckSpriteUser2;

	//vector interaction sprite
	std::vector<cocos2d::Sprite*> m_aSpritesInteractions;
	std::vector<cocos2d::Sprite*> m_aSpritesInteractionsUser2;

	//to know how many interaction have been played
	int m_iIndexInteractionScene;

	//to know how many inetraction done
	int m_iInteractionDone;
	//to know how many inetraction done user2
	int m_iInteractionDoneUser2;

	//to know if it's a back to the dashboard or its interactionscenefinished
	bool m_bBackToDashboard;

	//listener
	cocos2d::EventListenerTouchOneByOne* m_pListener;
	cocos2d::Sprite* m_pDescriptionBox;
	cocos2d::Label* m_pDescriptionLabel;

	//bool to sync when compare button is pushed
	bool m_bActionIsDone;

	//METHODS

	//call server manager and init m_vectorSceneOfTheGame
	bool init();

	//callback methods
	void compare();
	void compareDone();
	void back();
	void backDone();

	void runNextInteraction(cocos2d::Ref*);

	//init the dashboard
	bool initDashboard();

	//init splash screen
	bool initSplashScreen();

	//put interaction sprite on the dashboard
	void initDashboardInteraction();

	//update dashboard after each interactionscene
	void updateDashboard();

	//remove listeners avoid conflict between scene true = remove false = add
	void inputDisabled(bool);

	//callback method for the layer splash
	bool onTouchBeganSplashScreen(cocos2d::Touch*, cocos2d::Event*);

	//to add a star sprite to a label true = red; false = green
	cocos2d::Sprite* addSpriteToLabel(cocos2d::Label*, std::string);

	//callback method of parent layer
	bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);

	//return index of interaction sprite touched
	int interactionTouched(cocos2d::Touch*);

	//update position of chack and the star
	void updateSpriteToLabel(cocos2d::Sprite*, cocos2d::Label*);

	//list of callback event
	void onReadyForNextInteractionEvent(bytes);
	void onInteractionDoneEvent(bytes);

	//push the index scene interaction
	void runInteraction(int);

	//set visible lolipop user 2
	void setSpritesInteractionsUser2Visible(bool);

	//set visible element or not of the both state of dashboard
	void compareScreen(bool);

	void settings(cocos2d::Ref*);

};

#endif // LMGAMEMANAGER_H
