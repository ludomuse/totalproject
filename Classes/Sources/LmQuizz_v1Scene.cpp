/*
 * LmQuizz_v1.cpp
 *
 *  Created on: 24 sept. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmQuizz_v1Scene.h"

using namespace cocos2d;
using namespace cocos2d::ui;

LmQuizz_v1Scene::LmQuizz_v1Scene(const LmQuizz_v1SceneSeed &l_Seed) :
		LmInteractionScene()
{

	//json
	m_sFilenameSpriteBackground = l_Seed.FilenameSpriteBackground;
	m_sFilenameSpriteBandTop = l_Seed.FilenameSpriteBandTop;
	m_sFilenameSpriteAnswerBackground = l_Seed.FilenameSpriteAnswerBackground;
	m_sFilenameSpriteAnswerCross = l_Seed.FilenameSpriteAnswerCross;
	m_aQuestions = l_Seed.Questions;
	m_iAttemptByQuestion = l_Seed.AttemptByQuestion;
	m_fTimerDuration = l_Seed.TimerDuration;
	m_bTimerEnbaled = l_Seed.TimerEnbaled;
	m_sFilenameAudioAnswerSelected = l_Seed.FilenameAudioAnswerSelected;
	m_pInGameScreenParent = l_Seed.InGameScreenParent;
	m_pReplayScreen=l_Seed.ReplayScreen;

	//pointer
	m_pSpriteBackground = nullptr;
	m_pTimer = nullptr;
	m_pNextQuestionButton = nullptr;
	m_pAnswerLabel[0] = nullptr;
	m_pAnswerLabel[1] = nullptr;
	m_pAnswerLabel[2] = nullptr;
	m_pAnswerLabel[3] = nullptr;
	m_pQuestionLabel = nullptr;
	m_pBandTopSprite = nullptr;
	m_pCheckBoxAnswer[0] = nullptr;
	m_pCheckBoxAnswer[1] = nullptr;
	m_pCheckBoxAnswer[2] = nullptr;
	m_pCheckBoxAnswer[3] = nullptr;

	//primitive type
	m_iCounter = 0;
	m_iAnswerSelected = -1;
	m_fHeight = 0;
	m_iIndexQuestion = -1;
	m_iNumberOfAttempt = m_iAttemptByQuestion;
	m_bQuestionFinished = true;
}

LmQuizz_v1Scene::~LmQuizz_v1Scene()
{
	for (std::vector<LmQuestion*>::iterator it = m_aQuestions.begin();
			it != m_aQuestions.end(); ++it)
	{
		delete (*it);
	}
}

void LmQuizz_v1Scene::runGame()
{
	if (!initGame())
	{
		CCLOG("initGame failed");
	}

	//we preload the sound
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(
			m_sFilenameAudioAnswerSelected.c_str());
}

void LmQuizz_v1Scene::restart()
{
	//need top schedule the timer agian
	if (m_bTimerEnbaled && !m_bQuestionFinished)
	{
		schedule(schedule_selector(LmQuizz_v1Scene::updateLoadingBar),
				m_fTimerDuration);
	}
}

void LmQuizz_v1Scene::resetScene()
{

	if (m_bReplayButtonSync)
	{

		m_bReplayButtonSync = false;
		m_iIndexQuestion = -1;	//so when we init the next question index = 0

		//reset replay button
		m_pReplayButton->setVisible(false);
		m_bQuestionFinished = false;

		//if it's the parent that clicked
		if (m_pUser->isBParent())
		{
			m_iIndexQuestion++;

			m_pReplayScreen->setVisible(false);

			bytes msg(10);
			msg << LmEvent::Replay;
			WIFIFACADE->sendBytes(msg);
		}
		else
		{
			//reset checkbox
			m_pCheckBoxAnswer[0]->setSelected(false);
			m_pCheckBoxAnswer[1]->setSelected(false);
			m_pCheckBoxAnswer[2]->setSelected(false);
			m_pCheckBoxAnswer[3]->setSelected(false);

			m_iNumberOfAttempt = m_iAttemptByQuestion;

			if (m_bTimerEnbaled)
			{
				//reset timer
				m_pTimer->setPercent(0);
				m_iCounter = 0;
				schedule(schedule_selector(LmQuizz_v1Scene::updateLoadingBar),
						m_fTimerDuration);
			}
			//disable answer selected
			m_iAnswerSelected = -1;
			initNextQuestion();
		}

	}
}

bool LmQuizz_v1Scene::initGame()
{

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//init the background
	m_pSpriteBackground = Sprite::create(m_sFilenameSpriteBackground);
	m_pSpriteBackground->setPosition(l_oVisibleSize.width * 0.5f + l_oOrigin.x,
			l_oVisibleSize.height * 0.5f + l_oOrigin.y);
	m_pLayerGame->addChild(m_pSpriteBackground);

	if (m_pUser->isBParent())
	{
		m_pInGameScreenParent->init();
		m_pReplayScreen->init();

		m_pLayerGame->addChild(m_pInGameScreenParent);
		m_pLayerGame->addChild(m_pReplayScreen,1);
		m_pReplayScreen->setVisible(false);

	}


	//init timer
	m_pTimer = LoadingBar::create();
	m_pTimer->loadTexture("Ludomuse/Content/timer.png");
	m_pTimer->setPercent(0);
	m_pTimer->setPosition(
			Point(l_oVisibleSize.width * 0.5f + l_oOrigin.x,
					l_oVisibleSize.height * 0.05 + l_oOrigin.y));
	if (m_bTimerEnbaled && !m_pUser->isBParent())
	{
		m_pLayerGame->addChild(m_pTimer);
	}

	//band top sprite
	m_pBandTopSprite = Sprite::create(m_sFilenameSpriteBandTop);
	m_pBandTopSprite->setAnchorPoint(Vec2(0, 1));
	m_pBandTopSprite->setPosition(Vec2(0, l_oVisibleSize.height + l_oOrigin.y));

	if (!m_pUser->isBParent())
	{
		m_pLayerGame->addChild(m_pBandTopSprite);
	}

	//usefull height to scale
	m_fHeight = l_oVisibleSize.height
			- m_pBandTopSprite->getContentSize().height
			- m_pTimer->getContentSize().height;

	//menuitem answer create + set position
	//1
	m_pCheckBoxAnswer[0] = CheckBox::create(m_sFilenameSpriteAnswerBackground,
			m_sFilenameSpriteAnswerCross);
	m_pCheckBoxAnswer[0]->setTouchEnabled(true);
	m_pCheckBoxAnswer[0]->setSwallowTouches(false);
	m_pCheckBoxAnswer[0]->setPosition(
			Vec2(l_oVisibleSize.width * 0.25,
					m_fHeight * 0.75f + m_pTimer->getContentSize().height));
	m_pCheckBoxAnswer[0]->addEventListener(
			CC_CALLBACK_2(LmQuizz_v1Scene::answerSelected, this));
	//2
	m_pCheckBoxAnswer[1] = CheckBox::create(m_sFilenameSpriteAnswerBackground,
			m_sFilenameSpriteAnswerCross);
	m_pCheckBoxAnswer[1]->setTouchEnabled(true);
	m_pCheckBoxAnswer[1]->setSwallowTouches(false);
	m_pCheckBoxAnswer[1]->setPosition(
			Vec2(l_oVisibleSize.width * 0.75,
					m_fHeight * 0.75f + m_pTimer->getContentSize().height));
	m_pCheckBoxAnswer[1]->addEventListener(
			CC_CALLBACK_2(LmQuizz_v1Scene::answerSelected, this));
	//3
	m_pCheckBoxAnswer[2] = CheckBox::create(m_sFilenameSpriteAnswerBackground,
			m_sFilenameSpriteAnswerCross);
	m_pCheckBoxAnswer[2]->setTouchEnabled(true);
	m_pCheckBoxAnswer[2]->setSwallowTouches(false);
	m_pCheckBoxAnswer[2]->setPosition(
			Vec2(l_oVisibleSize.width * 0.25,
					m_fHeight * 0.25f + m_pTimer->getContentSize().height));
	m_pCheckBoxAnswer[2]->addEventListener(
			CC_CALLBACK_2(LmQuizz_v1Scene::answerSelected, this));
	//4
	m_pCheckBoxAnswer[3] = CheckBox::create(m_sFilenameSpriteAnswerBackground,
			m_sFilenameSpriteAnswerCross);
	m_pCheckBoxAnswer[3]->setTouchEnabled(true);
	m_pCheckBoxAnswer[3]->setSwallowTouches(false);
	m_pCheckBoxAnswer[3]->setPosition(
			Vec2(l_oVisibleSize.width * 0.75,
					m_fHeight * 0.25f + m_pTimer->getContentSize().height));
	m_pCheckBoxAnswer[3]->addEventListener(
			CC_CALLBACK_2(LmQuizz_v1Scene::answerSelected, this));

	if (!m_pUser->isBParent())
	{
		m_pLayerGame->addChild(m_pCheckBoxAnswer[0]);
		m_pLayerGame->addChild(m_pCheckBoxAnswer[1]);
		m_pLayerGame->addChild(m_pCheckBoxAnswer[2]);
		m_pLayerGame->addChild(m_pCheckBoxAnswer[3]);
	}

	//init label question
	m_pQuestionLabel = Label::createWithTTF("", "Fonts/JosefinSans-Regular.ttf",
			l_oVisibleSize.width * 0.04);
	m_pBandTopSprite->addChild(m_pQuestionLabel);
	m_pQuestionLabel->setPosition(
			Vec2(m_pBandTopSprite->getContentSize().width * 0.5,
					m_pBandTopSprite->getContentSize().height * 0.5));
	m_pQuestionLabel->setColor(Color3B::BLACK);
	m_pQuestionLabel->setAlignment(TextHAlignment::CENTER);
	m_pQuestionLabel->setMaxLineWidth(l_oVisibleSize.width * 0.65);

	//init label answer and add them to their menuitemiamge
	//1
	m_pAnswerLabel[0] = Label::createWithTTF("",
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pCheckBoxAnswer[0]->addChild(m_pAnswerLabel[0]);
	m_pAnswerLabel[0]->setPosition(
			Vec2(m_pCheckBoxAnswer[0]->getContentSize().width * 0.5,
					m_pCheckBoxAnswer[0]->getContentSize().height * 0.5));
	m_pAnswerLabel[0]->setColor(Color3B::BLACK);
	m_pAnswerLabel[0]->setMaxLineWidth(
			m_pCheckBoxAnswer[0]->getContentSize().width * 0.9);
	//2
	m_pAnswerLabel[1] = Label::createWithTTF("",
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pCheckBoxAnswer[1]->addChild(m_pAnswerLabel[1]);
	m_pAnswerLabel[1]->setPosition(
			Vec2(m_pCheckBoxAnswer[1]->getContentSize().width * 0.5,
					m_pCheckBoxAnswer[1]->getContentSize().height * 0.5));
	m_pAnswerLabel[1]->setColor(Color3B::BLACK);
	m_pAnswerLabel[1]->setMaxLineWidth(
			m_pCheckBoxAnswer[1]->getContentSize().width * 0.9);
	//3
	m_pAnswerLabel[2] = Label::createWithTTF("",
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pCheckBoxAnswer[2]->addChild(m_pAnswerLabel[2]);
	m_pAnswerLabel[2]->setPosition(
			Vec2(m_pCheckBoxAnswer[2]->getContentSize().width * 0.5,
					m_pCheckBoxAnswer[2]->getContentSize().height * 0.5));
	m_pAnswerLabel[2]->setColor(Color3B::BLACK);
	m_pAnswerLabel[2]->setMaxLineWidth(
			m_pCheckBoxAnswer[2]->getContentSize().width * 0.9);
	//4
	m_pAnswerLabel[3] = Label::createWithTTF("",
			"Fonts/JosefinSans-Regular.ttf", l_oVisibleSize.width * 0.04);
	m_pCheckBoxAnswer[3]->addChild(m_pAnswerLabel[3]);
	m_pAnswerLabel[3]->setPosition(
			Vec2(m_pCheckBoxAnswer[3]->getContentSize().width * 0.5,
					m_pCheckBoxAnswer[3]->getContentSize().height * 0.5));
	m_pAnswerLabel[3]->setColor(Color3B::BLACK);
	m_pAnswerLabel[3]->setMaxLineWidth(
			m_pCheckBoxAnswer[3]->getContentSize().width * 0.9);

	//init next question button
	m_pNextQuestionButton = ui::Button::create(
			"Ludomuse/GUIElements/nextButtonNormal.png",
			"Ludomuse/GUIElements/nextButtonPressed.png");
	m_pNextQuestionButton->setTouchEnabled(true);
	m_pNextQuestionButton->setAnchorPoint(Vec2(1,0.5));
	m_pNextQuestionButton->setPosition(
			Vec2(l_oVisibleSize.width , l_oVisibleSize.height * 0.1));
	m_pNextQuestionButton->addTouchEventListener(
			CC_CALLBACK_0(LmQuizz_v1Scene::beginQuestion, this));
	m_pNextQuestionButton->setVisible(false);
	m_pLayerGame->addChild(m_pNextQuestionButton,2);

	m_iIndexQuestion = -1;	//so when we init the next question index = 0
	beginQuestion();

	return true;

}

void LmQuizz_v1Scene::updateLoadingBar(float dt)
{
	m_iCounter++;
	if (m_iCounter > 100)
	{
		scheduleOnce(schedule_selector(LmQuizz_v1Scene::timerEnd), 0);
		unschedule(schedule_selector(LmQuizz_v1Scene::updateLoadingBar));
	}
	m_pTimer->setPercent(m_iCounter);
}

void LmQuizz_v1Scene::timerEnd(float dt)
{
	m_iNumberOfAttempt = 0;
	checkAnswer();
}

void LmQuizz_v1Scene::beginQuestion()
{
	if (m_bQuestionFinished)
	{
		CCLOG("begin question ");

		m_pNextQuestionButton->setVisible(false);

		m_bQuestionFinished = false;

		CCLOG("index %d", m_iIndexQuestion);

		//no more layers end of the game
		if ((int) m_iIndexQuestion >= (int) (m_aQuestions.size() - 1))
		{
			/*
			 * m_bwin is set to true for parent and child
			 * (architecture suck because remove at the last moment)
			 */
			win(m_bWin);
		}
		else
		{
			//reset checkbox
			m_pCheckBoxAnswer[0]->setSelected(false);
			m_pCheckBoxAnswer[1]->setSelected(false);
			m_pCheckBoxAnswer[2]->setSelected(false);
			m_pCheckBoxAnswer[3]->setSelected(false);

			m_iNumberOfAttempt = m_iAttemptByQuestion;

			if (m_bTimerEnbaled)
			{
				//reset timer
				m_pTimer->setPercent(0);
				m_iCounter = 0;
				schedule(schedule_selector(LmQuizz_v1Scene::updateLoadingBar),
						m_fTimerDuration);
			}

			//disable answer selected
			m_iAnswerSelected = -1;

			//next layer
			initNextQuestion();
		}
	}

}

void LmQuizz_v1Scene::initNextQuestion()
{
//increment index
	m_iIndexQuestion++;
	auto l_pQuestion = m_aQuestions.at(m_iIndexQuestion);

//set label string
	m_pQuestionLabel->setString(l_pQuestion->getSQuestion());
	m_pAnswerLabel[0]->setString(l_pQuestion->getSAnswer1());
	m_pAnswerLabel[1]->setString(l_pQuestion->getSAnswer2());
	m_pAnswerLabel[2]->setString(l_pQuestion->getSAnswer3());
	m_pAnswerLabel[3]->setString(l_pQuestion->getSAnswer4());

}

void LmQuizz_v1Scene::checkAnswer()
{
//good answer
	if (m_aQuestions.at(m_iIndexQuestion)->getINumberGoodAnswer()
			== m_iAnswerSelected)
	{

		questionFinish(true);
	}
	else
	{

		//still have attempt
		if (m_iNumberOfAttempt > 1)
		{
			m_iNumberOfAttempt--;

		}
		else
		{
			//load good texture for the next button to indicate loose
			questionFinish(false);
		}
	}
}

void LmQuizz_v1Scene::answerSelected(Ref* pSender, CheckBox::EventType type)
{
	auto l_pCheckBoxPressed = dynamic_cast<CheckBox*>(pSender);

	if (m_bQuestionFinished)
	{
		//force to unchange the check
		switch (type)
		{
		case CheckBox::EventType::SELECTED:
			l_pCheckBoxPressed->setSelected(false);
			break;

		case CheckBox::EventType::UNSELECTED:
			l_pCheckBoxPressed->setSelected(true);
			break;

		default:
			break;
		}
	}
	else
	{
		int l_iIdCheckBox = 0;

		//register the sender
		if (l_pCheckBoxPressed == m_pCheckBoxAnswer[0])
		{
			l_iIdCheckBox = 1;
		}
		else if (l_pCheckBoxPressed == m_pCheckBoxAnswer[1])
		{
			l_iIdCheckBox = 2;
		}
		else if (l_pCheckBoxPressed == m_pCheckBoxAnswer[2])
		{
			l_iIdCheckBox = 3;
		}
		else if (l_pCheckBoxPressed == m_pCheckBoxAnswer[3])
		{
			l_iIdCheckBox = 4;
		}
		else
		{
			CCLOG("callback checkbox failed");
		}

		switch (type)
		{
		case CheckBox::EventType::SELECTED:
			//play a sound from the json
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
					m_sFilenameAudioAnswerSelected.c_str(), false);
			select(l_iIdCheckBox, true);
			break;

		case CheckBox::EventType::UNSELECTED:
			select(l_iIdCheckBox, false);
			break;

		default:
			break;
		}
		checkAnswer();
	}

}

void LmQuizz_v1Scene::questionFinish(bool goodAnswer)
{
	m_bQuestionFinished = true;
//disable touch on checkbox
	if (m_bTimerEnbaled)
	{
		//stop timer
		unschedule(schedule_selector(LmQuizz_v1Scene::updateLoadingBar));
	}

	if (goodAnswer)
	{
		win(true);

		//send to parent that was a good answer
		bytes msg(10);
		msg << LmEvent::GoodAnswer;
		msg.write(true);
		WIFIFACADE->sendBytes(msg);
	}
	else
	{
		//send to parent that was a bad answer
		bytes msg(10);
		msg << LmEvent::GoodAnswer;
		msg.write(false);
		WIFIFACADE->sendBytes(msg);
	}

}

void LmQuizz_v1Scene::select(int l_iIdCheckBoxPressed, bool selected)
{
	if (selected)
	{
		//the answer selected is the checkbox selected
		m_iAnswerSelected = l_iIdCheckBoxPressed;

		//unselect the others
		switch (l_iIdCheckBoxPressed)
		{
		case 1:
			m_pCheckBoxAnswer[1]->setSelected(false);
			m_pCheckBoxAnswer[2]->setSelected(false);
			m_pCheckBoxAnswer[3]->setSelected(false);
			break;
		case 2:
			m_pCheckBoxAnswer[0]->setSelected(false);
			m_pCheckBoxAnswer[2]->setSelected(false);
			m_pCheckBoxAnswer[3]->setSelected(false);
			break;
		case 3:
			m_pCheckBoxAnswer[0]->setSelected(false);
			m_pCheckBoxAnswer[1]->setSelected(false);
			m_pCheckBoxAnswer[3]->setSelected(false);
			break;
		case 4:
			m_pCheckBoxAnswer[0]->setSelected(false);
			m_pCheckBoxAnswer[1]->setSelected(false);
			m_pCheckBoxAnswer[2]->setSelected(false);
			break;
		default:
			break;
		}

	}
	else
	{
		//the answer selected is reset
		m_iAnswerSelected = -1;
	}
}

void LmQuizz_v1Scene::checkBoxTouchEnabled(bool enabled)
{
	m_pCheckBoxAnswer[0]->setEnabled(enabled);
	m_pCheckBoxAnswer[1]->setEnabled(enabled);
	m_pCheckBoxAnswer[2]->setEnabled(enabled);
	m_pCheckBoxAnswer[3]->setEnabled(enabled);
}

void LmQuizz_v1Scene::onReceivingMsg(bytes l_oMsg)
{

	CCLOG("lmquizzscene _event is %d", LmWifiObserver::_event);
	switch (LmWifiObserver::_event)
	{
	case LmEvent::GoodAnswer:
		CCLOG("GoodAnswer");
		ON_CC_THREAD(LmQuizz_v1Scene::onGoodAnswerEvent, this, l_oMsg)
		;
		break;
	case LmEvent::Replay:
		CCLOG("Replay");
		m_bReplayButtonSync = true;
		ON_CC_THREAD(LmQuizz_v1Scene::resetScene, this)
		;
		break;
	default:
		break;
	}

}

void LmQuizz_v1Scene::onGoodAnswerEvent(bytes l_oMsg)
{
	//parent receive msg
	m_bReplayButtonSync = true;

	bool buffer = l_oMsg.readBool();
	goodAnswerFromChild(buffer);

}

void LmQuizz_v1Scene::goodAnswerFromChild(bool good)
{
	m_bQuestionFinished = true;

	if (good)
	{
		win(true);

	}
	else
	{

		CCLOG("bad ansswer");

		//can go to the next
		m_pReplayButton->setVisible(true);
		m_pReplayScreen->setVisible(true);
	}
}
