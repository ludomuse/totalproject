/*
 * LmStatistics.h
 *
 *  Created on: 9 nov. 2015
 *      Author: IHMTEKDev4
 */

#ifndef CLASSES_INCLUDE_LMSTATISTICS_H_
#define CLASSES_INCLUDE_LMSTATISTICS_H_

#include "cocos2d.h"

#include "../../cocos2d/external/json/document.h"
#include "../../cocos2d/external/json/stringbuffer.h"
#include "../../cocos2d/external/json/writer.h"

#include <map>
#include <time.h>

#define FILENAME_USER_CHILD "userStatsChild.json"
#define FILENAME_USER_PARENT "userStatsParent.json"

#define USER_NUMBER_KEY "Nombre d'Utilisateur"
#define ARRAY_DATE_KEY "Historique d'Utilisation"
#define ARRAY_GAME_KEY "Listes des Jeux"
#define ID_GAME "Identifiant jeux"
#define TITLE_GAME "Titre du jeux"
#define SCENE_MAP "Liste des mini-jeux"
#define ID_SCENE "Identifiant mini-jeux"
#define DESCRIPTION_SCENE "Description"
#define TIME_GAME "Temps dans le jeux (sec)"
#define TIME_INTERACTION "Temps (jeux + consigne) (sec)"

struct LmSceneStats
{
		int IdScene;
		std::string Description;
		int TimeGame;
		int TimeInteraction;

		void testPrint(int tag)
		{
			CCLOG("test No %d",tag);
			CCLOG("IdScene = %d , TimeGame = %d TimeInteraction = %d",IdScene,TimeGame,TimeInteraction);
		}
};

struct LmGameStats
{
		int IdGame;
		std::string Title;
		std::map<int, LmSceneStats*> SceneMap; //idscene is the key
};

class LmStatistics
{

	public:

		LmStatistics();
		~LmStatistics();

		//parameter = button tag
		void clicked(std::string);

		void printUserFileToLogcat();

		std::string getOutputString();

		void writeToJson();

		void setRoleUserForStats(bool IsParent)
		{
			m_bUserIsParent = IsParent;
		}

		//set id game from json
		void setIdGameForStats(int idgame)
		{
			CCLOG("setIdGameForStats %d",idgame);
			m_pGameStats->IdGame = idgame;
		}

		//get the description for stats
		void setGameTitle(std::string title)
		{
			CCLOG("setGameTitle");
			m_pGameStats->Title = title;
		}

		//to get time of interaction
		void interactionBegin();
		void interactionGameBegin();
		void interactionGameEnd();
		void interactionEnd();

		//end recording info of the scene
		void endRecord(int,std::string);

		//init
		void init();

	private:

		//ATTRIBUTES

		//game stats & info
		LmGameStats* m_pGameStats;

		//where is stocked info about button
		std::map<std::string, int> m_aData;

		//to know from the json how many people did it
		int m_iNumberOfUsers;

		//the path of where we write file
		std::string m_sFullPath;

		//4 time to know for each scene (number of the game + his time)
		time_t m_aSceneTimeBuffer[4];

		//where we stock info
		std::vector<LmGameStats*> m_aGameStatsVector;

		//historic buffer of other date
		std::vector<std::string> m_aHistoricBuffer;

		//id game - id scene

		//to choose file
		bool m_bUserIsParent;

		//METHODS

		//get if exist the existing json and merge info with it
		void mergeWithExistingJson();

		//extact game info and init m_pGameStats
		void importIntoGameStatsVector(const rapidjson::Value&);

		//return a string of the diff beetween this two time
		int getTimeDiff(time_t, time_t);

		//init scenevector from json
		void addSceneGameStats(int);

		void mergeTimeWithGameStats();

		//get the description for stats
		void setDescriptionOfScene(int idscene, std::string description)
		{
			CCLOG("setDescriptionOfScene");
			m_pGameStats->SceneMap.find(idscene)->second->Description =
					description;
		}

};

#endif /* CLASSES_INCLUDE_LMSTATISTICS_H_ */
