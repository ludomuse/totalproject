/*
 * LmStatistics.cpp
 *
 *  Created on: 9 nov. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmStatistics.h"

#include "../Include/LmJsonParser.h"

#include "cocos2d.h"

#define COCOS2D_DEBUG 1

using namespace rapidjson;
USING_NS_CC;

LmStatistics::LmStatistics()
{
	CCLOG("CREATE A LMSTATS OBJECT");

	//it will be push to gamevector
	m_pGameStats = new LmGameStats;
	m_aGameStatsVector.push_back(m_pGameStats);

	//init array time

	//register the begining of the acquisition
	time_t rawtime;
	time(&rawtime);
	std::string buffer = ctime(&rawtime);
	int len = buffer.length();
	buffer = buffer.substr(0, len - 1); //because there is \n at the end
	m_aHistoricBuffer.push_back(buffer.c_str());

	//primitive type
	m_iNumberOfUsers = 1;
	m_bUserIsParent = true; //is parent by default set by lmuser class

}

LmStatistics::~LmStatistics()
{
	//delete all scene and then game struct
	for (int i = 0; i < m_aGameStatsVector.size(); i++)
	{
		for (std::map<int, LmSceneStats*>::iterator it = m_aGameStatsVector.at(
				i)->SceneMap.begin();
				it != m_aGameStatsVector.at(i)->SceneMap.end(); ++it)
		{
			delete it->second;
		}

		delete m_aGameStatsVector.at(i);
	}

}

void LmStatistics::printUserFileToLogcat()
{
	std::string output = FileUtils::getInstance()->getStringFromFile(
			m_sFullPath);

	CCLOG("%s is :\n%s", m_sFullPath.c_str(), output.c_str());
}

std::string LmStatistics::getOutputString()
{
	CCLOG("getOutputString");

	GenericStringBuffer<UTF8<>> buffer;
	Writer<GenericStringBuffer<UTF8<>>> writer(buffer);

	//start writing object
	writer.StartObject();

	//write numbber of user
	writer.Key(USER_NUMBER_KEY);
	writer.Int(m_iNumberOfUsers);

	//write all the date (hack because writer.
	writer.Key(ARRAY_DATE_KEY);
	writer.StartArray();
	for (int i = 0; i < m_aHistoricBuffer.size(); i++)
	{
		writer.String(m_aHistoricBuffer.at(i).c_str());
	}
	writer.EndArray();

	//write info game
	writer.Key(ARRAY_GAME_KEY);
	writer.StartArray();
	//for all games
	for (int i = 0; i < m_aGameStatsVector.size(); i++)
	{
		writer.StartObject();
		//write id + title
		writer.Key(ID_GAME);
		writer.Int(m_aGameStatsVector.at(i)->IdGame);
		writer.Key(TITLE_GAME);
		writer.String(m_aGameStatsVector.at(i)->Title.c_str());

		writer.Key(SCENE_MAP);
		writer.StartArray();
		//for all scene of that game
		for (std::map<int, LmSceneStats*>::iterator it = m_aGameStatsVector.at(
				i)->SceneMap.begin();
				it != m_aGameStatsVector.at(i)->SceneMap.end(); ++it)
		{
			writer.StartObject();

			//id scene
			writer.Key(ID_SCENE);
			writer.Int(it->first);
			writer.Key(DESCRIPTION_SCENE);
			writer.String(it->second->Description.c_str());
			writer.Key(TIME_GAME);
			writer.Int(it->second->TimeGame);
			writer.Key(TIME_INTERACTION);
			writer.Int(it->second->TimeInteraction);
			writer.EndObject();

		}
		writer.EndArray();

		writer.EndObject();

	}
	writer.EndArray();

	//write custom stats
	for (std::map<std::string, int>::iterator it = m_aData.begin();
			it != m_aData.end(); ++it)
	{
		writer.Key(it->first.c_str());
		writer.Int(it->second);
	}

	//end writing
	writer.EndObject();

	return buffer.GetString();
}

void LmStatistics::clicked(std::string tag)
{

	if (m_aData.find(tag) != m_aData.end())
	{
		m_aData.find(tag)->second++;
		CCLOG("%s clicked %d times", tag.c_str(), m_aData.find(tag)->second);
	}
	else
	{
		CCLOG("LmStatistics::clicked this tag doesn't exist => %s",
				tag.c_str());
	}
}

void LmStatistics::writeToJson()
{
	//merge info with the previous file if exist
	mergeWithExistingJson();

	//then write the new json
	FileUtils::getInstance()->writeStringToFile(getOutputString(),
			m_sFullPath.c_str());

}

void LmStatistics::interactionBegin()
{

	CCLOG("LmStatistics::interactionBegin");
	time(&m_aSceneTimeBuffer[0]);

}

void LmStatistics::interactionGameBegin()
{
	CCLOG("LmStatistics::interactionGameBegin");
	time(&m_aSceneTimeBuffer[1]);
}

void LmStatistics::interactionGameEnd()
{
	CCLOG("LmStatistics::interactionGameEnd");
	time(&m_aSceneTimeBuffer[2]);
}

void LmStatistics::interactionEnd()
{
	CCLOG("LmStatistics::interactionEnd");
	time(&m_aSceneTimeBuffer[3]);

}

void LmStatistics::endRecord(int idscene, std::string description)
{
	CCLOG("LmStatistics::endRecord");

	addSceneGameStats(idscene);
	setDescriptionOfScene(idscene, description);

	//put all info into the gameStats object
	m_pGameStats->SceneMap.find(idscene)->second->TimeGame = getTimeDiff(
			m_aSceneTimeBuffer[1], m_aSceneTimeBuffer[2]);

	m_pGameStats->SceneMap.find(idscene)->second->TimeInteraction = getTimeDiff(
			m_aSceneTimeBuffer[0], m_aSceneTimeBuffer[3]);
}

void LmStatistics::init()
{
	CCLOG("LmStatistics::init()");

	if (m_bUserIsParent)
	{
		m_sFullPath =
				FileUtils::getInstance()->getWritablePath() + FILENAME_USER_PARENT;

	}
	else
	{
		m_sFullPath =
				FileUtils::getInstance()->getWritablePath() + FILENAME_USER_CHILD;

	}

	std::vector<std::string> l_aKeyButtonToListen = KEY_BUTTONS;

	//insert what we listen to
	for (int i = 0; i < l_aKeyButtonToListen.size(); i++)
	{
		m_aData.insert( { l_aKeyButtonToListen.at(i).c_str(), 0 });
	}
}

void LmStatistics::importIntoGameStatsVector(
		const rapidjson::Value& l_oArayGame)
{

	CCLOG("import");

	std::string l_sBufferString;

	//stock previous game into struct LmGameStats
	for (int i = 0; i < l_oArayGame.Size(); i++)
	{
		LmGameStats* l_SeedBufferGame = new LmGameStats;

		assert(l_oArayGame[i].IsObject());

		//idgame
		assert(l_oArayGame[i].HasMember(ID_GAME));
		assert(l_oArayGame[i][ID_GAME].IsInt());
		l_SeedBufferGame->IdGame = l_oArayGame[i][ID_GAME].GetInt();

		//title
		assert(l_oArayGame[i].HasMember(TITLE_GAME));
		assert(l_oArayGame[i][TITLE_GAME].IsString());
		l_sBufferString = l_oArayGame[i][TITLE_GAME].GetString();
		l_SeedBufferGame->Title = l_sBufferString.c_str();

		assert(l_oArayGame[i][SCENE_MAP].IsArray());

		for (int j = 0; j < l_oArayGame[i][SCENE_MAP].Size(); j++)
		{

			CCLOG("copy previous scene");

			LmSceneStats* l_SeedBufferScene = new LmSceneStats;

			assert(l_oArayGame[i][SCENE_MAP][j].IsObject());

			//id scene
			assert(l_oArayGame[i][SCENE_MAP][j].HasMember(ID_SCENE));
			assert(l_oArayGame[i][SCENE_MAP][j][ID_SCENE].IsInt());
			l_SeedBufferScene->IdScene =
					l_oArayGame[i][SCENE_MAP][j][ID_SCENE].GetInt();
			CCLOG("after id");

			//description
			assert(l_oArayGame[i][SCENE_MAP][j].HasMember(DESCRIPTION_SCENE));
			assert(l_oArayGame[i][SCENE_MAP][j][DESCRIPTION_SCENE].IsString());
			l_sBufferString =
					l_oArayGame[i][SCENE_MAP][j][DESCRIPTION_SCENE].GetString();

			l_SeedBufferScene->Description = l_sBufferString.c_str();

			//timegame
			assert(l_oArayGame[i][SCENE_MAP][j].HasMember(TIME_GAME));
			assert(l_oArayGame[i][SCENE_MAP][j][TIME_GAME].IsInt());
			l_SeedBufferScene->TimeGame =
					l_oArayGame[i][SCENE_MAP][j][TIME_GAME].GetInt();

			//timeInteraction
			assert(l_oArayGame[i][SCENE_MAP][j].HasMember(TIME_INTERACTION));
			assert(l_oArayGame[i][SCENE_MAP][j][TIME_INTERACTION].IsInt());
			l_SeedBufferScene->TimeInteraction =
					l_oArayGame[i][SCENE_MAP][j][TIME_INTERACTION].GetInt();

			//add the scene to our l_SeedBufferGame
			l_SeedBufferGame->SceneMap.insert( { l_SeedBufferScene->IdScene,
					l_SeedBufferScene });
		}
		CCLOG(" import game with %d scene already recorded index %d",
				l_SeedBufferGame->SceneMap.size(), i);
		m_aGameStatsVector.push_back(l_SeedBufferGame);
	}
}

void LmStatistics::mergeWithExistingJson()
{

	if (FileUtils::getInstance()->isFileExist(m_sFullPath))
	{

		//init json
		std::string l_sJsonString = FileUtils::getInstance()->getStringFromFile(
				m_sFullPath);

		CCLOG("There is a previous record => \n%s", l_sJsonString.c_str());

		Document l_oDocument;
		l_oDocument.Parse<0>(l_sJsonString.c_str());

		CCLOG("Number of user");

		//check there a number of user recorded to update the number of user
		if (l_oDocument.HasMember(USER_NUMBER_KEY))
		{
			assert(l_oDocument[USER_NUMBER_KEY].IsInt());
			//add one
			m_iNumberOfUsers += l_oDocument[USER_NUMBER_KEY].GetInt();
		}

		CCLOG("nb user :%d", m_iNumberOfUsers);

		CCLOG("Date");

		//get all the date
		if (l_oDocument.HasMember(ARRAY_DATE_KEY))
		{
			assert(l_oDocument[ARRAY_DATE_KEY].IsArray());
			std::string bufferString;
			for (int i = 0; i < l_oDocument[ARRAY_DATE_KEY].Size(); i++)
			{
				assert(l_oDocument[ARRAY_DATE_KEY][i].IsString());
				bufferString = l_oDocument[ARRAY_DATE_KEY][i].GetString();
				m_aHistoricBuffer.push_back(bufferString.c_str());
			}

		}

		CCLOG("Game info");

		//get all info by game
		if (l_oDocument.HasMember(ARRAY_GAME_KEY))
		{
			assert(l_oDocument[ARRAY_GAME_KEY].IsArray());
			importIntoGameStatsVector(l_oDocument[ARRAY_GAME_KEY]);
		}

		mergeTimeWithGameStats();

		CCLOG("button stats");

		//merge value of key in common
		for (std::map<std::string, int>::iterator it = m_aData.begin();
				it != m_aData.end(); ++it)
		{
			if (l_oDocument.HasMember(it->first.c_str()))
			{
				CCLOG("%s session : %d", it->first.c_str(), it->second);
				CCLOG("%s json : %d", it->first.c_str(),
						l_oDocument[it->first.c_str()].GetInt());

				assert(l_oDocument[it->first.c_str()].IsInt());
				CCLOG("avant arrondi %f",
						(l_oDocument[it->first.c_str()].GetInt()
								* (m_iNumberOfUsers - 1) + it->second)
								/ m_iNumberOfUsers);

				it->second = ceil(
						(l_oDocument[it->first.c_str()].GetInt()
								* (m_iNumberOfUsers - 1) + it->second)
								/ m_iNumberOfUsers);

				CCLOG("%s after merge : %d", it->first.c_str(), it->second);

			}
		}

	}
}

int LmStatistics::getTimeDiff(time_t time1, time_t time2)
{
//calculate the diff and return the result in a string
	int l_iDiffTime = (int) difftime(time2, time1);

	CCLOG("difftime result = %d", l_iDiffTime);

	return l_iDiffTime;
}

void LmStatistics::mergeTimeWithGameStats()
{
	//merge value of time with our actual game
	for (int i = 0; i < m_aGameStatsVector.size(); i++)
	{
		if (m_aGameStatsVector.at(i) == m_pGameStats)
		{
			//skip
			i++;
		}

		//if this is the same game we merge value into m_gamestats
		if (m_aGameStatsVector.at(i)->IdGame == m_pGameStats->IdGame)
		{

			CCLOG("same game into the previous json");

			CCLOG("we played %d scene on this session",
					m_pGameStats->SceneMap.size());

			//check what scene have been already recorded
			for (std::map<int, LmSceneStats*>::iterator itSceneStats =
					m_pGameStats->SceneMap.begin();
					itSceneStats != m_pGameStats->SceneMap.end();
					itSceneStats++)
			{

				//for each scene we played in that session

				itSceneStats->second->testPrint(1);

				std::map<int, LmSceneStats*>::iterator itGameRecordedScene;
				itGameRecordedScene = m_aGameStatsVector.at(i)->SceneMap.find(
						itSceneStats->first);

				if (itGameRecordedScene
						!= m_aGameStatsVector.at(i)->SceneMap.end())
				{

					itGameRecordedScene->second->testPrint(2);

					//find smthg itGameRecordedScene idscene = itSceneStats idscene

					//merge value
					itSceneStats->second->TimeGame = ceil(
							(itGameRecordedScene->second->TimeGame
									* (m_iNumberOfUsers - 1)
									+ itSceneStats->second->TimeGame)
									/ m_iNumberOfUsers);

					itSceneStats->second->TimeInteraction = ceil(
							(itGameRecordedScene->second->TimeInteraction
									* (m_iNumberOfUsers - 1)
									+ itSceneStats->second->TimeInteraction)
									/ m_iNumberOfUsers);

					//erase the record into the vector recorded record to not write it two time
					m_aGameStatsVector.at(i)->SceneMap.erase(
							itGameRecordedScene);
					CCLOG("erase the %d", i);
				}

			}

			//if there is still element in it put it into our game stats
			if (!m_aGameStatsVector.at(i)->SceneMap.empty())
			{

				for (std::map<int, LmSceneStats*>::iterator itGameRecordedScene =
						m_aGameStatsVector.at(i)->SceneMap.begin();
						itGameRecordedScene
								!= m_aGameStatsVector.at(i)->SceneMap.end();
						++itGameRecordedScene)
				{

					m_pGameStats->SceneMap.insert((*itGameRecordedScene));
				}
			}

			//then remove the previous gamestats recorded from vector this game
			m_aGameStatsVector.erase(m_aGameStatsVector.begin() + i);

		}
	}
}

void LmStatistics::addSceneGameStats(int idscene)
{
	LmSceneStats* l_EmptyScene = new LmSceneStats;
	l_EmptyScene->IdScene = idscene;

	m_pGameStats->SceneMap.insert( { idscene, l_EmptyScene });
}

