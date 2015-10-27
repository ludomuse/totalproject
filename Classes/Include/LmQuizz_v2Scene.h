/*
 * LmQuizz_v2Scene.h
 *
 *  Created on: 15 oct. 2015
 *      Author: IHMTEKDev4
 */

#ifndef CLASSES_INCLUDE_LMQUIZZ_V2SCENE_H_
#define CLASSES_INCLUDE_LMQUIZZ_V2SCENE_H_


#include "LmInteractionScene.h"
#include "ui/CocosGUI.h"
#include "LmQuestion.h"

struct LmQuizz_v2SceneSeed
{

	std::string FilenameSpriteBackground;
	std::string FilenameSpriteBandTop;
	std::string FilenameSpriteAnswerBackground;
	std::string FilenameSpriteAnswerCross;
	std::string FilenameAudioGoodAnswer;
	std::string FilenameAudioBadAnswer;
	std::vector<LmQuestion*> QuestionsChild;
	std::vector<LmQuestion*> QuestionsParent;
	int AttemptByQuestion;
	float TimerDuration;
	bool TimerEnbaled;

};

class LmQuizz_v2Scene: public LmInteractionScene
{
public:

	//id of this scene
	static const int s_iId =4;

	LmQuizz_v2Scene(const LmQuizz_v2SceneSeed &);
	~LmQuizz_v2Scene();

	//restart
	void restart();

	//to reset the game
	void resetScene();

private:

	//ATTRIBUTES

	//json parameters
	std::string m_sFilenameSpriteBackground;
	std::string m_sFilenameSpriteBandTop;
	std::string m_sFilenameSpriteAnswerBackground;
	std::string m_sFilenameSpriteAnswerCross;
	std::string m_sFilenameSpriteGoodAnswerButton;
	std::string m_sFilenameSpriteBadAnswerButton;
	std::string m_sFilenameAudioGoodAnswer;
	std::string m_sFilenameAudioBadAnswer;


	std::vector<LmQuestion*> m_aQuestionsChild;
	std::vector<LmQuestion*> m_aQuestionsParent;

	int m_iAttemptByQuestion;
	float m_fTimerDuration; //time between each percent of the timer bar
	//if the timer is enabled
	bool m_bTimerEnbaled;

	//gui elements

	//use to force checkbox to be checked when question finish
	bool m_bQuestionFinished;

	cocos2d::Sprite* m_pBandTopSprite;

	//label question
	cocos2d::Label* m_pQuestionLabel;

	//answer parent
	cocos2d::Label* m_pAnswerLabel[4];
	//answer child
	cocos2d::Sprite* m_pAnswerSprite[4];

	//menu item image & menu
	cocos2d::ui::CheckBox* m_pCheckBoxAnswer[4];

	cocos2d::Sprite* m_pSpriteBackground;

	int m_iAnswerSelected;
	//use to place element
	float m_fHeight;

	//timer
	cocos2d::ui::LoadingBar* m_pTimer;
	//timer counter
	int m_iCounter;

	//index of question
	int m_iIndexQuestion;

	//to go to the next question
	cocos2d::ui::Button* m_pNextQuestionButton;
	bool m_bNextQuestionButtonCanBePress;

	//buffer to know how attempt remain
	int m_iNumberOfAttempt;

	//METHODS

	//inherit method main of the scene
	void runGame();
	//init method
	bool initGame();

	//timer
	void updateLoadingBar(float);
	void timerEnd(float);

	//utility method
	void beginQuestion();

	//init label and incrtement index for the next question
	void initNextQuestion();

	//callback method of answer box
	void answerSelected(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType);

	//to select checkbox and answer slected
	void select(int, bool);

	//check the answer
	void checkAnswer();

	//call when question is finish
	void questionFinish(bool);

	//enbaled touch checkbox question
	void checkBoxTouchEnabled(bool);

	//init label for child
	void initAnswerLabel();


};

#endif /* CLASSES_INCLUDE_LMQUIZZ_V2SCENE_H_ */
