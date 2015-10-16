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
#include "../Include/LmWifiDirectFacade.h"
#include "../Include/LmJniCppFacade.h"
#include "../Include/LmWifiObserver.h"

static const float s_fMagingRatioOfSpriteBackgroundUser2Profile = 0.5f;
static const float s_fTimeCompareAction = 0.5f;
static const float s_fMarginBetweenInteraction = 260.0f;

class LmGameManager : public LmWifiObserver
{

public:

	LmGameManager(LmWifiDirectFacade* a_wifiFacade);
	~LmGameManager();

	//call different method of gamemanager, init launch scene and handle layers of interactions
	void runGame();

	void setPUser1(LmUser* pUser1)
	{
		m_pUser1 = pUser1;
	}

	void setPUser2(LmUser* pUser2)
	{
		m_pUser2 = pUser2;
	}

	inline void send(bool b)
	{
		m_pWifiDirectFacade->send(b);
	}

	virtual void onReceiving(int);

	virtual void onReceivingAccuse();

private:

	//ATTRIBUTES

	LmWifiDirectFacade* m_pWifiDirectFacade;

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
	cocos2d::Layer* m_pBlueLayer;
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
	cocos2d::ui::Button* m_pPlayNextInteractionButton;

	//need to be member so we can make it visible or not
	cocos2d::Sprite* m_pAvatarSpriteUser2;

	cocos2d::Label* m_pLabelScoreUser2;
	cocos2d::Sprite* m_pStarUser2Sprite;
	cocos2d::Label* m_pLabelInteractionDoneUser2;
	cocos2d::Sprite* m_pCheckSpriteUser2;

	//vector interaction sprite
	std::vector<cocos2d::Sprite*> m_aSpritesInteractions;

	//to know how many interaction have been played
	int m_iIndexInteractionScene;

	//to know how many inetraction done
	int m_iInteractionDone;

	//to know if it's a back to the dashboard or its interactionscenefinished
	bool m_bBackToDashboard;

	//listener
	cocos2d::EventListenerTouchOneByOne* m_pListener;
	cocos2d::Sprite* m_pDescriptionBox;
	cocos2d::Label* m_pDescriptionLabel;

	//METHODS

	//call server manager and init m_vectorSceneOfTheGame
	bool init();

	//callback methods
	void compare();
	void compareDone();
	void back();
	void backDone();

	void runNextInteraction();

	//bool to sync when compare button is pushed
	bool m_bActionIsDone;

	//init the dashboard
	bool initDashboard();

	//init splash screen
	bool initSplashScreen();

	//put interaction sprite on the dashboard
	void initDashboardInteraction();

	//update dashboard after each interactionscene
	void updateDashboard();

	//remove listeners avoid conflict between scene true = remove false = add
	void removeListeners(bool);

	//callback method for the layer splash
	bool onTouchBeganSplashScreen(cocos2d::Touch*, cocos2d::Event*);

	//to init texture of the user
	cocos2d::Sprite* makeUserAvatarSprite(LmUser*);

	//to add a star sprite to a label true = red; false = green
	cocos2d::Sprite* addSpriteToLabel(cocos2d::Label*, std::string);

	//callback method of parent layer
	bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);

	//return index of interaction sprite touched
	int interactionTouched(cocos2d::Touch*);

	//update position of chack and the star
	void updateSpriteToLabel(cocos2d::Sprite* ,cocos2d::Label* );


};

#endif // LMGAMEMANAGER_H
