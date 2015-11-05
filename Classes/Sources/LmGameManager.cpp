//use for the debug
#define COCOS2D_DEBUG 1

#include "../Include/LmGameManager.h"

#include "../Include/LmHelpers.h"

USING_NS_CC;

LmGameManager::LmGameManager()
{

	listenWifiFacade();

	//object
	m_pLmServerManager = new LmServerManager; //need to be delete
	m_pLmSettings = new LmSettings;

	//primitive type
	m_iIndexInteractionScene = 0;
	m_iInteractionDone = 0;
	m_iInteractionDoneUser2 = 0;
	m_bBackToDashboard = false;
	m_bActionIsDone = true;

	//pointer
	m_pUser1 = nullptr; //need to be delete
	m_pUser2 = nullptr; //need to be delete
	m_pLabelInteractionDone = nullptr;
	m_pSpriteBackgroundBlue = nullptr;
	m_pSpriteBackgroundBlueProfile = nullptr;
	m_pSpriteBackgroundPink = nullptr;
	m_pSpriteBackgroundPinkProfile = nullptr;
	m_pSpriteBandTop = nullptr;
	m_pGameManagerScene = nullptr;
	m_pBackButton = nullptr;
	m_pBackgroundLayer = nullptr;
	m_pCompareButton == nullptr;
	m_pLabelCompareButton = nullptr;
	m_pLabelScore = nullptr;
	m_pLabelTitleApplication = nullptr;
	m_pLabelUser1Name = nullptr;
	m_pCompareButton = nullptr;
	m_pScrollView = nullptr;
	m_pPlayNextInteractionButton = nullptr;
	m_pPinkLayer = nullptr;
	m_pSpriteBandMid = nullptr;
	m_pListener = nullptr;
	m_pSplashSreenLayer = nullptr;
	m_pStarUser1Sprite = nullptr;
	m_pCheckSpriteUser1 = nullptr;
	m_pAvatarSpriteUser2 = nullptr;
	m_pLabelScoreUser2 = nullptr;
	m_pDescriptionBox = nullptr;
	m_pDescriptionLabel = nullptr;
	m_pLabelInteractionDoneUser2 = nullptr;
	m_pLayerScrollView = nullptr;
	m_pStarUser1Sprite = nullptr;
	m_pStarUser2Sprite = nullptr;
	m_pCheckSpriteUser1 = nullptr;
	m_pCheckSpriteUser2 = nullptr;
	m_pSettingsButton = nullptr;

}

LmGameManager::~LmGameManager()
{
	delete m_pLmSettings;

	//delete users created by LmMenu
	delete m_pUser1;
	delete m_pUser2;

	delete m_pLmServerManager;

	//Browse the vector of scene and delete them
	for (std::vector<LmInteractionScene*>::iterator it =
			m_aInteractionSceneOfTheGame.begin();
			it != m_aInteractionSceneOfTheGame.end(); ++it)
	{
		delete (*it);
	}

	//remove the custom event
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(
			"InteractionSceneFinished");
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(
			"BackToDashboard");

}

void LmGameManager::runGames()
{
	//Init of the GameManager
	init();

	//replace the menu scene with Dashboard scene
	Director::getInstance()->replaceScene(
			TransitionFade::create(s_fTimeBetweenLmLayer, m_pGameManagerScene));
}

bool LmGameManager::init()
{

	//preload sounds
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(
			LmGameManager::s_sFilenameButtonClicked);

	if (!m_pLmServerManager->init())
	{
		CCLOG("Initialization Server Manager failed");
		return false;
	}

	//init splashscreen
	if (!initSplashScreen())
	{
		CCLOG("Initialization splash screen failed");
		return false;
	}

	//get the vector of scene through the serverManager and indicate if it's for child or parent
	m_aInteractionSceneOfTheGame =
			m_pLmServerManager->getInteractionSceneOfTheGame(
					m_pUser1->isBParent());

	//init callback method of the custom event (use to know when an interactionScene want to communicate with this)
	auto InteractionSceneFinished = [=](EventCustom * event)
	{

		CCLOG("onInteractionSceneFinishedEvent");

		//check if t's done and win etc and update sprite
			if(m_aInteractionSceneOfTheGame.at(m_iIndexInteractionScene)->getPLmReward())
			{
				if(m_aInteractionSceneOfTheGame.at(m_iIndexInteractionScene)->isBWin())
				{
					m_aSpritesInteractions.at(m_iIndexInteractionScene)->setTexture("Ludomuse/GUIElements/interactionDoneRewardDone.png");
				}
				else
				{
					m_aSpritesInteractions.at(m_iIndexInteractionScene)->setTexture("Ludomuse/GUIElements/interactionDoneRewardNotDone.png");
				}
			}
			else
			{
				m_aSpritesInteractions.at(m_iIndexInteractionScene)->setTexture("Ludomuse/GUIElements/interactionDoneNoReward.png");
			}

			m_iInteractionDone++;

			//update index
			m_iIndexInteractionScene++;

			//update label of dashboard
			updateDashboard();

			//it's not a back to dashboard
			m_bBackToDashboard=false;

			//reset touch enable
			inputDisabled(false);

		};

	auto BackToDashboard = [=](EventCustom * event)
	{

		CCLOG("onBackToDashboardEvent");

		Director::getInstance()->popScene();

		m_bBackToDashboard=true;

		//reset touch enable
			inputDisabled(false);

		};

	//add the custom event to the event dispatcher
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(
			"InteractionSceneFinished", InteractionSceneFinished);
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(
			"BackToDashboard", BackToDashboard);

	return true;
}

bool LmGameManager::initSplashScreen()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//create the scene
	m_pGameManagerScene = Scene::create();

	//add the layer of the splash screen
	m_pSplashSreenLayer = Layer::create();
	m_pGameManagerScene->addChild(m_pSplashSreenLayer);

	//add the sprite background get from the config json file
	auto l_pSplashScreenSprite = Sprite::create(
			m_pLmServerManager->getSFilenameSpriteSplashScreen());
	l_pSplashScreenSprite->setPosition(
			l_oVisibleSize.width * 0.5f + l_oOrigin.x,
			l_oVisibleSize.height * 0.5f + l_oOrigin.y);
	m_pSplashSreenLayer->addChild(l_pSplashScreenSprite);

	//init its listener
	m_pListener = EventListenerTouchOneByOne::create();
	m_pListener->onTouchBegan = CC_CALLBACK_2(
			LmGameManager::onTouchBeganSplashScreen, this);
	m_pListener->setSwallowTouches(true);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
			m_pListener, m_pSplashSreenLayer);

	return true;
}

bool LmGameManager::onTouchBeganSplashScreen(Touch* touch, Event* event)
{

	//remove layer
	m_pGameManagerScene->removeChild(m_pSplashSreenLayer);
	//remove listener
	Director::getInstance()->getEventDispatcher()->removeEventListener(
			m_pListener);

	//init dashboard
	if (!initDashboard())
	{
		CCLOG("Initialization DashBoard failed");
		return false;
	}

	return true;
}

bool LmGameManager::initDashboard()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();
	Size l_oWinSize = Director::getInstance()->getWinSize();

	//create the main layer and put element on it
	m_pBackgroundLayer = Layer::create();
	m_pPinkLayer = Layer::create();
	m_pGameManagerScene->addChild(m_pBackgroundLayer);
	m_pGameManagerScene->addChild(m_pPinkLayer);

	//we add the different background with different zorder

	//background blue
	m_pSpriteBackgroundBlue = Sprite::create(
			"Ludomuse/Background/dashboard.png");
	m_pSpriteBackgroundBlue->setPosition(l_oVisibleSize.width * 0.5f,
			l_oVisibleSize.height * 0.5f);
	m_pBackgroundLayer->addChild(m_pSpriteBackgroundBlue);

	//background pink
	m_pSpriteBackgroundPink = Sprite::create(
			"Ludomuse/GUIElements/compareNormal.png");
	m_pSpriteBackgroundPink->setPosition(l_oVisibleSize.width * 0.5f,
			m_pSpriteBackgroundPink->getContentSize().height
					* (-0.5f + s_fMagingRatioOfSpriteBackgroundUser2Profile));
	m_pPinkLayer->addChild(m_pSpriteBackgroundPink);

	/*
	 * INIT USER 1
	 *
	 */

	//background profile blue
	m_pSpriteBackgroundBlueProfile = Sprite::create(
			"Ludomuse/GUIElements/spriteBackgroundUser1Profile.png");
	m_pSpriteBackgroundBlueProfile->setPosition(
			m_pSpriteBackgroundBlueProfile->getContentSize().width * 0.5f,
			l_oVisibleSize.height * 0.5);
	m_pBackgroundLayer->addChild(m_pSpriteBackgroundBlueProfile, 1);

	auto l_fMidOfSpriteBackgroundUsers =
			m_pSpriteBackgroundBlueProfile->getContentSize().width * 0.5f;
	auto l_fHeightSpriteBackgroundUser1 =
			m_pSpriteBackgroundBlueProfile->getContentSize().height;

	//user1 name
	m_pLabelUser1Name = Label::createWithTTF(m_pUser1->getPUserName(),
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.05);
	m_pLabelUser1Name->setPosition(l_fMidOfSpriteBackgroundUsers,
			l_fHeightSpriteBackgroundUser1 * 0.95f);
	m_pLabelUser1Name->setMaxLineWidth(
			m_pSpriteBackgroundBlueProfile->getContentSize().width);
	m_pSpriteBackgroundBlueProfile->addChild(m_pLabelUser1Name, 1);

	//sprite user 1 avatar
	auto l_pAvatarSpriteUser1 = makeUserAvatarSprite(m_pUser1);
	l_pAvatarSpriteUser1->setPosition(
			Vec2(l_fMidOfSpriteBackgroundUsers,
					l_fHeightSpriteBackgroundUser1 * 0.8));
	m_pSpriteBackgroundBlueProfile->addChild(l_pAvatarSpriteUser1, 1);

	//label score user 1
	char l_aScoreString[20];
	sprintf(l_aScoreString, "%d pts", m_pUser1->getPScore());
	m_pLabelScore = Label::createWithTTF(l_aScoreString,
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pLabelScore->setPosition(l_fMidOfSpriteBackgroundUsers,
			l_fHeightSpriteBackgroundUser1 * 0.65f);
	m_pSpriteBackgroundBlueProfile->addChild(m_pLabelScore, 1);

	//star user1 score
	m_pStarUser1Sprite = addSpriteToLabel(m_pLabelScore,
			"Ludomuse/GUIElements/stargreen.png");
	m_pSpriteBackgroundBlueProfile->addChild(m_pStarUser1Sprite);

	//how manny interaction done label user 1
	char l_aInteractionDoneString[20];
	sprintf(l_aInteractionDoneString, "%d/%d", m_iInteractionDone,
			m_aInteractionSceneOfTheGame.size());
	m_pLabelInteractionDone = Label::createWithTTF(l_aInteractionDoneString,
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pLabelInteractionDone->setPosition(l_fMidOfSpriteBackgroundUsers,
			l_fHeightSpriteBackgroundUser1 * 0.55f);
	m_pSpriteBackgroundBlueProfile->addChild(m_pLabelInteractionDone, 1);

	//sprite associated to interaction label user1
	m_pCheckSpriteUser1 = addSpriteToLabel(m_pLabelInteractionDone,
			"Ludomuse/GUIElements/checkgreen.png");
	m_pSpriteBackgroundBlueProfile->addChild(m_pCheckSpriteUser1);

	/*
	 * INIT USER 2
	 *
	 */

	//background profil pink
	m_pSpriteBackgroundPinkProfile = Sprite::create(
			"Ludomuse/GUIElements/spriteBackgroundUser2Profile.png");
	m_pSpriteBackgroundPinkProfile->setPosition(
			m_pSpriteBackgroundPinkProfile->getContentSize().width * 0.5f,
			m_pSpriteBackgroundPinkProfile->getContentSize().height
					* (-0.5f + s_fMagingRatioOfSpriteBackgroundUser2Profile));
	m_pPinkLayer->addChild(m_pSpriteBackgroundPinkProfile, 1);

	auto l_fHeightSpriteBackgroundUser2 =
			m_pSpriteBackgroundPinkProfile->getContentSize().height;

	//user2 name
	auto l_pLabelUser2Name = Label::createWithTTF(m_pUser2->getPUserName(),
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.05);
	l_pLabelUser2Name->setPosition(l_fMidOfSpriteBackgroundUsers,
			l_fHeightSpriteBackgroundUser2 * 0.9f);
	l_pLabelUser2Name->setMaxLineWidth(
			m_pSpriteBackgroundBlueProfile->getContentSize().width);
	m_pSpriteBackgroundPinkProfile->addChild(l_pLabelUser2Name, 1);

	//sprite user 2 avatar
	m_pAvatarSpriteUser2 = makeUserAvatarSprite(m_pUser2);
	m_pAvatarSpriteUser2->setPosition(
			Vec2(l_fMidOfSpriteBackgroundUsers,
					l_fHeightSpriteBackgroundUser2 * 0.6));
	m_pAvatarSpriteUser2->setVisible(false);
	m_pSpriteBackgroundPinkProfile->addChild(m_pAvatarSpriteUser2, 1);

	//label score user 2
	sprintf(l_aScoreString, "%d pts", m_pUser2->getPScore());
	m_pLabelScoreUser2 = Label::createWithTTF(l_aScoreString,
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pLabelScoreUser2->setPosition(l_fMidOfSpriteBackgroundUsers,
			l_fHeightSpriteBackgroundUser2 * 0.7f);
	m_pSpriteBackgroundPinkProfile->addChild(m_pLabelScoreUser2, 1);

	//star user2 score
	m_pStarUser2Sprite = addSpriteToLabel(m_pLabelScoreUser2,
			"Ludomuse/GUIElements/redstar.png");
	m_pSpriteBackgroundPinkProfile->addChild(m_pStarUser2Sprite);

	//how manny interaction done label user 2
	sprintf(l_aInteractionDoneString, "%d/%d", m_iInteractionDone,
			m_aInteractionSceneOfTheGame.size());
	m_pLabelInteractionDoneUser2 = Label::createWithTTF(
			l_aInteractionDoneString, "Fonts/JosefinSans-Regular.ttf",
			l_oVisibleSize.width * 0.04);
	m_pLabelInteractionDoneUser2->setPosition(l_fMidOfSpriteBackgroundUsers,
			l_fHeightSpriteBackgroundUser2 * 0.1f);
	m_pSpriteBackgroundPinkProfile->addChild(m_pLabelInteractionDoneUser2, 1);

	//sprite associated to interaction label user2
	m_pCheckSpriteUser2 = addSpriteToLabel(m_pLabelInteractionDoneUser2,
			"Ludomuse/GUIElements/checkred.png");
	m_pSpriteBackgroundPinkProfile->addChild(m_pCheckSpriteUser2);

	m_pCheckSpriteUser2->setVisible(false);
	m_pLabelInteractionDoneUser2->setVisible(false);

	/*
	 * Others gui elements
	 */

	//compare button with ui::Button
	m_pCompareButton = ui::Button::create(
			"Ludomuse/GUIElements/compareNormal1.png");
	m_pCompareButton->setTouchEnabled(true);
	m_pCompareButton->setPosition(
			Vect(l_oVisibleSize.width * 0.5f, l_oVisibleSize.height * 0.1f));
	m_pCompareButton->addTouchEventListener(
			CC_CALLBACK_0(LmGameManager::compare, this));
	m_pPinkLayer->addChild(m_pCompareButton);

	//label compare button
	m_pLabelCompareButton = Label::createWithTTF("comparer",
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pLabelCompareButton->setPosition(
			l_oVisibleSize.width * 0.6f
					+ m_pCompareButton->getContentSize().width,
			l_oVisibleSize.height * 0.1f);
	m_pPinkLayer->addChild(m_pLabelCompareButton);

	//back button
	m_pBackButton = ui::Button::create("Ludomuse/GUIElements/backNormal.png");
	m_pBackButton->setTouchEnabled(true);
	m_pBackButton->setAnchorPoint(Vec2(0.5, 1));
	m_pBackButton->setPosition(
			Vect(
					(l_oVisibleSize.width
							- m_pSpriteBackgroundBlueProfile->getContentSize().width)
							* 0.5f
							+ m_pSpriteBackgroundBlueProfile->getContentSize().width,
					l_oVisibleSize.height + l_oOrigin.y));
	m_pBackButton->addTouchEventListener(
			CC_CALLBACK_0(LmGameManager::back, this));
	m_pBackgroundLayer->addChild(m_pBackButton);

	//put the top band
	m_pSpriteBandTop = Sprite::create("Ludomuse/GUIElements/bandTop.png");
	m_pSpriteBandTop->setAnchorPoint(Vec2(0, 1));
	m_pSpriteBandTop->setPosition(
			Vec2(m_pSpriteBackgroundBlueProfile->getContentSize().width,
					l_oVisibleSize.height + l_oOrigin.y));
	m_pPinkLayer->addChild(m_pSpriteBandTop);

	//add the band mid at the top of the pink background
	m_pSpriteBandMid = Sprite::create("Ludomuse/GUIElements/bandMid.png");
	m_pSpriteBandMid->setAnchorPoint(Vec2(0, 0.5));
	m_pSpriteBandMid->setPosition(
			Vec2(m_pSpriteBackgroundBlueProfile->getContentSize().width,
					m_pSpriteBackgroundPink->getContentSize().height
							* s_fMagingRatioOfSpriteBackgroundUser2Profile));
	m_pPinkLayer->addChild(m_pSpriteBandMid);

	//title label app
	m_sTitleApplication = m_pLmServerManager->getSTitleApplication();
	m_pLabelTitleApplication = Label::createWithTTF(m_sTitleApplication,
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pLabelTitleApplication->setPosition(
			Vec2(
					(l_oVisibleSize.width
							- m_pSpriteBackgroundBlueProfile->getContentSize().width)
							* 0.5,
					m_pSpriteBandTop->getContentSize().height * 0.5f));
	m_pLabelTitleApplication->setColor(Color3B::WHITE);
	m_pLabelTitleApplication->setMaxLineWidth(
			(l_oVisibleSize.width
					- m_pSpriteBackgroundBlueProfile->getContentSize().width)
					* 0.7);
	m_pSpriteBandTop->addChild(m_pLabelTitleApplication);

	auto l_pMenu = Menu::create();
	l_pMenu->setPosition(Vec2::ZERO);
	m_pSpriteBandMid->addChild(l_pMenu);

	//play next interaction button
	m_pPlayNextInteractionButton = MenuItemImage::create(
			"Ludomuse/GUIElements/playNextInteraction.png",
			"Ludomuse/GUIElements/playNextInteraction.png",
			CC_CALLBACK_1(LmGameManager::runNextInteraction, this));
	m_pPlayNextInteractionButton->setAnchorPoint(Vec2(1, 0.5));
	m_pPlayNextInteractionButton->setPosition(
			Vec2(m_pSpriteBandMid->getContentSize().width * 0.95,
					m_pSpriteBandMid->getContentSize().height * 0.5f));
	l_pMenu->addChild(m_pPlayNextInteractionButton, 2);

	//checkbox settings
	m_pSettingsButton = MenuItemImage::create(
			"Ludomuse/GUIElements/settings.png",
			"Ludomuse/GUIElements/settings.png",
			CC_CALLBACK_1(LmGameManager::settings, this));
	m_pSettingsButton->setAnchorPoint(Vec2(1, 1));
	m_pSettingsButton->setPosition(
			Vec2(m_pSpriteBandMid->getContentSize().width, l_oWinSize.height));
	l_pMenu->addChild(m_pSettingsButton);

	//add settings menu
	m_pLmSettings->init();
	m_pLmSettings->setVisible(false);
	m_pLmSettings->addTo((Node*) m_pPinkLayer, 3);

	//init interactions sprite
	initDashboardInteraction();

	return true;
}

Sprite* LmGameManager::addSpriteToLabel(Label* l_pLabel, std::string filename)
{
	auto result = Sprite::create(filename);

	result->setAnchorPoint(Vec2(1, 0.5));

	//the label must have an anchor point 0.5;0.5
	auto position = l_pLabel->getPosition();
	auto labelSize = l_pLabel->getContentSize();
	result->setPosition(Vec2(position.x - labelSize.width * 0.5, position.y));

	return result;
}

Sprite* LmGameManager::makeUserAvatarSprite(LmUser* l_pUser)
{
	auto result = Sprite::create();

	if (l_pUser->isBParent() && l_pUser->isBMale())
	{
		result->setTexture("Ludomuse/GUIElements/dadavatar.png");
	}
	else if (l_pUser->isBParent() && !l_pUser->isBMale())
	{
		result->setTexture("Ludomuse/GUIElements/momavatar.png");
	}
	else if (!l_pUser->isBParent() && l_pUser->isBMale())
	{
		result->setTexture("Ludomuse/GUIElements/sonavatar.png");
	}
	else if (!l_pUser->isBParent() && !l_pUser->isBMale())
	{
		result->setTexture("Ludomuse/GUIElements/daughteravatar.png");
	}

	return result;
}

void LmGameManager::initDashboardInteraction()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	Sprite* l_pSpriteBuffer;
	int l_iIndex(0);

	//Browse the vector and init sprite then add to the vector
	for (std::vector<LmInteractionScene*>::iterator it =
			m_aInteractionSceneOfTheGame.begin();
			it != m_aInteractionSceneOfTheGame.end(); ++it)
	{
		//if the scene get a reward
		if ((*it)->getReward())
		{
			l_pSpriteBuffer = Sprite::create(
					"Ludomuse/GUIElements/interactionNotDoneRewardNotDone.png");
		}
		else
		{
			l_pSpriteBuffer = Sprite::create(
					"Ludomuse/GUIElements/interactionNotDoneNoReward.png");
		}

		l_pSpriteBuffer->setAnchorPoint(Vec2(0, 0));
		l_pSpriteBuffer->setPosition(
				Vec2((l_iIndex) * s_fMarginBetweenInteraction,
						m_pSpriteBandMid->getContentSize().height * 0.5));
		m_aSpritesInteractions.push_back(l_pSpriteBuffer);

		//if the scene get a reward for the second user
		if ((*it)->getPLmRewardUser2())
		{
			l_pSpriteBuffer =
					Sprite::create(
							"Ludomuse/GUIElements/interactionNotDoneRewardNotDoneUser2.png");
		}
		else
		{
			l_pSpriteBuffer = Sprite::create(
					"Ludomuse/GUIElements/interactionNotDoneNoRewardUser2.png");
		}

		l_pSpriteBuffer->setAnchorPoint(Vec2(0, 1));
		l_pSpriteBuffer->setPosition(
				Vec2((l_iIndex) * s_fMarginBetweenInteraction,
						m_pSpriteBandMid->getContentSize().height * 0.5));
		m_aSpritesInteractionsUser2.push_back(l_pSpriteBuffer);

		l_iIndex++;
	}

	//init the scroll view
	m_pScrollView = ui::ScrollView::create();
	Size l_oScrollFrameSize = Size(m_pSpriteBandMid->getContentSize().width,
			m_pSpriteBandMid->getContentSize().height
					+ l_pSpriteBuffer->getContentSize().height);
	m_pScrollView->setContentSize(l_oScrollFrameSize);
	m_pScrollView->setAnchorPoint(Vec2(0, 0));
	m_pScrollView->setPosition(Vec2(0, 0));
	m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);

	//if there is no enough interaction to fill the frame size
	Size l_oContainerSize;
	if ((l_iIndex) * (s_fMarginBetweenInteraction)
			<= m_pSpriteBandMid->getContentSize().width)
	{
		l_oContainerSize = Size(m_pSpriteBandMid->getContentSize().width,
				m_pSpriteBandMid->getContentSize().height
						+ l_pSpriteBuffer->getContentSize().height);
	}
	else
	{
		l_oContainerSize = Size((l_iIndex) * (s_fMarginBetweenInteraction),
				m_pSpriteBandMid->getContentSize().height
						+ l_pSpriteBuffer->getContentSize().height);
	}

	m_pScrollView->setInnerContainerSize(l_oContainerSize);

	m_pLayerScrollView = Layer::create();
	m_pScrollView->addChild(m_pLayerScrollView);

	//user 1
	for (std::vector<Sprite*>::iterator it = m_aSpritesInteractions.begin();
			it != m_aSpritesInteractions.end(); ++it)
	{
		m_pLayerScrollView->addChild((*it));
	}

	//user 2
	for (std::vector<Sprite*>::iterator it =
			m_aSpritesInteractionsUser2.begin();
			it != m_aSpritesInteractionsUser2.end(); ++it)
	{
		(*it)->setVisible(false);
		m_pLayerScrollView->addChild((*it));
	}

	m_pScrollView->setBounceEnabled(true);
	m_pScrollView->setTouchEnabled(true);
	m_pScrollView->setSwallowTouches(false);

	m_pSpriteBandMid->addChild(m_pScrollView);

	//init the listener to touch interaction and see description
	//init listener
	m_pListener = EventListenerTouchOneByOne::create();
	m_pListener->onTouchBegan = CC_CALLBACK_2(LmGameManager::onTouchBegan,
			this);
	m_pListener->onTouchMoved = CC_CALLBACK_2(LmGameManager::onTouchMoved,
			this);
	m_pListener->onTouchEnded = CC_CALLBACK_2(LmGameManager::onTouchEnded,
			this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
			m_pListener, m_pSpriteBandMid);

	//init the box containing description

	m_pDescriptionBox = Sprite::create(
			"Ludomuse/GUIElements/descriptionBox.png");
	m_pDescriptionBox->setPosition(
			Vec2(
					m_pSpriteBackgroundBlueProfile->getContentSize().width
							+ (l_oVisibleSize.width
									- m_pSpriteBackgroundBlueProfile->getContentSize().width)
									* 0.5, l_oVisibleSize.height * 0.6));
	m_pBackgroundLayer->addChild(m_pDescriptionBox);

	m_pDescriptionLabel = Label::createWithTTF("test",
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.02);
	m_pDescriptionBox->addChild(m_pDescriptionLabel);
	m_pDescriptionLabel->setPosition(
			Vec2(m_pDescriptionBox->getContentSize().width * 0.5,
					m_pDescriptionBox->getContentSize().height * 0.5));
	m_pDescriptionLabel->setMaxLineWidth(
			m_pDescriptionBox->getContentSize().width * 0.9);

	m_pDescriptionBox->setVisible(false);

}

void LmGameManager::updateSpriteToLabel(cocos2d::Sprite* sprite,
		cocos2d::Label* label)
{
	//the label must have an anchor point 0.5;0.5
	auto position = label->getPosition();
	auto labelSize = label->getContentSize();
	sprite->setPosition(Vec2(position.x - labelSize.width * 0.5, position.y));
}

void LmGameManager::updateDashboard()
{
	/*
	 * USER 1 LABEL
	 */

	//update interaction done user1
	char l_aInteractionDoneString[10];
	sprintf(l_aInteractionDoneString, "%d/%d", m_iInteractionDone,
			m_aInteractionSceneOfTheGame.size());
	//slow but we are using it just at the end of an interaction
	m_pLabelInteractionDone->setString(l_aInteractionDoneString);

	//update sprite associated user1 inetraction done
	updateSpriteToLabel(m_pCheckSpriteUser1, m_pLabelInteractionDone);

	//update score user1
	char l_aScoreString[10];
	sprintf(l_aScoreString, "%d pts", m_pUser1->getPScore());
	m_pLabelScore->setString(l_aScoreString);

	//update sprite associated user1 score
	updateSpriteToLabel(m_pStarUser1Sprite, m_pLabelScore);

	/*
	 * USER 2 LABEL
	 */

	//update interaction done user2
	sprintf(l_aInteractionDoneString, "%d/%d", m_iInteractionDoneUser2,
			m_aInteractionSceneOfTheGame.size());
	m_pLabelInteractionDoneUser2->setString(l_aInteractionDoneString);

	//update sprite associated user2 inetraction done
	updateSpriteToLabel(m_pCheckSpriteUser2, m_pLabelInteractionDoneUser2);

	//update score user2
	sprintf(l_aScoreString, "%d pts", m_pUser2->getPScore());
	m_pLabelScoreUser2->setString(l_aScoreString);

	//update sprite associated user1 score
	updateSpriteToLabel(m_pStarUser2Sprite, m_pLabelScoreUser2);

}

void LmGameManager::compare()
{
	if (m_bActionIsDone)
	{

		//play button clicked sound
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
				FILENAME_BUTTON_CLICKED);

		m_bActionIsDone = false;

		m_pListener->setEnabled(false);

		auto l_oCompareAction =
				MoveBy::create(s_fTimeCompareAction,
						Vect(0,
								(m_pSpriteBackgroundPink->getContentSize().height)
										* 0.5f
										- s_fMagingRatioOfSpriteBackgroundUser2Profile));
		auto l_oCompareActionIsDone = CallFunc::create(
				std::bind(&LmGameManager::compareDone, this));
		m_pPinkLayer->runAction(
				Sequence::create(l_oCompareAction, l_oCompareActionIsDone,
						nullptr));

	}
}

void LmGameManager::compareDone()
{

	m_bActionIsDone = true;

	compareScreen(true);

	//move the label of the score and the star
	m_pLabelScoreUser2->setPosition(
			m_pSpriteBackgroundPinkProfile->getContentSize().width * 0.5,
			m_pSpriteBackgroundPinkProfile->getContentSize().height * 0.3f);
	updateSpriteToLabel(m_pStarUser2Sprite, m_pLabelScoreUser2);

}

void LmGameManager::back()
{
	if (m_bActionIsDone)
	{
		//play button clicked sound
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
		FILENAME_BUTTON_CLICKED);

		m_bActionIsDone = false;
		auto l_oBackAction =
				MoveBy::create(s_fTimeCompareAction,
						Vect(0,
								-(m_pSpriteBackgroundPink->getContentSize().height)
										* 0.5f
										- s_fMagingRatioOfSpriteBackgroundUser2Profile));
		auto l_oBackActionIsDone = CallFunc::create(
				std::bind(&LmGameManager::backDone, this));
		m_pPinkLayer->runAction(
				Sequence::create(l_oBackAction, l_oBackActionIsDone, nullptr));
	}
}

void LmGameManager::backDone()
{
	m_bActionIsDone = true;

	m_pListener->setEnabled(true);

	compareScreen(false);

	//move the label of the score and the star
	m_pLabelScoreUser2->setPosition(
			m_pSpriteBackgroundPinkProfile->getContentSize().width * 0.5,
			m_pSpriteBackgroundPinkProfile->getContentSize().height * 0.7f);
	updateSpriteToLabel(m_pStarUser2Sprite, m_pLabelScoreUser2);

}

void LmGameManager::setSpritesInteractionsUser2Visible(bool visible)
{
	m_pSpriteBackgroundPink->setVisible(!visible);

	for (std::vector<Sprite*>::iterator it =
			m_aSpritesInteractionsUser2.begin();
			it != m_aSpritesInteractionsUser2.end(); ++it)
	{
		(*it)->setVisible(visible);
	}
}

void LmGameManager::compareScreen(bool visible)
{
	m_pCompareButton->setVisible(!visible);
	m_pLabelCompareButton->setVisible(!visible);
	//m_pSpriteBandTop->setVisible(!visible);
	//m_pBackButton->setVisible(visible);
	m_pAvatarSpriteUser2->setVisible(visible);
	m_pCheckSpriteUser2->setVisible(visible);
	m_pLabelInteractionDoneUser2->setVisible(visible);

	setSpritesInteractionsUser2Visible(visible);

}

void LmGameManager::runInteraction(int index)
{
	//enable the back button of the interaction because it was disable before the back action (LmInteractionScene::backToDashboard)
	m_aInteractionSceneOfTheGame.at(index)->setBBackPressed(false);

	CCLOG("pushscene");
	inputDisabled(true);
	Director::getInstance()->pushScene(
			TransitionFade::create(s_fTimeBetweenLmLayer,
					m_aInteractionSceneOfTheGame.at(index)));

	//it was a back
	if (m_bBackToDashboard)
	{
		m_aInteractionSceneOfTheGame.at(index)->restart();
	}
}

void LmGameManager::runNextInteraction(Ref* p_Sender)
{
	//play button clicked sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	//if its the last interactionscene the app finished
	if (m_iIndexInteractionScene >= m_aInteractionSceneOfTheGame.size())
	{
		//no more games for now end application
		LmSettings::endApplication();
	}
	else
	{

		//if yes we need to init the scene
		if (!m_bBackToDashboard)
		{
			//we pass the local user
			if (!m_aInteractionSceneOfTheGame.at(m_iIndexInteractionScene)->init(
					m_pUser1))
			{
				CCLOG("Interaction scene init failed");
			}

		}

		runInteraction(m_iIndexInteractionScene);

	}

}

void LmGameManager::settings(cocos2d::Ref*)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			FILENAME_BUTTON_CLICKED);

	m_pLmSettings->setVisible(true);

}

void LmGameManager::inputDisabled(bool l_bTrue)
{
	m_pBackButton->setTouchEnabled(!l_bTrue);
	m_pCompareButton->setTouchEnabled(!l_bTrue);
	m_pPlayNextInteractionButton->setEnabled(!l_bTrue);
	m_pListener->setEnabled(!l_bTrue);
	m_pSettingsButton->setEnabled(!l_bTrue);

}

bool LmGameManager::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{

	auto index = interactionTouched(touch);

	if (index >= 0)
	{
		m_pDescriptionBox->setVisible(true);
		m_pDescriptionLabel->setString(
				m_aInteractionSceneOfTheGame.at(index)->getSDescription());
	}
	else
	{
		m_pDescriptionBox->setVisible(false);

	}

	return true;
}

void LmGameManager::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto index = interactionTouched(touch);

	if (index >= 0)
	{
		m_pDescriptionBox->setVisible(true);
		m_pDescriptionLabel->setString(
				m_aInteractionSceneOfTheGame.at(index)->getSDescription());
	}
	else
	{
		m_pDescriptionBox->setVisible(false);

	}
}

void LmGameManager::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	m_pDescriptionBox->setVisible(false);

}

int LmGameManager::interactionTouched(cocos2d::Touch* touch)
{
	auto l_oTouchLocation = touch->getLocation();

	Point l_oPointInWindowSpace;
	Size elementSize;
	Rect boundingboxInWorldSpace;

	for (int i = 0; i < m_aSpritesInteractions.size(); i++)
	{
		l_oPointInWindowSpace = m_pLayerScrollView->convertToWorldSpace(
				m_aSpritesInteractions.at(i)->getPosition());

		elementSize = m_aSpritesInteractions.at(i)->getContentSize();

		boundingboxInWorldSpace = Rect(l_oPointInWindowSpace.x,
				l_oPointInWindowSpace.y, elementSize.width, elementSize.height);

		if (boundingboxInWorldSpace.containsPoint(l_oTouchLocation))
		{
			return i;
		}
	}
	return -1;
}

void LmGameManager::onReceivingMsg(bytes l_oMsg)
{
	CCLOG("lmgamemanager _event is %d", _event);
	switch (_event)
	{
	case LmEvent::ReadyForNextInteraction:
		CCLOG("ReadyForNextInteraction");
		ON_CC_THREAD(LmGameManager::onReadyForNextInteractionEvent, this,
				l_oMsg)
		;
		break;

	case LmEvent::InteractionDone:
		CCLOG("InteractionDone");
		ON_CC_THREAD(LmGameManager::onInteractionDoneEvent, this, l_oMsg)
		;
		break;
	default:
		break;
	}

}

void LmGameManager::onReadyForNextInteractionEvent(bytes l_oMsg)
{
	int idInteractionScene = l_oMsg.readInt();

	//when we received it mean the user 2 cklicked his button
	m_aInteractionSceneOfTheGame.at(idInteractionScene)->setBUser2IsReadyForNextInteraction(
			true);

	if (!m_aInteractionSceneOfTheGame.at(idInteractionScene)->isBGameIsRunning())
	{
		m_aInteractionSceneOfTheGame.at(idInteractionScene)->startGame();
	}

}

void LmGameManager::onInteractionDoneEvent(bytes l_oMsg)
{
	int idInteractionScene = l_oMsg.readInt();
	bool l_bWin = l_oMsg.readBool();

	if (m_aInteractionSceneOfTheGame.at(idInteractionScene)->getRewardUser2())
	{
		if (l_bWin)
		{
			m_pUser2->winReward(
					m_aInteractionSceneOfTheGame.at(idInteractionScene)->getPLmRewardUser2());
			m_aSpritesInteractionsUser2.at(idInteractionScene)->setTexture(
					"Ludomuse/GUIElements/interactionDoneRewardDoneUser2.png");

		}
		else
		{
			m_aSpritesInteractionsUser2.at(idInteractionScene)->setTexture(
					"Ludomuse/GUIElements/interactionDoneRewardNotDoneUser2.png");

		}

	}
	else
	{
		m_aSpritesInteractionsUser2.at(idInteractionScene)->setTexture(
				"Ludomuse/GUIElements/interactionDoneNoRewardUser2.png");

	}

	m_iInteractionDoneUser2++;

	updateDashboard();
}

