/*
 * LmFindGoodCategoryScene.cpp
 *
 *  Created on: 5 oct. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmFindGoodCategoryScene.h"

using namespace cocos2d;

LmFindGoodCategoryScene::LmFindGoodCategoryScene(
		const LmFindGoodCategorySceneSeed &l_Seed) {
	//from json
	m_sFilenameSpriteBackground = l_Seed.FilenameSpriteBackground;
	m_sFilenameSpriteSendingArea = l_Seed.FilenameSpriteSendingArea;
	m_aImages = l_Seed.Images;
	m_aCategories = l_Seed.Categories;
	m_iNumberOfGoodImages = l_Seed.NumberOfGoodImages;
	m_sFilenameAudioGoodCategory = l_Seed.FilenameAudioGoodCategory;
	m_sFilenameAudioBadCategory = l_Seed.FilenameAudioBadCategory;

	//primitive type
	m_bSpriteSelected = false;
	m_bSendingAreaElementTouched = false;
	m_iBufferId = -1;
	m_fSquareDimension = 0;
	m_iCategoryTouchedIndex = -1;

	//pointer
	m_pSpriteBackground = nullptr;
	m_pSendingAreaElement = nullptr;
	m_pBufferSprite = nullptr;
	m_pListener = nullptr;
	m_pSpriteBackground = nullptr;
	m_pSendingArea = nullptr;
}

LmFindGoodCategoryScene::~LmFindGoodCategoryScene() {

}

void LmFindGoodCategoryScene::restart() {

}

void LmFindGoodCategoryScene::resetScene() {
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	for (std::vector<LmGameComponent*>::iterator it =
			m_aGameComponentImages.begin(); it != m_aGameComponentImages.end();
			++it) {
		(*it)->setVisible(false);
	}

	if (m_pUser->isBParent()) {
		//parent view

		int l_iIndex = 0;

		int numberOfImageInLine = (int) l_oVisibleSize.height
				/ (m_fSquareDimension + s_fMarginBetweenImage);

		int l_iLineIndex = 1;

		//place images
		for (std::vector<LmGameComponent*>::iterator it =
				m_aGameComponentImages.begin();
				it != m_aGameComponentImages.end(); ++it) {
			(*it)->setAnchorPoint(Vec2(0, 0.5));
			(*it)->setVisible(true);
			(*it)->setPosition(
					Vec2(
							l_iIndex
									* ((*it)->getContentSize().width
											+ s_fMarginBetweenImage)
									+ s_fMarginLeft,
							(s_fMarginBetweenImage - m_fSquareDimension)
									+ l_oVisibleSize.height
											* ((float) l_iLineIndex
													/ (float) (numberOfImageInLine))));

			l_iLineIndex++;

			//last line
			if (l_iLineIndex == numberOfImageInLine + 1) {
				l_iLineIndex = 1;
				l_iIndex++;
			}

		}
	} else {
		//child view

	}
}

void LmFindGoodCategoryScene::runGame() {
	if (!initGame()) {
		CCLOG("initGame failed");
	}

	//we preload the sound
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(
			m_sFilenameAudioGoodCategory.c_str());

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(
			m_sFilenameAudioBadCategory.c_str());
}

bool LmFindGoodCategoryScene::initGame() {
	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	//init the background
	m_pSpriteBackground = Sprite::create(m_sFilenameSpriteBackground);
	m_pSpriteBackground->setPosition(l_oVisibleSize.width * 0.5f + l_oOrigin.x,
			l_oVisibleSize.height * 0.5f + l_oOrigin.y);
	m_pLayerGame->addChild(m_pSpriteBackground);

	//init Sending Area
	m_pSendingArea = makeGameComponent();
	m_pSendingArea->initSpriteComponent(m_sFilenameSpriteSendingArea);

	m_oSendingAreaPosition =
			(m_pUser->isBParent()) ?
					Vec2(
							l_oVisibleSize.width
									- m_pSendingArea->getContentSize().width
											* 0.5f + l_oOrigin.x,
							l_oVisibleSize.height * 0.5)

							:
					Vec2(
							m_pSendingArea->getContentSize().width * 0.5f
									+ l_oOrigin.x, l_oVisibleSize.height * 0.5);

	m_pSendingArea->setPosition(m_oSendingAreaPosition);

	m_pSendingArea->addTo(m_pLayerGame);

	//init buffer sprite
	m_pBufferSprite = Sprite::create();
	m_pLayerGame->addChild(m_pBufferSprite, 1);

	//usefull variable
	Size l_oPlayableScreenSize = Size(
			l_oOrigin.x - s_fMarginLeft
					+ (l_oVisibleSize.width
							- m_pSendingArea->getContentSize().width),
			l_oVisibleSize.height);

	LmGameComponent* l_pBufferGameComponent;

	//create images of the game
	for (std::vector<std::pair<int, std::string>>::iterator it =
			m_aImages.begin(); it != m_aImages.end(); ++it) {
		//create the component
		l_pBufferGameComponent = makeGameComponent();
		//add it to the vector of all images
		m_aGameComponentImages.push_back(l_pBufferGameComponent);
		//init with filename json
		l_pBufferGameComponent->initSpriteComponent(it->second);
		//register into the map
		m_aIdGamecomponentToIdCategory.insert( {
				l_pBufferGameComponent->getIId(), it->first });
		l_pBufferGameComponent->setVisible(false);
		l_pBufferGameComponent->addTo(m_pLayerGame);

	}

	//create categories of the game
	for (std::map<int, std::pair<std::string, std::string>>::iterator it =
			m_aCategories.begin(); it != m_aCategories.end(); ++it) {
		//create the component
		l_pBufferGameComponent = makeGameComponent();
		//init with filename json
		l_pBufferGameComponent->initSpriteComponent(it->second.first);
		m_aCategoriesElement.insert( { it->first, l_pBufferGameComponent });
		l_pBufferGameComponent->setVisible(false);
		l_pBufferGameComponent->addTo(m_pLayerGame);

	}

	m_fSquareDimension = m_aGameComponentImages.at(0)->getContentSize().height;

	if (m_pUser->isBParent()) {
		//parent view

		int l_iIndex = 0;

		if (!(m_aGameComponentImages.size() > 0)) {
			CCLOG("No Images found");
		}

		/*
		 * check height of piece to know how many to display in a row /!\ get the dimension of the first one
		 * meaning every images have to be the same size		 *
		 */
		int numberOfImageInLine = (int) l_oVisibleSize.height
				/ (m_fSquareDimension + s_fMarginBetweenImage);

		if (numberOfImageInLine == 0) {
			CCLOG("Image doesn't fit on the screen");
		}

		int l_iLineIndex = 1;

		//place images
		for (std::vector<LmGameComponent*>::iterator it =
				m_aGameComponentImages.begin();
				it != m_aGameComponentImages.end(); ++it) {
			(*it)->setAnchorPoint(Vec2(0, 0.5));
			(*it)->setVisible(true);
			(*it)->setPosition(
					Vec2(
							l_iIndex
									* ((*it)->getContentSize().width
											+ s_fMarginBetweenImage)
									+ s_fMarginLeft,
							(s_fMarginBetweenImage - m_fSquareDimension)
									+ l_oVisibleSize.height
											* ((float) l_iLineIndex
													/ (float) (numberOfImageInLine))));
			//create hole associated
			m_aHolesLayerParent.push_back(
					{ Rect((*it)->getPosition().x, (*it)->getPosition().y,
							m_fSquareDimension, m_fSquareDimension),
							(*it)->getIId() });

			l_iLineIndex++;

			//last line
			if (l_iLineIndex == numberOfImageInLine + 1) {
				l_iLineIndex = 1;
				l_iIndex++;
			}

		}

		//init listener
		m_pListener = EventListenerTouchOneByOne::create();
		m_pListener->onTouchBegan = CC_CALLBACK_2(
				LmFindGoodCategoryScene::onTouchBeganParent, this);
		m_pListener->onTouchMoved = CC_CALLBACK_2(
				LmFindGoodCategoryScene::onTouchMovedParent, this);
		m_pListener->onTouchEnded = CC_CALLBACK_2(
				LmFindGoodCategoryScene::onTouchEndedParent, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
				m_pListener, m_pLayerGame);
	} else {
		//child view

		//init categories element
		for (std::map<int, LmGameComponent*>::iterator it =
				m_aCategoriesElement.begin(); it != m_aCategoriesElement.end();
				++it) {
			it->second->setAnchorPoint(Vec2(0.5, 0.5));

			switch (it->first) {
			case 0:
				//top left
				it->second->setPosition(
						Vec2(
								l_oPlayableScreenSize.width * 0.25
										+ s_fMarginLeft
										+ m_pSendingArea->getContentSize().width,
								l_oPlayableScreenSize.height * 0.75));
				break;
			case 1:
				//top right
				it->second->setPosition(
						Vec2(
								l_oPlayableScreenSize.width * 0.75
										+ s_fMarginLeft
										+ m_pSendingArea->getContentSize().width,
								l_oPlayableScreenSize.height * 0.75));
				break;
			case 2:
				//bot left
				it->second->setPosition(
						Vec2(
								l_oPlayableScreenSize.width * 0.25
										+ s_fMarginLeft
										+ m_pSendingArea->getContentSize().width,
								l_oPlayableScreenSize.height * 0.25));
				break;
			case 3:
				//bot right
				it->second->setPosition(
						Vec2(
								l_oPlayableScreenSize.width * 0.75
										+ s_fMarginLeft
										+ m_pSendingArea->getContentSize().width,
								l_oPlayableScreenSize.height * 0.25));
				break;
			default:
				break;
			}

			it->second->setVisible(true);

		}

		//init listener
		m_pListener = EventListenerTouchOneByOne::create();
		m_pListener->onTouchBegan = CC_CALLBACK_2(
				LmFindGoodCategoryScene::onTouchBeganChild, this);
		m_pListener->onTouchMoved = CC_CALLBACK_2(
				LmFindGoodCategoryScene::onTouchMovedChild, this);
		m_pListener->onTouchEnded = CC_CALLBACK_2(
				LmFindGoodCategoryScene::onTouchEndedChild, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
				m_pListener, m_pLayerGame);
	}

	return true;
}

bool LmFindGoodCategoryScene::onTouchBeganParent(cocos2d::Touch* touch,
		cocos2d::Event* event) {
	//to avoid to bein a touch when the previous is not finish
	if (m_bTouchBeganDisabled) {
		return false;
	} else {
		m_bTouchBeganDisabled = true;
	}

	m_iBufferId = idImage(touch);

	//if something is touched
	if (m_iBufferId >= 0) {
		//check if its the sending area element
		if (m_pSendingAreaElement) {
			if (m_pSendingAreaElement->getIId() == m_iBufferId) {
				m_bSendingAreaElementTouched = true;
			} else {
				m_bSendingAreaElementTouched = false;
			}
		}

		//init buffer gamecomponent set buffer sprite with his attributes and go invisible
		initBufferSprite(m_iBufferId);
		moveBufferSprite(touch);
	} else {
		m_bSpriteSelected = false;
	}
	return true;
}

void LmFindGoodCategoryScene::onTouchMovedParent(cocos2d::Touch* touch,
		cocos2d::Event* event) {
	if (m_bSpriteSelected) {

		moveBufferSprite(touch);
	}
}

void LmFindGoodCategoryScene::onTouchEndedParent(cocos2d::Touch* touch,
		cocos2d::Event* event) {

	bool l_bThereIsElementInSendingArea = false;

	if (m_bSpriteSelected) {
		//no sprite selected anymore
		m_bSpriteSelected = false;

		//if the move end on the sending area and it's not the sending area element
		if (bufferCollideSendingArea() && !m_bSendingAreaElementTouched) {

			CCLOG("we are sending %d", m_iBufferId);
			bytes msg(20);
			msg << LmEvent::Gamecomponent;
			msg.write(m_iBufferId);
			WIFIFACADE->sendBytes(msg);

			//move gamecomponent
			setPositionInSendingArea(m_iBufferId);

			//if there was already an element in the sending area
			if (m_pSendingAreaElement) {
				replaceSendingAreaElementToHisOriginalPlace();
			}

			m_pSendingAreaElement = m_aIdTable.find(m_iBufferId)->second;

			//we block touch till we receive a response from the child
			l_bThereIsElementInSendingArea = true;

		} else if (m_bSendingAreaElementTouched
				&& !bufferCollideSendingArea()) {

			//we can't touch the sending area element
			replaceSendingAreaElementToHisOriginalPlace();
		}

		m_bSendingAreaElementTouched = false;

		//put it invisible
		m_pBufferSprite->setVisible(false);

		//we put the gamecomponent visible again
		m_aIdTable.find(m_iBufferId)->second->setVisible(true);

	}

	if (!l_bThereIsElementInSendingArea) {
		m_bTouchBeganDisabled = false;
	}
}

bool LmFindGoodCategoryScene::onTouchBeganChild(cocos2d::Touch* touch,
		cocos2d::Event* event) {
	//to avoid to bein a touch when the previous is not finish
	if (m_bTouchBeganDisabled) {
		return false;
	} else {
		m_bTouchBeganDisabled = true;
	}

	m_iBufferId = idLmGameComponentTouchedInSendingArea(touch);

	//if something is touched
	if (m_iBufferId >= 0) {
		//init buffer gamecomponent set buffer sprite with his attributes and go invisible
		initBufferSprite(m_iBufferId);
		moveBufferSprite(touch);
	} else {
		m_bSpriteSelected = false;
	}

	return true;
}
void LmFindGoodCategoryScene::onTouchMovedChild(cocos2d::Touch* touch,
		cocos2d::Event* event) {
	if (m_bSpriteSelected) {

		moveBufferSprite(touch);

		//get the id of the category touched
		m_iCategoryTouchedIndex = touchCollideCategory();

	}
}
void LmFindGoodCategoryScene::onTouchEndedChild(cocos2d::Touch* touch,
		cocos2d::Event* event) {
	if (m_bSpriteSelected) {

		//no sprite selected anymore
		m_bSpriteSelected = false;

		//check if its good position
		if (m_iCategoryTouchedIndex
				== m_aIdGamecomponentToIdCategory.find(m_iBufferId)->second) {

			//indicate to parent that it's well placed
			bytes msg(20);
			msg << LmEvent::GamecomponentWellPlaced;
			msg.write(m_iBufferId);
			msg.write(true);
			WIFIFACADE->sendBytes(msg);

			//change the sprite of the category to the second one
			m_aCategoriesElement.at(m_iCategoryTouchedIndex)->setTexture(
					m_aCategories.find(m_iCategoryTouchedIndex)->second.second);

			m_iNumberOfGoodImages--;

			//play a sound from the json
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
					m_sFilenameAudioGoodCategory.c_str(), false);

			//check win
			if (m_iNumberOfGoodImages == 0) {
				win(true);
			}

		} else {

			//send back to the void
			bytes msg(20);
			msg << LmEvent::GamecomponentWellPlaced;
			msg.write(m_iBufferId);
			msg.write(false);
			WIFIFACADE->sendBytes(msg);

			CCLOG("bad placed");

			//play a sound from the json
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
					m_sFilenameAudioBadCategory.c_str(), false);

		}

		//make it disapear
		m_pSendingAreaElement->setVisible(false);

		//free sending area
		m_pSendingAreaElement = nullptr;

		//put it invisible
		m_pBufferSprite->setVisible(false);

	}

	m_bTouchBeganDisabled = false;

}

int LmFindGoodCategoryScene::idImage(cocos2d::Touch* touch) {
	auto l_oTouchLocation = touch->getLocation();

	for (std::vector<LmGameComponent*>::iterator it =
			m_aGameComponentImages.begin(); it != m_aGameComponentImages.end();
			++it) {

		if ((*it)->getPSpriteComponent()->getBoundingBox().containsPoint(
				l_oTouchLocation)) {
			return (*it)->getIId();
		}
	}
	return -1;
}

int LmFindGoodCategoryScene::idLmGameComponentTouchedInSendingArea(
		Touch* touch) {
	if (m_pSendingAreaElement) {
		auto l_oTouchLocation = touch->getLocation();

		if (m_pSendingAreaElement->getPSpriteComponent()->getBoundingBox().containsPoint(
				l_oTouchLocation)) {
			return m_pSendingAreaElement->getIId();
		}
	} else {
		CCLOG("sending area empty");
	}

	return -1;
}

void LmFindGoodCategoryScene::initBufferSprite(int l_iIdGameComponent) {
	//the sprite is selected
	m_bSpriteSelected = true;
	//the gamecomponent selected is copy in the buffer

	m_pBufferSprite->setSpriteFrame(
			m_aIdTable.find(l_iIdGameComponent)->second->getPSpriteComponent()->getSpriteFrame());
	m_pBufferSprite->setAnchorPoint(Vec2(0.5, 0.5));
	m_pBufferSprite->setPosition(
			m_aIdTable.find(l_iIdGameComponent)->second->getPosition());
	m_aIdTable.find(l_iIdGameComponent)->second->setVisible(false);
	m_pBufferSprite->setVisible(true);
}

void LmFindGoodCategoryScene::moveBufferSprite(Touch* touch) {
	auto l_oTouchLocation = touch->getLocation();
	m_pBufferSprite->setPosition(Vec2(l_oTouchLocation.x, l_oTouchLocation.y));
}

bool LmFindGoodCategoryScene::bufferCollideSendingArea() {
	return m_pBufferSprite->getBoundingBox().intersectsRect(
			m_pSendingArea->getPSpriteComponent()->getBoundingBox());
}

void LmFindGoodCategoryScene::layerChildReceive(int l_iIdLmGameComponent) {
	//we get the gamecomponent with id
	auto l_pGameComponentReceived =
			m_aIdTable.find(l_iIdLmGameComponent)->second;

	//put it approximatly in the sending area(
	setPositionInSendingArea(l_iIdLmGameComponent);
	m_pSendingAreaElement = l_pGameComponentReceived;
	l_pGameComponentReceived->setVisible(true);

}

void LmFindGoodCategoryScene::setPositionInSendingArea(int id) {

	//use to place elements
	Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
	Point l_oOrigin = Director::getInstance()->getVisibleOrigin();

	auto l_pComponent = m_aIdTable.find(id)->second;
	l_pComponent->setAnchorPoint(Vec2(0.5, 0.5));
	l_pComponent->setPosition(m_oSendingAreaPosition);

}

Rect LmFindGoodCategoryScene::holeOfThisImage(int l_iIdGamecomponent) {
	for (int i = 0; i < m_aHolesLayerParent.size(); i++) {
		if (m_aHolesLayerParent.at(i).second == l_iIdGamecomponent) {
			return m_aHolesLayerParent.at(i).first;
		}
	}

	CCLOG("error no hole found");
	return Rect::ZERO;
}

int LmFindGoodCategoryScene::touchCollideCategory() {

	for (std::map<int, LmGameComponent*>::iterator it =
			m_aCategoriesElement.begin(); it != m_aCategoriesElement.end();
			++it) {
		if (m_pBufferSprite->getBoundingBox().intersectsRect(
				it->second->getPSpriteComponent()->getBoundingBox())) {
			//buffer sprite touched smthg
			return it->first;
		}
	}

	return -1;

}

void LmFindGoodCategoryScene::replaceSendingAreaElementToHisOriginalPlace() {

	//place to his last position
	auto m_oHoleToFill = holeOfThisImage(m_iBufferId);
	m_pSendingAreaElement->setAnchorPoint(Vec2(0, 0.5));
	m_pSendingAreaElement->setPosition(
			Vec2(m_oHoleToFill.origin.x, m_oHoleToFill.origin.y));

	//remove the element of the sending area
	m_pSendingAreaElement = nullptr;
}

void LmFindGoodCategoryScene::onReceivingMsg(bytes l_oMsg) {

	LmInteractionScene::onReceivingMsg(l_oMsg);

	CCLOG("LmFindGoodCategoryScene _event is %d", LmWifiObserver::_event);
	switch (LmWifiObserver::_event) {
	case LmEvent::Gamecomponent:
		CCLOG("Gamecomponent");
		ON_CC_THREAD(LmFindGoodCategoryScene::onGamecomponentEvent, this,
				l_oMsg)
		;
		break;
	case LmEvent::GamecomponentWellPlaced:
		CCLOG("GamecomponentWellPlaced");
		ON_CC_THREAD(LmFindGoodCategoryScene::onGamecomponentWellPlacedEvent,
				this, l_oMsg)
		;
		break;
	default:
		break;
	}

}

void LmFindGoodCategoryScene::onGamecomponentEvent(bytes l_oMsg) {

	int idGameComponent = l_oMsg.readInt();
	layerChildReceive(idGameComponent);
}

void LmFindGoodCategoryScene::onGamecomponentWellPlacedEvent(bytes l_oMsg) {
	int l_iIdGameComponent = l_oMsg.readInt();
	bool l_bWellPlaced = l_oMsg.readBool();

	if (l_bWellPlaced) {

		//erase it of the elements that can be touched
		m_aGameComponentImages.erase(
				std::remove(m_aGameComponentImages.begin(),
						m_aGameComponentImages.end(),
						m_aIdTable.find(l_iIdGameComponent)->second),
				m_aGameComponentImages.end());

		//make disapear this element
		m_aIdTable.find(l_iIdGameComponent)->second->setVisible(false);

		m_pSendingAreaElement = nullptr;

		m_iNumberOfGoodImages--;

		//check win
		if (m_iNumberOfGoodImages == 0) {
			win(true);
		}

	} else {
		//replace this element in the list
		replaceSendingAreaElementToHisOriginalPlace();
	}

	m_bTouchBeganDisabled = false;

}

