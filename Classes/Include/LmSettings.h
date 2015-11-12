/*
 * LmSettings.h
 *
 *  Created on: 3 nov. 2015
 *      Author: IHMTEKDev4
 */

#ifndef CLASSES_INCLUDE_LMSETTINGS_H_
#define CLASSES_INCLUDE_LMSETTINGS_H_

#include "cocos2d.h"
#include "LmSprite.h"
#include "LmJniCppFacade.h"
#include "SimpleAudioEngine.h"

//to know stats file to choose
#include "LmUser.h"

#include "GUI/CCControlExtension/CCControlExtensions.h"


class LmCredits: public cocos2d::Scene
{
public:

	LmCredits();
	~LmCredits();

	void initCredits();
	void run();

private:


	//ATTRIBUTES
	std::string m_sCreditsText;
	cocos2d::Label* m_pLabel;
	bool m_bLayerTouched;
	cocos2d::EventListenerTouchOneByOne* m_pListener;
	float m_pSpeed;//the speed of the text in px/s

	//METHODS

	bool onTouchBeganCredits(cocos2d::Touch* ,cocos2d::Event* );

	void initText();


};


class LmSettings: public cocos2d::Ref
{
public:

	LmSettings(LmUser*);
	~LmSettings();

	bool init();

	void setVisible(bool);

	void endApplication();

	//to add it to a node
	void addTo(cocos2d::Node*, int);

	void addChild(cocos2d::Node*);

private:

	//ATTRIBUTES
	LmSprite* m_pBackGroundSprite;

	LmCredits* m_pLmCredits;

	LmUser* m_pUser;


	//METHODS

	//callback end button
	void endApplicationCallbackButton(cocos2d::Ref*);

	void back(cocos2d::Ref*);

	void valueChangedCallbackBackgroundVolume(cocos2d::Ref*,
			cocos2d::extension::Control::EventType);

	void valueChangedCallbackEffectVolume(cocos2d::Ref*,
			cocos2d::extension::Control::EventType);

	void runCredits(Ref* p_Sender);

};


#endif /* CLASSES_INCLUDE_LMSETTINGS_H_ */
