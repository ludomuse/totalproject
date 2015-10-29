/*
 * LmAudioHintScene.cpp
 *
 *  Created on: 7 oct. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmAudioHintScene.h"

using namespace cocos2d;

LmAudioHintScene::LmAudioHintScene(const LmAudioHintSceneSeed &l_Seed)
{
	//from json
	m_sFilenameSpriteBackground = l_Seed.FilenameSpriteBackground;
	m_sFilenameSpriteMainImage = l_Seed.FilenameSpriteMainImage;
	m_aLabelsFilenameSprite = l_Seed.LabelsFilenameSprite;
	m_aLabelsFilenameSpriteHint = l_Seed.LabelsFilenameSpriteHint;
	m_aLabelsCoordonateHole = l_Seed.LabelsCoordonateHole;

	//primitive type
	m_bBufferSpriteFillHole = false;
	m_iBufferId = -1;
	m_iHoleTouchedIndex = -1;
	m_bSpriteSelected = false;

	//pointer
	m_pSpriteBackground = nullptr;
	m_pSpriteMainImage = nullptr;
	m_pBufferSprite = nullptr;

}

LmAudioHintScene::~LmAudioHintScene()
{

}

void LmAudioHintScene::restart()
{

}

void LmAudioHintScene::resetScene()
{

}

void LmAudioHintScene::runGame()
{
	if (!initGame())
	{
		CCLOG("initGame failed");
	}

}

bool LmAudioHintScene::initGame()
{
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//init the background
	m_pSpriteBackground = Sprite::create(m_sFilenameSpriteBackground);
	m_pSpriteBackground->setPosition(l_oVisibleSize.width * 0.5f + l_oOrigin.x,
			l_oVisibleSize.height * 0.5f + l_oOrigin.y);
	m_pLayerGame->addChild(m_pSpriteBackground);

	//init main image so we can get his dimension
	m_pSpriteMainImage = Sprite::create(m_sFilenameSpriteMainImage);
	m_pSpriteMainImage->setAnchorPoint(Vec2(0, 0.5));
	m_pSpriteMainImage->setPosition(s_fMarginLeft,
			l_oVisibleSize.height * 0.5f);
	m_pLayerGame->addChild(m_pSpriteMainImage);

	//init buffer sprite
	m_pBufferSprite = Sprite::create();
	m_pLayerGame->addChild(m_pBufferSprite, 1);

	//use to place element
	int l_iIndex = 0;
	LmGameComponent* l_pGameComponentCreated;
	Sprite* l_pSpriteHintCreated;

	//init labels gamecomponent
	for (std::map<int, std::string>::iterator it =
			m_aLabelsFilenameSprite.begin();
			it != m_aLabelsFilenameSprite.end(); ++it)
	{
		//init game component
		l_pGameComponentCreated = makeGameComponent();
		m_aLabelsGameComponent.insert(
		{ it->first, l_pGameComponentCreated });
		l_pGameComponentCreated->initSpriteComponent(it->second);
		//place on the game
		l_pGameComponentCreated->setAnchorPoint(Vec2(1, 0.5));
		l_pGameComponentCreated->setPosition(
				Vec2(l_oVisibleSize.width,
						(l_iIndex + 1) * l_oVisibleSize.height
								/ (m_aLabelsFilenameSprite.size() + 1)));
		l_pGameComponentCreated->addTo(m_pLayerGame);
		l_pGameComponentCreated->setVisible(false);

		//we get coordonnate in percent from json
		Vec2 l_oPosition = Vec2(
				(m_aLabelsCoordonateHole.find(it->first)->second.x
						* m_pSpriteMainImage->getContentSize().width / 100)
						+ s_fMarginLeft,
				((m_aLabelsCoordonateHole.find(it->first)->second.y - 50)
						* m_pSpriteMainImage->getContentSize().height / 100)
						+ l_oVisibleSize.height * 0.5);

		//init hole set aposition according the anchor point
		m_aLabelsHole.insert(
				{ it->first,
						Rect(
								l_oPosition.x
										- l_pGameComponentCreated->getContentSize().width
												* 0.5,
								l_oPosition.y
										- l_pGameComponentCreated->getContentSize().height
												* 0.5,
								l_pGameComponentCreated->getContentSize().width,
								l_pGameComponentCreated->getContentSize().height) });

		//init id label well place to check victory
		m_aIdLabelWellPlaced.insert(
		{ it->first, false });

		l_iIndex++;
	}

	for (std::map<int, std::string>::iterator it =
			m_aLabelsFilenameSpriteHint.begin();
			it != m_aLabelsFilenameSpriteHint.end(); ++it)
	{
		//init sprite hint image

		l_pSpriteHintCreated = Sprite::create(it->second);
		l_pSpriteHintCreated->setAnchorPoint(Vec2(0.5, 0.5));

		//we get coordonnate in percent from json
		Vec2 l_oPosition = Vec2(
				(m_aLabelsCoordonateHole.find(it->first)->second.x
						* m_pSpriteMainImage->getContentSize().width / 100)
						+ s_fMarginLeft,
				((m_aLabelsCoordonateHole.find(it->first)->second.y - 50)
						* m_pSpriteMainImage->getContentSize().height / 100)
						+ l_oVisibleSize.height * 0.5);

		l_pSpriteHintCreated->setPosition(l_oPosition);
		l_pSpriteHintCreated->setVisible(false);

		m_aLabelsSpriteHint.insert(
		{  it->first,l_pSpriteHintCreated });
		m_pLayerGame->addChild(l_pSpriteHintCreated);
	}

	//check what type of user
	if (m_pUser->isBParent())
	{
		//put label visible
		for (std::map<int, LmGameComponent*>::iterator it =
				m_aLabelsGameComponent.begin();
				it != m_aLabelsGameComponent.end(); ++it)
		{
			it->second->setVisible(true);
		}

		//init listener
		m_pListener = EventListenerTouchOneByOne::create();
		m_pListener->onTouchBegan = CC_CALLBACK_2(
				LmAudioHintScene::onTouchBeganParent, this);
		m_pListener->onTouchMoved = CC_CALLBACK_2(
				LmAudioHintScene::onTouchMovedParent, this);
		m_pListener->onTouchEnded = CC_CALLBACK_2(
				LmAudioHintScene::onTouchEndedParent, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
				m_pListener, m_pLayerGame);
	}
	else
	{

		for(std::map<int,Sprite*>::iterator it=m_aLabelsSpriteHint.begin();it!=m_aLabelsSpriteHint.end();++it)
		{
			it->second->setVisible(true);
		}
	}

	return true;
}

//callback method of parent layer
bool LmAudioHintScene::onTouchBeganParent(cocos2d::Touch* touch,
		cocos2d::Event* event)
{
	//to avoid to bein a touch when the previous is not finish
	if (m_bTouchBeganDisabled)
	{
		return false;
	}
	else
	{
		m_bTouchBeganDisabled = true;
	}
	//get id label
	m_iBufferId = idLabel(touch);
	//if something is touched
	if (m_iBufferId >= 0)
	{
		//init buffer gamecomponent set buffer sprite with his attributes and go invisible
		initBufferSprite(m_iBufferId);
		moveBufferSprite(touch);
	}
	else
	{
		m_bSpriteSelected = false;
	}

	return true;
}

void LmAudioHintScene::onTouchMovedParent(cocos2d::Touch* touch,
		cocos2d::Event* event)
{
	if (m_bSpriteSelected)
	{
		moveBufferSprite(touch);

		m_iHoleTouchedIndex = touchCollideHole(touch);

		if (m_iHoleTouchedIndex >= 0)
		{

			auto l_oHoleTouched =
					m_aLabelsHole.find(m_iHoleTouchedIndex)->second;
			//we move the buffer sprite to this position
			m_pBufferSprite->setAnchorPoint(Vec2(0, 0));
			m_pBufferSprite->setPosition(
					Vec2(l_oHoleTouched.origin.x, l_oHoleTouched.origin.y));
			m_bBufferSpriteFillHole = true;
		}
		else
		{
			m_pBufferSprite->setAnchorPoint(Vec2(0.5, 0.5));

			m_bBufferSpriteFillHole = false;
		}

	}
}

void LmAudioHintScene::onTouchEndedParent(cocos2d::Touch* touch,
		cocos2d::Event* event)
{
	if (m_bSpriteSelected)
	{

		//no sprite selected anymore
		m_bSpriteSelected = false;

		//if when we end movement an hole is filled we operate change in vector
		if (m_bBufferSpriteFillHole)
		{

			//check if its good position
			if (m_iHoleTouchedIndex == m_iBufferId)
			{

				//we change position of this gameobject to that hole
				m_aLabelsGameComponent.find(m_iBufferId)->second->setAnchorPoint(
						Vec2(0, 0));
				m_aLabelsGameComponent.find(m_iBufferId)->second->setPosition(
						Vec2(
								m_aLabelsHole.find(m_iHoleTouchedIndex)->second.origin.x,
								m_aLabelsHole.find(m_iHoleTouchedIndex)->second.origin.y));

				m_aIdLabelWellPlaced.find(m_iBufferId)->second = true;

				m_bWin = true;
				//check win
				for (std::map<int, bool>::iterator it =
						m_aIdLabelWellPlaced.begin();
						it != m_aIdLabelWellPlaced.end(); ++it)
				{
					//if one is not well placed
					if (!it->second)
					{
						m_bWin = false;
						break;
					}
				}

				if (m_bWin)
				{
					win(m_bWin);

					bytes msg(10);
					msg << LmEvent::Win;
					msg.write(m_bWin);
					WIFIFACADE->sendBytes(msg);
				}

			}

		}

		//put it invisible
		m_pBufferSprite->setVisible(false);
		//we put the gamecomponent visible again
		m_aLabelsGameComponent.find(m_iBufferId)->second->setVisible(true);

	}
	m_bTouchBeganDisabled = false;
}

int LmAudioHintScene::idLabel(cocos2d::Touch* touch)
{
	auto l_oTouchLocation = touch->getLocation();

	for (std::map<int, LmGameComponent*>::iterator it =
			m_aLabelsGameComponent.begin(); it != m_aLabelsGameComponent.end();
			++it)
	{

		if (it->second->getPSpriteComponent()->getBoundingBox().containsPoint(
				l_oTouchLocation))
		{
			return it->first;
		}
	}
	return -1;
}

void LmAudioHintScene::initBufferSprite(int l_iIdGameComponent)
{
	//the sprite is selected
	m_bSpriteSelected = true;
	//the gamecomponent selected is copy in the buffer
	m_pBufferSprite->setSpriteFrame(
			m_aLabelsGameComponent.find(l_iIdGameComponent)->second->getPSpriteComponent()->getSpriteFrame());
	m_pBufferSprite->setAnchorPoint(Vec2(0.5, 0.5));
	m_pBufferSprite->setPosition(
			m_aLabelsGameComponent.find(l_iIdGameComponent)->second->getPosition());
	m_aLabelsGameComponent.find(l_iIdGameComponent)->second->setVisible(false);
	m_pBufferSprite->setVisible(true);
}

void LmAudioHintScene::moveBufferSprite(Touch* touch)
{
	auto l_oTouchLocation = touch->getLocation();
	m_pBufferSprite->setPosition(Vec2(l_oTouchLocation.x, l_oTouchLocation.y));
}

int LmAudioHintScene::touchCollideHole(Touch* touch)
{
	auto l_oTouchLocation = touch->getLocation();

	for (std::map<int, Rect>::iterator it = m_aLabelsHole.begin();
			it != m_aLabelsHole.end(); ++it)
	{
		if (it->second.containsPoint(l_oTouchLocation))
		{
			return it->first;
		}
	}

	return -1;
}

