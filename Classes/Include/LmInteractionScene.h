/*************************************************************************
 LmInteractionScene  -  Generic step create an empty
 layer create a simple scene
 *************************************************************************/
#ifndef LMINTERACTION_H__
#define LMINTERACTION_H__

#include "cocos2d.h"
#include "LmGameComponent.h"
#include "LmSetPoint.h"
#include "LmUser.h"
#include "LmSprite.h"
#include "LmReward.h"

#include "LmJniCppFacade.h"
#include "LmWifiObserver.h"
#include "LmHelpers.h"

//use for stats
#include "LmStatistics.h"



typedef std::map<int, LmGameComponent*>::iterator it_type;

static const float s_fDashboardRatioHidden = 0.95f;
static const float s_fTimeMovingDashboard = 0.4f;
static const float s_fMarginLeft = 80.0f;
static const float s_fMarginBot = 20.0f;

//margin between image when images has to be display
static const float s_fMarginBetweenImage = 20.0f;



class LmInteractionScene: public cocos2d::Scene , public LmWifiObserver
{

public:

	LmInteractionScene();
	virtual ~LmInteractionScene();

	//in case we need to do stuff when we get the focus again after a back
	void restart();

	//call by replay button
	virtual void resetScene()=0;

	//init
	bool init(LmUser*);

	bool isDone() const
	{
		return m_bDone;
	}

	void setBBackPressed(bool bBackPressed)
	{
		m_bBackPressed = bBackPressed;
	}

	void setPLmSetPointEnd(LmSetPoint* pLmSetPoint)
	{
		m_pLmSetPointEnd = pLmSetPoint;
	}

	void setPLmSetPointBegin(LmSetPoint* pLmSetPoint)
	{
		m_pLmSetPointBegin = pLmSetPoint;
	}

	void setPLmReward(LmReward* pLmReward)
	{
		m_pLmReward = pLmReward;
	}

	void setPLmRewardUser2(LmReward* pLmRewardUser2)
	{
		m_pLmRewardUser2 = pLmRewardUser2;
	}

	bool getReward() const
	{
		if (m_pLmReward)
		{
			return true;
		}
		return false;
	}

	bool getRewardUser2() const
	{
		if (m_pLmRewardUser2)
		{
			return true;
		}
		return false;
	}

	const std::string& getSDescription() const
	{
		return m_sDescription;
	}

	void setSDescription(const std::string& sDescription)
	{
		m_sDescription = sDescription;
	}

	virtual void onReceivingMsg(bytes );
	virtual void onReceivingFile(std::string)
	{

	}

	bool startGame();

	void setBUser1IsReadyForNextInteraction(
			bool bUser1IsReadyForNextInteraction)
	{
		m_bUser1IsReadyForNextInteraction = bUser1IsReadyForNextInteraction;
	}

	void setBUser2IsReadyForNextInteraction(
			bool bUser2IsReadyForNextInteraction)
	{
		m_bUser2IsReadyForNextInteraction = bUser2IsReadyForNextInteraction;
	}

	bool isBGameIsRunning() const
	{
		return m_bGameIsRunning;
	}

	int getIIdGame() const
	{
		return m_iIdGame;
	}

	LmReward* getPLmReward() const
	{
		return m_pLmReward;
	}

	LmReward* getPLmRewardUser2() const
	{
		return m_pLmRewardUser2;
	}

	bool isBWin() const
	{
		return m_bWin;
	}

protected:

	//ATTRIBUTES

	static int s_iNumberOfInteraction;

	int m_iIdGame;

	//to sync users before launching a game
	bool m_bUser1IsReadyForNextInteraction;
	bool m_bUser2IsReadyForNextInteraction;

	//to know if game is running or not
	bool m_bGameIsRunning;

	cocos2d::Sprite* m_pSpriteWaitingScreen;


	//text to display in the dashboard to describe the scene
	std::string m_sDescription;

	//use to sync movement
	bool m_bTouchBeganDisabled;

	//to know if this has been win
	bool m_bWin;

	//reward of user 1
	LmReward* m_pLmReward;
	//user 2 reward
	LmReward* m_pLmRewardUser2;

	//autorelease object it's the first layer of the scene
	cocos2d::Layer* m_pLayerGame;

	//an introduction to the InteractionScene
	LmSetPoint* m_pLmSetPointBegin;
	LmSetPoint* m_pLmSetPointEnd;

	cocos2d::Layer* m_pDashboardBandLayer;

	//user which play the scene to know if it's 1 or 2 init dashbord layer and modify his score
	LmUser* m_pUser;

	//finish button
	cocos2d::ui::Button* m_pFinishGameButton;
	bool m_bFinishGameButtonSync;

	//replay button
	cocos2d::ui::Button* m_pReplayButton;
	bool m_bReplayButtonSync;

	//introduction gui element
	//buttons next and previous
	cocos2d::MenuItemImage* m_pNextButton;
	cocos2d::MenuItemImage* m_pPreviousButton;
	cocos2d::Menu* m_pMenu;
	cocos2d::ui::CheckBox* m_pPlayCheckBox;

	//to know when a set point is finish
	bool m_bSetPointFinished;
	//to know if next button pass through the begion set point begin or other one
	bool m_bSetPointBegin;

	//gui elements
	LmSprite* m_pSpriteDashboardBand;

	cocos2d::ui::Button* m_pMoveLayerButton;
	bool m_bMoveDone;
	bool m_bDone;

	cocos2d::Label* m_pLabelUserName;
	cocos2d::Label* m_pLabelScore;
	cocos2d::MenuItemImage* m_pBackDashboardButton;

	bool m_bBackPressed;

	bool m_bActionRunning;

	std::map<int, LmGameComponent*> m_aIdTable;

	int m_iNumberOfGameComponent;

	bool m_bDashboardIsHidden;

	//listener
	cocos2d::EventListenerTouchOneByOne* m_pListener;

	//METHODS

	//callback
	void nextSetPointLayer(cocos2d::Ref*);
	void previousSetPointLayer(cocos2d::Ref*);

	//interface for all games where we init our games and so on
	virtual void runGame()=0;

	//init the game
	virtual bool initGame()=0;

	//add the dashboard layer
	void initDashboardLayer();

	void moveDashboardLayer();
	void moveRightDone();
	void moveLeftDone();

	void backToDashboard(cocos2d::Ref* );

	//register the gamecomponent and return a pointer to it
	LmGameComponent* makeGameComponent();

	//call when the finishgamebutton is pressed
	void endGame();

	//when it's a win
	void win(bool);

	//callback event wifi
	void onWinEvent(bytes );


	void playCallback(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);

	void initFinishButtonTexture();

	void initNextPreviousButton();

	void finishInteraction();

	//to remove from the menu the next and previous menuitem
	void removeNextPreviousMenuItem();

	//check id there is a sound to display or not the play checkbox
	void checkIfDisplayPlayCheckBox(LmSetPoint*);





};

#endif // LMINTERACTION_H__
