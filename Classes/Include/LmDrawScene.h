/*
 * LmDrawScene.h
 *
 *  Created on: 23 nov. 2015
 *      Author: IHMTEKDev4
 */

#ifndef CLASSES_INCLUDE_LMDRAWSCENE_H_
#define CLASSES_INCLUDE_LMDRAWSCENE_H_

#include "LmInteractionScene.h"


struct LmDrawSceneSeed
{

	std::string FilenameSpriteBackground;


};

class LmDrawScene: public LmInteractionScene
{

	public:

		static const int s_iId = 6;

		LmDrawScene(const LmDrawSceneSeed& l_oSeed);
		LmDrawScene();
		~LmDrawScene();

		virtual void onReceivingMsg(bytes);

		void resetScene();
		void restart();

	private:
		LmDrawSceneSeed m_oSeed;


		void runGame();
		bool initGame();

		void sendHello(cocos2d::Ref*);
		void onGoodAnswerEvent(bytes msg);
};

#endif /* CLASSES_INCLUDE_LMDRAWSCENE_H_ */
