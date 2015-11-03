/*
 * LmHintScene.h
 *
 *  Created on: 7 oct. 2015
 *      Author: IHMTEKDev4
 */

#ifndef CLASSES_INCLUDE_LMHINTSCENE_H_
#define CLASSES_INCLUDE_LMHINTSCENE_H_

#include "LmInteractionScene.h"

struct LmHintSceneSeed
{
	std::string FilenameSpriteBackground;
	std::string FilenameSpriteMainImage;
	std::map<int, std::string> LabelsFilenameSpriteHint;
	std::map<int, std::string> LabelsFilenameSprite;
	std::map<int, cocos2d::Vec2> LabelsCoordonateHole;

};

class LmHintScene: public LmInteractionScene
{

public:

	static const int s_iId = 3;

	LmHintScene(const LmHintSceneSeed &);
	~LmHintScene();

	//use in case the user back to the dashboard and we need to restore some stuff
	void restart();

	//use to replay the game
	void resetScene();


private:

	//ATTRIBUTES

	//json
	std::string m_sFilenameSpriteBackground;
	std::string m_sFilenameSpriteMainImage;
	std::map<int, std::string> m_aLabelsFilenameSpriteHint;
	std::map<int, std::string> m_aLabelsFilenameSprite;
	std::map<int, cocos2d::Vec2> m_aLabelsCoordonateHole;

	//gui elements

	cocos2d::Sprite* m_pSpriteBackground;
	cocos2d::Sprite* m_pSpriteMainImage;

	//counter to know victory
	std::map<int,bool> m_aIdLabelWellPlaced;

	//gamecomponents
	std::map<int, cocos2d::Rect> m_aLabelsHole;

	//conversion from id label to gamecomponent
	std::map<int, LmGameComponent*> m_aLabelsGameComponent;

	//sprite hint
	std::map<int,cocos2d::Sprite*> m_aLabelsSpriteHint;

	//sprite icon hint
	std::map<int,cocos2d::Sprite*> m_aLabelsSpriteHintIcon;


	//to know if a sprite has been selected
	bool m_bSpriteSelected;
	//buffer sprite use to simulmlate a move
	cocos2d::Sprite* m_pBufferSprite;
	//the id of the element touched
	int m_iBufferId;
	int m_iHoleTouchedIndex;
	bool m_bBufferSpriteFillHole;


	//METHODS

	//main
	void runGame();

	//init
	bool initGame();

	bool menuItemImagePressed(cocos2d::Ref*);

	//callback method of parent layer
	bool onTouchBeganParent(cocos2d::Touch*, cocos2d::Event*);
	void onTouchMovedParent(cocos2d::Touch*, cocos2d::Event*);
	void onTouchEndedParent(cocos2d::Touch*, cocos2d::Event*);

	bool onTouchBeganChild(cocos2d::Touch*, cocos2d::Event*);
	void onTouchMovedChild(cocos2d::Touch*, cocos2d::Event*);
	void onTouchEndedChild(cocos2d::Touch*, cocos2d::Event*);

	//get id label with touch /!\ not id game component
	int idLabel(cocos2d::Touch* );
	//to know which hint is touched
	int idHintTouched(cocos2d::Touch*);
	//init the buffer sprite & move it with touch
	void initBufferSprite(int );
	void moveBufferSprite(cocos2d::Touch* );

	int touchCollideHole(cocos2d::Touch* );

	//for the child to stop and play musique
	void playCallback(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);





};

#endif /* CLASSES_INCLUDE_LMHINTSCENE_H_ */
