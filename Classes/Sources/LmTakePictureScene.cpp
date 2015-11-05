/*
 * LmTakePictureScene.cpp
 *
 *  Created on: 5 nov. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmTakePictureScene.h"

using namespace cocos2d;

LmTakePictureScene::LmTakePictureScene(const LmTakePictureSceneSeed& l_Seed)
{
	m_sFilenameSpriteBackground = l_Seed.FilenameSpriteBackground;
}

LmTakePictureScene::~LmTakePictureScene()
{

}

void LmTakePictureScene::restart()
{

}

void LmTakePictureScene::resetScene()
{

}

void LmTakePictureScene::onReceivingMsg(bytes)
{

}

void LmTakePictureScene::runGame()
{
	initGame();
}

bool LmTakePictureScene::initGame()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//init the background
	auto l_pSpriteBackground = Sprite::create(m_sFilenameSpriteBackground);
	l_pSpriteBackground->setPosition(l_oVisibleSize.width * 0.5f + l_oOrigin.x,
			l_oVisibleSize.height * 0.5f + l_oOrigin.y);
	m_pLayerGame->addChild(l_pSpriteBackground);

	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	m_pLayerGame->addChild(menu,1);

	//take picture button
	auto l_pTakepictureButton = MenuItemImage::create(
			"Ludomuse/GUIElements/playNormal.png",
			"Ludomuse/GUIElements/playPressed.png",
			CC_CALLBACK_1(LmTakePictureScene::takePicture, this));
	l_pTakepictureButton->setPosition(
			Vec2(l_oVisibleSize.width * 0.5, l_oVisibleSize.height * 0.5));
	menu->addChild(l_pTakepictureButton);

	return true;
}

void LmTakePictureScene::takePicture(Ref* p_Sender)
{
	TAKE_PICTURE
}
