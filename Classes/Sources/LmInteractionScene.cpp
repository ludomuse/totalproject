//use for the debug
#define COCOS2D_DEBUG 1

#include "../Include/LmInteractionScene.h"
//to avoid double inclusion and to use static method makeuseravatar
#include "../Include/LmGameManager.h"

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
	m_bUser1IsReadyForNextInteraction = false;
	m_bUser2IsReadyForNextInteraction = false;
	m_bGameIsRunning = false;
	m_iIdGame = s_iNumberOfInteraction; //interaction scene can know which scene of the game it is (use to send event)
	s_iNumberOfInteraction++;

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

}

LmInteractionScene::~LmInteractionScene()
{
	s_iNumberOfInteraction--;

	//if this scene own a reward delete it
	if (m_pLmReward)
	{
		delete m_pLmReward;
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

}

bool LmInteractionScene::init(LmUser* l_pUser)
{

	//preload sounds
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(
			LmGameManager::s_sFilenameButtonClicked);

	//init user
	m_pUser = l_pUser;

	initDashboardLayer();
	if (!m_pLmSetPointBegin->init(this))
	{
		CCLOG("LmSetPoint init failed");
		return false;
	}

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	// create menu, it's an autorelease object
	m_pMenu = Menu::create();
	m_pMenu->setPosition(Vec2::ZERO);
	addChild(m_pMenu, 1);

	initNextPreviousButton();

	//play pause checkbox
	m_pPlayCheckBox = ui::CheckBox::create("Ludomuse/GUIElements/pause.png",
			"Ludomuse/GUIElements/play.png");
	m_pPlayCheckBox->setTouchEnabled(true);
	m_pPlayCheckBox->setSwallowTouches(false);
	m_pPlayCheckBox->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.1));
	m_pPlayCheckBox->addEventListener(
			CC_CALLBACK_2(LmInteractionScene::playCallback, this));
	m_pPlayCheckBox->retain();
	addChild(m_pPlayCheckBox, 1);

	checkIfDisplayPlayCheckBox(m_pLmSetPointBegin);

	//create the game layer
	m_pLayerGame = Layer::create();
	m_pLayerGame->retain();

	//finish button default one
	m_pFinishGameButton = ui::Button::create("Ludomuse/GUIElements/fin.png",
			"Ludomuse/GUIElements/finpress.png");
	m_pFinishGameButton->setAnchorPoint(Vec2(1, 0.5));
	m_pFinishGameButton->setPosition(
			Vec2(l_oVisibleSize.width, l_oVisibleSize.height * 0.1));
	m_pFinishGameButton->setTouchEnabled(true);
	m_pFinishGameButton->addTouchEventListener(
			CC_CALLBACK_0(LmInteractionScene::endGame, this));
	m_pFinishGameButton->setVisible(false);
	m_pLayerGame->addChild(m_pFinishGameButton, 2);

	//replay button default one
	m_pReplayButton = ui::Button::create("Ludomuse/GUIElements/rejouer.png",
			"Ludomuse/GUIElements/rejouerpress.png");
	m_pReplayButton->setAnchorPoint(Vec2(0, 0.5));
	m_pReplayButton->setPosition(Vect(0, l_oVisibleSize.height * 0.2));
	m_pReplayButton->setTouchEnabled(true);
	m_pReplayButton->addTouchEventListener(
			CC_CALLBACK_0(LmInteractionScene::resetScene, this));
	m_pReplayButton->setVisible(false);
	m_pLayerGame->addChild(m_pReplayButton, 2);

	//waiting screen to wait his partner before each interaction
	m_pSpriteWaitingScreen = Sprite::create("Ludomuse/Background/splash.png");
	m_pSpriteWaitingScreen->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.5));
	m_pLayerGame->addChild(m_pSpriteWaitingScreen);
	m_pSpriteWaitingScreen->setVisible(false);

	return true;
}

void LmInteractionScene::restart()
{
	CCLOG("restart parent");
}

bool LmInteractionScene::startGame()
{
	CCLOG("LmInteractionScene::startGame");

	if (m_bUser1IsReadyForNextInteraction && m_bUser2IsReadyForNextInteraction)
	{
		m_bGameIsRunning = true;

		//to get notigy by event
		m_pSpriteWaitingScreen->setVisible(false);
		listenWifiFacade();

		CCLOG("LmInteractionScene::runGame");
		runGame();
		return true;
	}
	else
	{
		return false;
	}

}

void LmInteractionScene::onReceivingMsg(bytes l_oMsg)
{
	CCLOG("LmInteractionScene _event is %d", LmWifiObserver::_event);
	switch (LmWifiObserver::_event)
	{
	case LmEvent::Win:
		CCLOG("Win");
		ON_CC_THREAD(LmInteractionScene::onWinEvent, this, l_oMsg)
		;
		break;
	default:
		break;
	}
}

void LmInteractionScene::notifyPictureIsTaken()
{
	CCLOG("Picture is saved in %s", CURR_PHOTO_PATH.c_str());
}

void LmInteractionScene::initNextPreviousButton()
{
	CCLOG("init button");
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//next button
	m_pNextButton = MenuItemImage::create(
			"Ludomuse/GUIElements/nextButtonNormal.png",
			"Ludomuse/GUIElements/nextButtonPressed.png",
			CC_CALLBACK_1(LmInteractionScene::nextSetPointLayer, this));
	m_pNextButton->setPosition(
			Vect(
					l_oVisibleSize.width
							- m_pNextButton->getContentSize().width * 0.8,
					l_oVisibleSize.height * 0.1));
	m_pMenu->addChild(m_pNextButton, 1);

	//previous button
	m_pPreviousButton = MenuItemImage::create(
			"Ludomuse/GUIElements/previousButtonNormal.png",
			"Ludomuse/GUIElements/previousButtonPressed.png",
			CC_CALLBACK_1(LmInteractionScene::previousSetPointLayer, this));
	m_pPreviousButton->setPosition(
			Vect(m_pPreviousButton->getContentSize().width * 0.8,
					l_oVisibleSize.height * 0.1));
	m_pMenu->addChild(m_pPreviousButton, 1);

	m_pNextButton->setVisible(true);
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

	if (m_bSetPointBegin)
	{
		//false => setpoint begin is finished
		if (!m_pLmSetPointBegin->nextLayer()
				&& m_pLmSetPointBegin->isBActionDone() && !m_bSetPointFinished)
		{
			m_bSetPointFinished = true;
			ON_CC_THREAD(LmInteractionScene::removeNextPreviousMenuItem, this);
			m_pPlayCheckBox->setVisible(false);

			//we are ready
			m_bUser1IsReadyForNextInteraction = true;

			//send the msg to indicate user 2 we are ready
			bytes msg(10);
			msg << LmEvent::ReadyForNextInteraction;
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
			if (m_pLmSetPointBegin->getIIndex() > 0 && !m_bSetPointFinished)
			{
				m_pPreviousButton->setVisible(true);
			}

			checkIfDisplayPlayCheckBox(m_pLmSetPointBegin);

			m_pPlayCheckBox->setSelected(false);
		}

	}
	else
	{
		//false => setpoint end is finished
		if (!m_pLmSetPointEnd->nextLayer() && m_pLmSetPointEnd->isBActionDone()
				&& !m_bSetPointFinished)
		{
			m_bSetPointFinished = true;
			m_pPlayCheckBox->setVisible(false);
			ON_CC_THREAD(LmInteractionScene::removeNextPreviousMenuItem, this);

			//stop sound
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(
					true);

			//to permit the user2 to update his dashboard
			if (m_bWin)
			{
				//send msg to inform other usert that he wins
				bytes msg(10);
				msg << LmEvent::InteractionDone;
				msg.write(m_iIdGame);
				msg.write(true);
				WIFIFACADE->sendBytes(msg);

			}
			else
			{
				//send msg to inform other usert that he wins
				bytes msg(10);
				msg << LmEvent::InteractionDone;
				msg.write(m_iIdGame);
				msg.write(false);
				WIFIFACADE->sendBytes(msg);
			}

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

	//spritebackground
	m_pSpriteDashboardBand = LmSprite::create(
			"Ludomuse/GUIElements/dashboardLayerTexture.png");
	m_pSpriteDashboardBand->setAnchorPoint(Vec2(0, 0));
	m_pSpriteDashboardBand->setPosition(Vec2(0, 0));
	m_pDashboardBandLayer->addChild(m_pSpriteDashboardBand, 0);

	//button to move the layer
	m_pMoveLayerButton = ui::Button::create(
			"Ludomuse/GUIElements/moveDashboardLayer.png");
	m_pMoveLayerButton->setTouchEnabled(true);
	m_pMoveLayerButton->setPosition(
			Vec2(m_pSpriteDashboardBand->getContentSize().width,
					l_oVisibleSize.height * 0.5f));
	m_pMoveLayerButton->addTouchEventListener(
			CC_CALLBACK_0(LmInteractionScene::moveDashboardLayer, this));
	m_pDashboardBandLayer->addChild(m_pMoveLayerButton, 0);

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

	//create a menu and back to dashboard menuitemiamhe
	auto l_pMenu = Menu::create();
	l_pMenu->setPosition(Vec2::ZERO);
	m_pDashboardBandLayer->addChild(l_pMenu, 1);

	m_pBackDashboardButton = MenuItemImage::create(
			"Ludomuse/GUIElements/backToDashboard.png",
			"Ludomuse/GUIElements/backToDashboardpressed.png",
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

void LmInteractionScene::moveDashboardLayer()
{
	if (m_bMoveDone)
	{
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

		CCLOG("back to dashboard");
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
				"BackToDashboard");
	}
}

void LmInteractionScene::win(bool win)
{

	m_bWin = win;

	//depend of m_bwin
	initFinishButtonTexture();

	if (m_bWin)
	{
		m_pLmReward->playRewardSound();
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

		//init sprite reward
		m_pLmReward->init();
		//init background button
		m_pFinishGameButton->loadTextures(
				m_pLmReward->getSFilenameSpriteBackground(),
				m_pLmReward->getSFilenameSpriteBackground(),
				m_pLmReward->getSFilenameSpriteBackground());

		m_pFinishGameButton->setAnchorPoint(Vec2(0.5, 0.5));
		m_pFinishGameButton->setPosition(
				Vect(l_oVisibleSize.width * 0.5 + l_oOrigin.x,
						l_oVisibleSize.height * 0.5 + l_oOrigin.y));

		//if there is a sprite reward
		if (m_pLmReward->getPSpriteReward())
		{
			//add the sprite to the button
			m_pLmReward->getPSpriteReward()->setPosition(
					Vec2(l_oVisibleSize.width * 0.5 + l_oOrigin.x,
							l_oVisibleSize.height * 0.5 + l_oOrigin.y));

			m_pFinishGameButton->addChild(m_pLmReward->getPSpriteReward());
		}

		m_pFinishGameButton->addChild(m_pLmReward->getPLabekReward());

	}
}

void LmInteractionScene::endGame()
{
	if (m_bFinishGameButtonSync)
	{
		CCLOG("end function");
		m_bFinishGameButtonSync = false;

		//reset sync beetween player
		m_bUser1IsReadyForNextInteraction = false;
		m_bUser2IsReadyForNextInteraction = false;

		//stop send event async till the next inetraction
		stopListenWifiFacade();

		if (m_pLmReward && m_bWin)
		{
			//add reward to the user
			m_pUser->winReward(m_pLmReward);
		}

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

		checkIfDisplayPlayCheckBox(m_pLmSetPointEnd);

	}
}

void LmInteractionScene::onWinEvent(bytes l_oMsg)
{
	bool buffer = l_oMsg.readBool();

	win(buffer);
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

void LmInteractionScene::removeNextPreviousMenuItem()
{
	m_pMenu->removeAllChildrenWithCleanup(true);

}

void LmInteractionScene::checkIfDisplayPlayCheckBox(LmSetPoint* l_pSetPoint)
{


	if(l_pSetPoint->getNextLayerDisplay()->getSSoundUrl().compare("")==0)
	{

		m_pPlayCheckBox->setVisible(false);
	}
	else
	{

		m_pPlayCheckBox->setVisible(true);
	}
}


