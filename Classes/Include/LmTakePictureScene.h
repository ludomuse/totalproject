/*
 * LmTakePictureScene.h
 *
 *  Created on: 5 nov. 2015
 *      Author: IHMTEKDev4
 */

#ifndef CLASSES_INCLUDE_LMTAKEPICTURESCENE_H_
#define CLASSES_INCLUDE_LMTAKEPICTURESCENE_H_

#include "LmInteractionScene.h"

struct LmTakePictureSceneSeed
{
	std::string FilenameSpriteBackground;

};

class LmTakePictureScene : public LmInteractionScene
{
public:

	static const int s_iId = 5;

	LmTakePictureScene(const LmTakePictureSceneSeed&);
	~LmTakePictureScene();

	//use in case the user back to the dashboard and we need to restore some stuff
	void restart();

	//use to replay the game
	void resetScene();

	//dispatcher
	virtual void onReceivingMsg(bytes);



private:

	//ATTRIBUTES

	//json
	std::string m_sFilenameSpriteBackground;

	//buffer sprite photo
	cocos2d::Sprite* m_pBufferSpritePhoto;

	//METHODS

	//main
	void runGame();

	//init
	bool initGame();

	void takePicture(cocos2d::Ref*);

	void notifyPictureIsTaken();

};



#endif /* CLASSES_INCLUDE_LMTAKEPICTURESCENE_H_ */
