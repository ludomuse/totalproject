/*
 * LmSettings.h
 *
 *  Created on: 3 nov. 2015
 *      Author: IHMTEKDev4
 */

#ifndef CLASSES_INCLUDE_LMSETTINGS_H_
#define CLASSES_INCLUDE_LMSETTINGS_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
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
		float m_pSpeed; //the speed of the text in px/s

		//METHODS

		void back(cocos2d::Ref*);

		void initText();

};

class LmStatsView: public cocos2d::Scene
{
	public:

		LmStatsView();
		~LmStatsView();

		void runStats(bool);

		void initStats();

	private:

		std::string m_sTextJson;

		cocos2d::Label* m_pLabel;

		void back(cocos2d::Ref*);

		//parse json string into a displayable string
		std::string parseTextJson();

		cocos2d::ui::ScrollView* m_pScrollView;

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

	private:

		//ATTRIBUTES
		LmSprite* m_pBackGroundSprite;

		LmCredits* m_pLmCredits;

		LmUser* m_pUser;

		LmStatsView* m_pLmStatsView;

		//METHODS

		//callback end button
		void endApplicationCallbackButton(cocos2d::Ref*);

		void back(cocos2d::Ref*);

		void valueChangedCallbackBackgroundVolume(cocos2d::Ref*,
				cocos2d::extension::Control::EventType);

		void valueChangedCallbackEffectVolume(cocos2d::Ref*,
				cocos2d::extension::Control::EventType);

		void runCredits(Ref* p_Sender);

		void displayStatsChild(Ref* p_Sender);
		void displayStatsParent(Ref* p_Sender);

};

#endif /* CLASSES_INCLUDE_LMSETTINGS_H_ */
