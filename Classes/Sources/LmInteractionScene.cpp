//use for the debug
#define COCOS2D_DEBUG 1

#include "../Include/LmInteractionScene.h"
//to avoid double inclusion and to use static method makeuseravatar
#include "../Include/LmGameManager.h"

#include <algorithm>    // std::max
#include <stdlib.h>     /* abs */

USING_NS_CC;

int LmInteractionScene::s_iNumberOfInteraction = 0;

LmInteractionScene::LmInteractionScene()
{

	//object ludomuse
	m_pLmSetPointBegin = new LmSetPoint; //need to be delete
	m_pLmSetPointEnd = new LmSetPoint; //need to be delete
	m_pLmReward = nullptr;
	m_pLmRewardUser2 = nullptr;

	//primitive type
	m_bDone = false;
	m_bActionRunning = false;
	m_bDashboardIsHidden = true;
	m_bMoveDone = true;
	m_bBackPressed = false;
	m_iNumberOfGameComponent = 0;
	m_bFinishGameButtonSync = true;
	m_bReplayButtonSync = true;
	m_bSetPointBegin = true;
	m_bSetPointFinished = false;
	m_bWin = false;
	m_bTouchBeganDisabled = false;
	m_bUser1IsReadyForNextGame = false;
	m_bUser2IsReadyForNextGame = false;
	m_bGameIsRunning = false;
	m_iIdGame = s_iNumberOfInteraction; //interaction scene can know which scene of the game it is (use to send event)
	s_iNumberOfInteraction++;
	m_iSetPointPositionUser1 = 0;
	m_iSetPointPositionUser2 = 0;
	m_bSync = true;

	//pointer
	m_pBackDashboardButton = nullptr;
	m_pUser = nullptr;
	m_pDashboardBandLayer = nullptr;
	m_pMoveLayerButton = nullptr;
	m_pLayerGame = nullptr;
	m_pLabelScore = nullptr;
	m_pLabelUserName = nullptr;
	m_pSpriteDashboardBand = nullptr;
	m_pFinishGameButton = nullptr;
	m_pReplayButton = nullptr;
	m_pNextButton = nullptr;
	m_pPreviousButton = nullptr;
	m_pListener = nullptr;
	m_pSpriteWaitingScreen = nullptr;
	m_pPlayCheckBox = nullptr;
	m_pMenu = nullptr;
	m_pInstruction = nullptr;

}

LmInteractionScene::~LmInteractionScene()
{

	s_iNumberOfInteraction--;

	//if this scene own a reward delete it
	if (m_pLmReward)
	{
		delete m_pLmReward;
	}

	if (m_pLmRewardUser2)
	{
		delete m_pLmRewardUser2;
	}

	//destroy all gamecomponent

	for (it_type it = m_aIdTable.begin(); it != m_aIdTable.end(); ++it)
	{
		//destroy the pointer on the LmGameComponent
		delete it->second;
	}

	delete m_pLmSetPointBegin;
	delete m_pLmSetPointEnd;

	if (m_pListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(
				m_pListener);
	}

	//we retained it because it w&as create into lmjsonparser class
	if (m_pInstruction)
	{
		m_pInstruction->release();
	}

}

bool LmInteractionScene::init(LmUser* l_pUser)
{

	//preload sounds
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(
	FILENAME_BUTTON_CLICKED);

	//init user
	m_pUser = l_pUser;

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	// create menu, it's an autorelease object
	m_pMenu = Menu::create();
	m_pMenu->setPosition(Vec2::ZERO);
	addChild(m_pMenu, 1);

	initNextPreviousButton();

	//play pause checkbox
	m_pPlayCheckBox = ui::CheckBox::create("Ludomuse/Content/pause.png",
			"Ludomuse/Content/play.png");
	m_pPlayCheckBox->setTouchEnabled(true);
	m_pPlayCheckBox->setSwallowTouches(false);
	m_pPlayCheckBox->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.05));
	m_pPlayCheckBox->addEventListener(
			CC_CALLBACK_2(LmInteractionScene::playCallback, this));
	m_pPlayCheckBox->retain();
	addChild(m_pPlayCheckBox, 1);

	checkIfDisplayPlayCheckBox(m_pLmSetPointBegin);

	//create the game layer
	m_pLayerGame = Layer::create();
	m_pLayerGame->retain();

	//finish button default one
	m_pFinishGameButton = ui::Button::create("Ludomuse/Content/fin.png",
			"Ludomuse/Content/finpress.png");
	m_pFinishGameButton->setAnchorPoint(Vec2(1, 0.5));
	m_pFinishGameButton->setPosition(
			Vec2(l_oVisibleSize.width, l_oVisibleSize.height * 0.05));
	m_pFinishGameButton->setTouchEnabled(true);
	m_pFinishGameButton->addTouchEventListener(
			CC_CALLBACK_0(LmInteractionScene::endGame, this));
	m_pFinishGameButton->setVisible(false);
	m_pLayerGame->addChild(m_pFinishGameButton, 2);

	//replay button default one
	m_pReplayButton = ui::Button::create("Ludomuse/Content/rejouer.png",
			"Ludomuse/Content/rejouerpress.png");
	m_pReplayButton->setAnchorPoint(Vec2(0, 0.5));
	m_pReplayButton->setPosition(Vect(0, l_oVisibleSize.height * 0.05));
	m_pReplayButton->setTouchEnabled(true);
	m_pReplayButton->addTouchEventListener(
			CC_CALLBACK_0(LmInteractionScene::resetScene, this));
	m_pReplayButton->setVisible(false);
	m_pLayerGame->addChild(m_pReplayButton, 2);

	//waiting screen to wait his partner before each interaction
	m_pSpriteWaitingScreen = Sprite::create(
			"Ludomuse/Background/creditsbg.png");
	m_pSpriteWaitingScreen->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.5));
	addChild(m_pSpriteWaitingScreen, 1);
	m_pSpriteWaitingScreen->setVisible(false);
	//feedback label init
	auto l_pLabelFeedback = Label::createWithTTF(
			"En attente de ton partenaire ...", "Fonts/JosefinSans-Italic.ttf",
			l_oVisibleSize.height * 0.06);
	l_pLabelFeedback->setPosition(l_oVisibleSize.width * 0.5,
			l_oVisibleSize.height * 0.5);
	l_pLabelFeedback->setColor(Color3B::BLACK);
	m_pSpriteWaitingScreen->addChild(l_pLabelFeedback);

	//add instruction label
	if (m_pInstruction)
	{
		CCLOG("there is instruction");
		m_pInstruction->setVisible(false);
		m_pLayerGame->addChild(m_pInstruction, 2);
	}

	initDashboardLayer();

	if (!m_pLmSetPointBegin->init(this))
	{
		CCLOG("LmSetPoint init failed");
		return false;
	}
	else
	{
		m_pUser->getPLmStatistics()->interactionBegin();

		m_iSetPointPositionUser1 = 0;
		m_iSetPointPositionUser2 = 0;

	}

	return true;
}

void LmInteractionScene::restart()
{
	CCLOG("restart parent");
}

bool LmInteractionScene::startGame()
{
	CCLOG("LmInteractionScene::startGame");

	if (m_bUser1IsReadyForNextGame && m_bUser2IsReadyForNextGame)
	{
		m_bGameIsRunning = true;

		//to get notigy by event
		m_pSpriteWaitingScreen->setVisible(false);

		m_pUser->getPLmStatistics()->interactionGameBegin();

		//set visible instruction
		if (m_pInstruction)
		{
			m_pInstruction->setVisible(true);
		}

		CCLOG("LmInteractionScene::runGame");
		runGame();
		return true;
	}
	else
	{
		return false;
	}

}

void LmInteractionScene::setNextVisible(bool visible)
{

	m_pNextButton->setVisible(visible);

	if(m_bSetPointBegin)
	{
		CCLOG("scene %d is listening", m_iIdGame);

		listenWifiFacade();
	}

}

void LmInteractionScene::playSound()
{
	if (m_bSetPointBegin)
	{
		m_pLmSetPointBegin->playSound();
	}
	else
	{
		m_pLmSetPointEnd->playSound();
	}
}

void LmInteractionScene::onReceivingMsg(bytes l_oMsg)
{
	CCLOG("LmInteractionScene %d _event is %d", m_iIdGame,
			LmWifiObserver::_event);
	switch (LmWifiObserver::_event)
	{
		case LmEvent::Win:
			CCLOG("Win");
			ON_CC_THREAD(LmInteractionScene::onWinEvent, this, l_oMsg)
			;
			break;
		case LmEvent::SetPointPosition:
			CCLOG("SetPointPosition");
			ON_CC_THREAD(LmInteractionScene::onSetPointPositionEvent, this,
					l_oMsg)
			;
			break;
		default:
			break;
	}
}

void LmInteractionScene::initNextPreviousButton()
{
	CCLOG("init button");
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//next button
	m_pNextButton = LmMenuItemImage::create("Suivant",
			CC_CALLBACK_1(LmInteractionScene::nextSetPointLayer, this));
	m_pNextButton->setPosition(
			Vect(
					l_oVisibleSize.width
							- m_pNextButton->getContentSize().width * 0.8,
					l_oVisibleSize.height * 0.05));
	m_pMenu->addChild(m_pNextButton, 1);

	//previous button
	m_pPreviousButton = LmMenuItemImage::create("Precedent",
			CC_CALLBACK_1(LmInteractionScene::previousSetPointLayer, this));
	m_pPreviousButton->setPosition(
			Vect(m_pPreviousButton->getContentSize().width * 0.8,
					l_oVisibleSize.height * 0.05));
	m_pMenu->addChild(m_pPreviousButton, 1);

	if (m_bSync)
	{
		//wait user 2
		m_pNextButton->setVisible(false);
	}
	m_pPreviousButton->setVisible(false);

}

void LmInteractionScene::playCallback(cocos2d::Ref*,
		cocos2d::ui::CheckBox::EventType type)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	FILENAME_BUTTON_CLICKED);

	switch (type)
	{
		case ui::CheckBox::EventType::SELECTED:
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

			break;

		case ui::CheckBox::EventType::UNSELECTED:
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

			break;

		default:
			break;
	}
}

void LmInteractionScene::previousSetPointLayer(cocos2d::Ref* p_Sender)
{

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	FILENAME_BUTTON_CLICKED);

	if (m_bSetPointBegin)
	{
		if (m_pLmSetPointBegin->previousLayer())
		{
			if (m_pLmSetPointBegin->getIIndex() == 0)
			{
				m_pPreviousButton->setVisible(false);
			}

			checkIfDisplayPlayCheckBox(m_pLmSetPointBegin);

		}
	}
	else
	{
		if (m_pLmSetPointEnd->previousLayer())
		{
			if (m_pLmSetPointEnd->getIIndex() == 0)
			{
				m_pPreviousButton->setVisible(false);
			}

			checkIfDisplayPlayCheckBox(m_pLmSetPointEnd);
		}
	}
}

void LmInteractionScene::nextSetPointLayer(cocos2d::Ref* p_Sender)
{

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	FILENAME_BUTTON_CLICKED);

	CCLOG("LmInteractionScene::nextSetPointLayer");
	CCLOG("abs(%d-%d)", m_iSetPointPositionUser1, m_iSetPointPositionUser2);

	if (m_bSetPointBegin)
	{

		setPointBeginNext();

	}
	else
	{
		setPointEndNext();
	}

	if (m_bSync && !(m_iSetPointPositionUser1 - m_iSetPointPositionUser2 <= 1))
	{

		CCLOG("waiting screen");

		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

		//waiting screen appear
		m_pSpriteWaitingScreen->setVisible(true);

		if (m_bSetPointBegin)
		{

			setVisibleSetPointElements(false, m_pLmSetPointBegin);

		}
		else
		{
			setVisibleSetPointElements(false, m_pLmSetPointEnd);
		}

	}

}

void LmInteractionScene::finishInteraction()
{
	//finish the interaction
	CCLOG("popscene");
	Director::getInstance()->popScene();
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
			"InteractionSceneFinished");
}

void LmInteractionScene::initDashboardLayer()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//layer
	m_pDashboardBandLayer = Layer::create();
	this->addChild(m_pDashboardBandLayer, 2);

	//create a menu and back to dashboard menuitemiamhe
	auto l_pMenu = Menu::create();
	l_pMenu->setPosition(Vec2::ZERO);
	m_pDashboardBandLayer->addChild(l_pMenu, 1);

	//spritebackground
	m_pSpriteDashboardBand = LmSprite::create(
			"Ludomuse/Content/dashboardLayerTexture.png");
	m_pSpriteDashboardBand->setAnchorPoint(Vec2(0, 0));
	m_pSpriteDashboardBand->setPosition(Vec2(0, 0));
	m_pDashboardBandLayer->addChild(m_pSpriteDashboardBand, 0);

	//button to move the layer
	m_pMoveLayerButton = MenuItemImage::create(
			"Ludomuse/Content/moveDashboardLayer.png",
			"Ludomuse/Content/moveDashboardLayer.png",
			CC_CALLBACK_1(LmInteractionScene::moveDashboardLayer, this));
	m_pMoveLayerButton->setPosition(
			Vec2(m_pSpriteDashboardBand->getContentSize().width,
					l_oVisibleSize.height * 0.5f));
	l_pMenu->addChild(m_pMoveLayerButton);

	//user1 name
	m_pLabelUserName = Label::createWithTTF(m_pUser->getPUserName(),
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.05);
	m_pLabelUserName->setPosition(
			m_pSpriteDashboardBand->getContentSize().width * (0.5f),
			m_pSpriteDashboardBand->getContentSize().height * 0.85f);
	m_pLabelUserName->setMaxLineWidth(
			m_pSpriteDashboardBand->getContentSize().width);
	m_pSpriteDashboardBand->addChild(m_pLabelUserName);

	//sprite user 1 avatar
	auto l_pAvatarSpriteUser1 = LmGameManager::makeUserAvatarSprite(m_pUser);
	l_pAvatarSpriteUser1->setPosition(
			Vec2(m_pSpriteDashboardBand->getContentSize().width * 0.5,
					m_pSpriteDashboardBand->getContentSize().height * 0.7f));
	m_pSpriteDashboardBand->addChild(l_pAvatarSpriteUser1);

	//label score
	char l_aScoreString[10];
	sprintf(l_aScoreString, "%d pts", m_pUser->getPScore());
	m_pLabelScore = Label::createWithTTF(l_aScoreString,
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pLabelScore->setPosition(
			m_pSpriteDashboardBand->getContentSize().width * (0.5f),
			m_pSpriteDashboardBand->getContentSize().height * 0.55f);
	m_pSpriteDashboardBand->addChild(m_pLabelScore);

	m_pBackDashboardButton = LmMenuItemImage::create("Tableau de Bord",
			CC_CALLBACK_1(LmInteractionScene::backToDashboard, this));
	m_pBackDashboardButton->setPosition(
			Vec2(m_pSpriteDashboardBand->getContentSize().width * (0.5f),
					m_pSpriteDashboardBand->getContentSize().height * 0.3f));
	l_pMenu->addChild(m_pBackDashboardButton);

	//hide it
	auto l_oMoveLeftAction = MoveBy::create(0,
			Vect(
					(-1) * s_fDashboardRatioHidden
							* m_pSpriteDashboardBand->getContentSize().width,
					0));
	m_pDashboardBandLayer->runAction(l_oMoveLeftAction);

	m_pSpriteDashboardBand->setBTouchEnabled(!m_bDashboardIsHidden);

}

void LmInteractionScene::moveDashboardLayer(cocos2d::Ref* p_Sender)
{

	if (m_bMoveDone)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
		FILENAME_BUTTON_CLICKED);

		//stats
		m_pUser->getPLmStatistics()->clicked("Sortie Tableau de Bord");

		m_bMoveDone = false;

		//we look if we have to move right or left
		if (m_bDashboardIsHidden)
		{
			auto l_oMoveRightAction =
					MoveBy::create(s_fTimeMovingDashboard,
							Vect(
									s_fDashboardRatioHidden
											* m_pSpriteDashboardBand->getContentSize().width,
									0));
			auto l_oMoveRightActionDone = CallFunc::create(
					std::bind(&LmInteractionScene::moveRightDone, this));
			m_pDashboardBandLayer->runAction(
					Sequence::create(l_oMoveRightAction, l_oMoveRightActionDone,
							nullptr));
			m_bDashboardIsHidden = false;
			m_pSpriteDashboardBand->setBTouchEnabled(!m_bDashboardIsHidden);
		}
		else
		{
			auto l_oMoveLeftAction =
					MoveBy::create(s_fTimeMovingDashboard,
							Vect(
									(-1) * s_fDashboardRatioHidden
											* m_pSpriteDashboardBand->getContentSize().width,
									0));
			auto l_oMoveLeftActionDone = CallFunc::create(
					std::bind(&LmInteractionScene::moveLeftDone, this));
			m_pDashboardBandLayer->runAction(
					Sequence::create(l_oMoveLeftAction, l_oMoveLeftActionDone,
							nullptr));
			m_bDashboardIsHidden = true;
			m_pSpriteDashboardBand->setBTouchEnabled(!m_bDashboardIsHidden);
		}

	}

}

void LmInteractionScene::moveRightDone()
{
	//enable back button
	m_bBackPressed = false;

	//active button
	m_bMoveDone = true;

}

void LmInteractionScene::moveLeftDone()
{
	//disable back button
	m_bBackPressed = true;

	//active button
	m_bMoveDone = true;

}

void LmInteractionScene::backToDashboard(cocos2d::Ref* p_Sender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	FILENAME_BUTTON_CLICKED);

	//stats
	m_pUser->getPLmStatistics()->clicked("Tableau de Bord");

	if (!m_bBackPressed && m_bMoveDone && m_pLmSetPointBegin->isBActionDone())
	{
		m_bBackPressed = true;

		if (!m_pPlayCheckBox->isSelected())
		{
			//simulate a press on the m_pplaycheckbox
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			m_pPlayCheckBox->setSelected(true);
		}

		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
				"BackToDashboard");
	}

}

void LmInteractionScene::win(bool win)
{

	m_pUser->getPLmStatistics()->interactionGameEnd();

	m_bWin = win;

	//send msg to inform other usert that he wins or not
	bytes msg(10);
	msg << LmEvent::GameFinished;
	msg.write(m_iIdGame);
	msg.write(m_bWin);
	WIFIFACADE->sendBytes(msg);

	//depend of m_bwin TODO try to launch it into cocos thread
	initFinishButtonTexture();

	if (m_pInstruction)
	{
		m_pInstruction->setVisible(false);
	}

	m_pFinishGameButton->setVisible(true);

}

void LmInteractionScene::initFinishButtonTexture()
{

	//if there is a reward we init the button with the appropriate sprite
	if (m_pLmReward && m_bWin)
	{

		CCLOG("init finish button texture");

		//use to place elements
		Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
		Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

		CCLOG("add reward");
		//add reward to the user
		m_pLmReward->init();
		m_pUser->winReward(m_pLmReward);

		CCLOG("init texture");
		//init background button
		m_pFinishGameButton->loadTextures(
				m_pLmReward->getSFilenameSpriteBackground(),
				m_pLmReward->getSFilenameSpriteBackground(),
				m_pLmReward->getSFilenameSpriteBackground());

		m_pFinishGameButton->setAnchorPoint(Vec2(0.5, 0.5));
		m_pFinishGameButton->setPosition(
				Vect(l_oVisibleSize.width * 0.5 + l_oOrigin.x,
						l_oVisibleSize.height * 0.5 + l_oOrigin.y));

		m_pFinishGameButton->setScaleX(1.2);

		//if there is a sprite reward
		if (m_pLmReward->getPSpriteReward())
		{

			CCLOG("on place la sprite de la reward %f width,%f height",
					m_pLmReward->getFSpriteRewardWidthPercent(),
					m_pLmReward->getFSpriteRewardHeightPercent());

			m_pLmReward->getPSpriteReward()->setPosition(
					Vec2(
							l_oVisibleSize.width
									* m_pLmReward->getFSpriteRewardWidthPercent(),
							l_oVisibleSize.height
									* m_pLmReward->getFSpriteRewardHeightPercent()));

			m_pFinishGameButton->addChild(m_pLmReward->getPSpriteReward());
		}

		m_pFinishGameButton->addChild(m_pLmReward->getPLabekReward());

		CCLOG("place label");

		auto l_pLabelInstruction = Label::createWithTTF(
				"(Touchez pour continuez)", "Fonts/JosefinSans-Italic.ttf",
				l_oVisibleSize.width * 0.04);
		l_pLabelInstruction->setAnchorPoint(Vec2(0.5, 0));
		l_pLabelInstruction->setPosition(l_oVisibleSize.width * 0.5, 0);
		l_pLabelInstruction->setColor(m_pLmReward->getOColorText());
		l_pLabelInstruction->setAlignment(TextHAlignment::CENTER);
		m_pFinishGameButton->addChild(l_pLabelInstruction);

		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
				m_pLmReward->getSFilenameSound().c_str(), false);
	}
}

void LmInteractionScene::endGame()
{
	if (m_bFinishGameButtonSync)
	{
		CCLOG("end function");
		m_bFinishGameButtonSync = false;

		//reset sync beetween player
		m_bUser1IsReadyForNextGame = false;
		m_bUser2IsReadyForNextGame = false;

		//the game is finished we can remove the layer of the game
		removeChild(m_pLayerGame);

		//so the next time we print the end setpoint
		m_bSetPointBegin = false;
		//new setpoint
		m_bSetPointFinished = false;

		m_pPlayCheckBox->setVisible(true);
		m_pPlayCheckBox->setSelected(false);

		//its a linear progression so for now it can be release here
		m_pLayerGame->release();
		m_pPlayCheckBox->release();

		m_bGameIsRunning = false;

		//stop sound
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(
				true);

		initNextPreviousButton();

		//init the end set point
		if (!m_pLmSetPointEnd->init(this))
		{
			CCLOG("LmSetPoint init failed");
		}

		//reset
		m_iSetPointPositionUser1 = 0;
		m_iSetPointPositionUser2 = 0;

		checkIfDisplayPlayCheckBox(m_pLmSetPointEnd);

		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
				"GameFinished");
	}
}

void LmInteractionScene::onWinEvent(bytes l_oMsg)
{
	bool buffer = l_oMsg.readBool();

	win(buffer);
}

void LmInteractionScene::onSetPointPositionEvent(bytes l_oMsg)
{
	m_iSetPointPositionUser2 = l_oMsg.readInt();

	CCLOG("abs(%d-%d)", m_iSetPointPositionUser1, m_iSetPointPositionUser2);

	//set viisible if there less than 1 layer distance and user 1 is not waiting to play
	if (!m_bUser1IsReadyForNextGame)
	{
		if (abs(m_iSetPointPositionUser1 - m_iSetPointPositionUser2) < 2)
		{
			CCLOG("setvisible layer");

			//waiting screen appear
			m_pSpriteWaitingScreen->setVisible(false);

			CCLOG("resume sound");

			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

			CCLOG("restore gui element visibility");

			//restore gui elements
			if (m_bSetPointBegin)
			{

				setVisibleSetPointElements(true, m_pLmSetPointBegin);

			}
			else
			{
				setVisibleSetPointElements(true, m_pLmSetPointEnd);
			}

		}
	}

}

LmGameComponent* LmInteractionScene::makeGameComponent()
{
	//generate a new id
	m_iNumberOfGameComponent++;
	//register it into the id table
	m_aIdTable.insert(
			std::pair<int, LmGameComponent*>(m_iNumberOfGameComponent,
					new LmGameComponent(m_iNumberOfGameComponent)));

	return m_aIdTable.find(m_iNumberOfGameComponent)->second;
}

void LmInteractionScene::removeMenuItem()
{
	m_pMenu->removeAllChildrenWithCleanup(true);
	m_pPlayCheckBox->setVisible(false);

}

void LmInteractionScene::checkIfDisplayPlayCheckBox(LmSetPoint* l_pSetPoint)
{

	if (l_pSetPoint->getNextLayerDisplay()->getSSoundUrl().compare("") == 0)
	{

		m_pPlayCheckBox->setVisible(false);
	}
	else
	{

		m_pPlayCheckBox->setVisible(true);
	}
}

void LmInteractionScene::updateScoreLabel()
{
	//update score user1
	char l_aScoreString[10];
	sprintf(l_aScoreString, "%d pts", m_pUser->getPScore());
	m_pLabelScore->setString(l_aScoreString);
}

void LmInteractionScene::updatePosition(int l_iSetPointIndex)
{
	int buffer = m_iSetPointPositionUser1;

	//the new position is the max
	m_iSetPointPositionUser1 = std::max(m_iSetPointPositionUser1,
			l_iSetPointIndex);

	if (buffer != m_iSetPointPositionUser1 && m_bSync)
	{
		//update user 2 position on the other tab
		bytes msg(20);
		msg << LmEvent::SetPointPosition;
		msg.write(m_iSetPointPositionUser1);
		WIFIFACADE->sendBytes(msg);

		CCLOG("new position = %d", m_iSetPointPositionUser1);
	}

}

void LmInteractionScene::setPointBeginNext()
{
	//false => setpoint begin is finished
	if (!m_pLmSetPointBegin->nextLayer() && m_pLmSetPointBegin->isBActionDone()
			&& !m_bSetPointFinished)
	{
		m_bSetPointFinished = true;
		ON_CC_THREAD(LmInteractionScene::removeMenuItem, this);

		//we are ready
		m_bUser1IsReadyForNextGame = true;

		//send the msg to indicate user 2 we are ready
		bytes msg(10);
		msg << LmEvent::ReadyForNextGame;
		msg.write(m_iIdGame);
		WIFIFACADE->sendBytes(msg);

		//add the layer of the game
		this->addChild(m_pLayerGame, 0);

		//stop sound
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(
				true);

		//if we can't start game because we are waiting for user 2
		if (!startGame())
		{
			m_pSpriteWaitingScreen->setVisible(true);
		}

	}
	else
	{
		//go to the next layer
		if (m_pLmSetPointBegin->getIIndex() > 0 && !m_bSetPointFinished)
		{
			m_pPreviousButton->setVisible(true);
		}

		checkIfDisplayPlayCheckBox(m_pLmSetPointBegin);

		m_pPlayCheckBox->setSelected(false);

		updatePosition(m_pLmSetPointBegin->getIIndex());
	}
}

void LmInteractionScene::setPointEndNext()
{
	//false => setpoint end is finished
	if (!m_pLmSetPointEnd->nextLayer() && m_pLmSetPointEnd->isBActionDone()
			&& !m_bSetPointFinished)
	{
		m_bSetPointFinished = true;
		m_pPlayCheckBox->setVisible(false);
		ON_CC_THREAD(LmInteractionScene::removeMenuItem, this);

		//stop sound
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(
				true);

		//to permit the user2 to update his dashboard

		bytes msg(10);
		msg << LmEvent::InteractionDone;
		WIFIFACADE->sendBytes(msg);

		m_pUser->getPLmStatistics()->interactionEnd();
		m_pUser->getPLmStatistics()->endRecord(m_iIdGame, m_sDescription);

		CCLOG("%d stop listen to wifi facade", m_iIdGame);
		//stop send event async till the next inetraction
		stopListenWifiFacade();

		//test to see if we need to display event in cocos thread
		ON_CC_THREAD(LmInteractionScene::finishInteraction, this);

	}
	else
	{
		if (m_pLmSetPointEnd->getIIndex() > 0 && !m_bSetPointFinished)
		{
			m_pPreviousButton->setVisible(true);
		}

		checkIfDisplayPlayCheckBox(m_pLmSetPointEnd);

		//uncheck play button
		m_pPlayCheckBox->setSelected(false);

		updatePosition(m_pLmSetPointEnd->getIIndex());

	}
}

void LmInteractionScene::setVisibleSetPointElements(bool visible,
		LmSetPoint* setpoint)
{
	CCLOG("next button visible or not?");

	if (!m_bSetPointFinished)
	{
		m_pNextButton->setVisible(visible);

		CCLOG("previous button visible or not?");
		if (setpoint->getIIndex() > 0)
		{
			m_pPreviousButton->setVisible(visible);
		}

	}

	CCLOG("playcheckbox  visible or not?");
	if (visible)
	{
		checkIfDisplayPlayCheckBox(setpoint);

	}
	else
	{
		m_pPlayCheckBox->setVisible(visible);

	}
}
