/*
 * LmDrawScene.cpp
 *
 *  Created on: 23 nov. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmDrawScene.h"
#include "../Include/LmMenuItemImage.h"

#define COCOS2D_DEBUG 1

using namespace cocos2d;

LmDrawScene::LmDrawScene(const LmDrawSceneSeed& l_oSeed) :
		m_oSeed(l_oSeed)
{

}

LmDrawScene::~LmDrawScene()
{

}

void LmDrawScene::runGame()
{
	if (!initGame())
	{

	}
}

bool LmDrawScene::initGame()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	auto l_pBackground = Sprite::create(m_oSeed.FilenameSpriteBackground);
	l_pBackground->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.5));
	m_pLayerGame->addChild(l_pBackground);

	if (m_pUser->isBParent())
	{
		auto l_pButton = LmMenuItemImage::create("Bonjour",
				CC_CALLBACK_1(LmDrawScene::sendHello, this));

		l_pButton->setPosition(
				Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.5));
		auto l_oMenu = Menu::create(l_pButton, nullptr);
		l_oMenu->setPosition(Vec2::ZERO);
		m_pLayerGame->addChild(l_oMenu, 1);
	}
}

void LmDrawScene::resetScene()
{

}

void LmDrawScene::restart()
{

}

void LmDrawScene::onReceivingMsg(bytes l_oMsg)
{
	LmInteractionScene::onReceivingMsg(l_oMsg);

	switch (LmWifiObserver::_event)
	{
		case LmEvent::GoodAnswer:
			ON_CC_THREAD(LmDrawScene::onGoodAnswerEvent, this, l_oMsg)
			;
			break;
		default:
			return;
	}
}

void LmDrawScene::sendHello(Ref* p_Sender)
{
	if (m_pUser->isBParent())
	{
		bytes msg(100);
		msg << LmEvent::GoodAnswer;
		msg.write("hello world");
		WIFIFACADE->sendBytes(msg);

	}
}

void LmDrawScene::onGoodAnswerEvent(bytes msg)
{
	std::string str = msg.readString();
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	auto l_oLabel = Label::createWithTTF(str, "Fonts/JosefinSans-Bold.ttf",
			l_oVisibleSize.height * 0.05);
	l_oLabel->setPosition(l_oVisibleSize.width * 0.5,
			l_oVisibleSize.height * 0.5);
	l_oLabel->setColor(Color3B::BLACK);
	l_oLabel->setMaxLineWidth(l_oVisibleSize.width * 0.8);
	m_pLayerGame->addChild(l_oLabel);

	win(true);

}
