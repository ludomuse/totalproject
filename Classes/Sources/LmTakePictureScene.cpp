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

	//pointer
	m_pBufferSpritePhoto = nullptr;
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
	m_pLayerGame->addChild(menu, 1);

	if (m_pUser->isBParent())
	{
		//take picture button
		auto l_pTakepictureButton = MenuItemImage::create(
				"Ludomuse/GUIElements/playNormal.png",
				"Ludomuse/GUIElements/playPressed.png",
				CC_CALLBACK_1(LmTakePictureScene::takePicture, this));
		l_pTakepictureButton->setAnchorPoint(Vec2(1, 0.5));
		l_pTakepictureButton->setPosition(
				Vec2(l_oVisibleSize.width,
						l_oVisibleSize.height * 0.5 + l_oOrigin.y));
		menu->addChild(l_pTakepictureButton);
	}

	m_pBufferSpritePhoto = Sprite::create();
	m_pBufferSpritePhoto->setAnchorPoint(Vec2(0, 0.5));
	m_pBufferSpritePhoto->setPosition(
			Vec2(s_fMarginLeft, l_oVisibleSize.height * 0.5));
	m_pBufferSpritePhoto->setVisible(false);
	l_pSpriteBackground->addChild(m_pBufferSpritePhoto);

	//listen to custom event picture taken
	auto PictureTaken =
			[=](EventCustom * event)
			{

				CCLOG("picture has been take");
				ON_CC_THREAD(LmTakePictureScene::setFileToSprite, this,LmJniCppFacade::getCurrentPicturePath());

				CCLOG("file sent");
				//send it to the other player
				WIFIFACADE->sendFile(LmJniCppFacade::getCurrentPicturePath());

				/*bytes msg(10);
				msg << LmEvent::Win;
				msg.write(true);
				WIFIFACADE->sendBytes(msg);*/
			};

	//add the custom event to the event dispatcher
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(
			"PictureTaken", PictureTaken);


	return true;
}

void LmTakePictureScene::takePicture(Ref* p_Sender)
{
	//bytes msg(10);
	//msg << LmEvent::Win;
	//msg.write(true);
	//WIFIFACADE->sendBytes(msg);


	TAKE_PICTURE;
}

void LmTakePictureScene::onReceivingFile(std::string pathFile)
{
	CCLOG("LmTakePictureScene::onReceivingFile : %s", pathFile.c_str());
	ON_CC_THREAD(LmTakePictureScene::setFileToSprite, this, pathFile);
}

void LmTakePictureScene::setFileToSprite(std::string pathFile)
{
	m_pBufferSpritePhoto->setTexture(pathFile);

	//resize it
	Size l_oSizePhoto = m_pBufferSpritePhoto->getContentSize();
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();

	if (l_oSizePhoto.height > l_oVisibleSize.height * 0.7)
	{
		float scalefactor = l_oVisibleSize.height * 0.7 / l_oSizePhoto.height;

		m_pBufferSpritePhoto->setScaleX(scalefactor);
		m_pBufferSpritePhoto->setScaleY(scalefactor);
	}

	m_pBufferSpritePhoto->setVisible(true);
}

