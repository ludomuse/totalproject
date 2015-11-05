/*
 * LmJsonParser.cpp
 *
 *  Created on: 3 sept. 2015
 *      Author: IHMTEKDev4
 */

//use for the debug
#define COCOS2D_DEBUG 1

#include "../Include/LmJsonParser.h"

using namespace std;
using namespace cocos2d;

LmJsonParser::LmJsonParser()
{
	//primitive type
	m_sTitleApplication = "";
	//default value
	m_bIsParent = true;
}

LmJsonParser::~LmJsonParser()
{
}

bool LmJsonParser::initJsonDocument(string l_sFilename)
{

	string l_sJsonString = FileUtils::getInstance()->getStringFromFile(
			l_sFilename);
	m_oDocument.Parse<0>(l_sJsonString.c_str());

	//check if the json file has a ludomuse object and an array of scenes
	if (m_oDocument.HasMember("Ludomuse"))
	{
		return true;
	}
	else
	{
		return false;
	}

}

std::string LmJsonParser::getStringValue(const char* key)
{
	std::string result;

	assert(m_oDocument["Ludomuse"].HasMember(key));
	assert(m_oDocument["Ludomuse"][key].IsString());

	result = m_oDocument["Ludomuse"][key].GetString();

	return result.c_str();
}

const std::vector<LmInteractionScene*>& LmJsonParser::getAInteractionSceneOfTheGame(
		bool l_bIsParent)
{
	m_bIsParent = l_bIsParent;

	if (!m_aInteractionSceneOfTheGame.size())
	{
		initInteractionAttributesSceneOfTheGame();
	}

	return m_aInteractionSceneOfTheGame;
}

std::string LmJsonParser::getSTitleApplication()
{
	if (m_sTitleApplication == "")
	{
		//init the string
		assert(
				m_oDocument["Ludomuse"]["Configuration"].IsObject()
						&& m_oDocument["Ludomuse"].HasMember("Configuration"));
		assert(
				m_oDocument["Ludomuse"]["Configuration"]["Title"].IsString()
						&& m_oDocument["Ludomuse"]["Configuration"].HasMember(
								"Title"));

		string l_sBuffer =
				m_oDocument["Ludomuse"]["Configuration"]["Title"].GetString();

		m_sTitleApplication = l_sBuffer.c_str();

	}

	return m_sTitleApplication;
}

std::string LmJsonParser::getSFilenameSpriteSplashScreen()
{
	if (m_sFilenameSpriteSplashScreen == "")
	{
		//init the string
		assert(
				m_oDocument["Ludomuse"]["Configuration"].IsObject()
						&& m_oDocument["Ludomuse"].HasMember("Configuration"));
		assert(
				m_oDocument["Ludomuse"]["Configuration"]["FilenameSpriteSplashScreen"].IsString()
						&& m_oDocument["Ludomuse"]["Configuration"].HasMember(
								"FilenameSpriteSplashScreen"));

		string l_sBuffer =
				m_oDocument["Ludomuse"]["Configuration"]["FilenameSpriteSplashScreen"].GetString();

		m_sFilenameSpriteSplashScreen = l_sBuffer.c_str();

	}

	return m_sFilenameSpriteSplashScreen;
}

LmLayer* LmJsonParser::makeLmLayer(const rapidjson::Value& l_oLayer)
{

	LmLayerSeed l_oSeedBuffer;
	std::string l_sBuffer;

	//Images
	std::string l_sImgURLBuffer;
	int l_iAnchorPointBuffer;

	assert(l_oLayer.IsObject());
	assert(l_oLayer["Images"].IsArray() && l_oLayer.HasMember("Images"));
	for (int j = 0; j < l_oLayer["Images"].Size(); j++)
	{

		assert(
				l_oLayer["Images"][j]["imgURL"].IsString()
						&& l_oLayer["Images"][j].HasMember("imgURL"));

		l_sBuffer = l_oLayer["Images"][j]["imgURL"].GetString();
		l_sImgURLBuffer = l_sBuffer.c_str();

		assert(
				l_oLayer["Images"][j]["anchorPoint"].IsInt()
						&& l_oLayer["Images"][j].HasMember("anchorPoint"));

		l_iAnchorPointBuffer = l_oLayer["Images"][j]["anchorPoint"].GetInt();

		l_oSeedBuffer.Images.push_back(
		{ l_sImgURLBuffer, l_iAnchorPointBuffer });

	}
	//Sound
	assert(l_oLayer["SoundURL"].IsString() && l_oLayer.HasMember("SoundURL"));
	l_sBuffer = l_oLayer["SoundURL"].GetString();
	l_oSeedBuffer.SoundURL = l_sBuffer.c_str();
	//Label
	assert(l_oLayer["Label"].IsObject() && l_oLayer.HasMember("Label"));
	//color
	int r, g, b;
	assert(
			l_oLayer["Label"].HasMember("ColorText")
					&& l_oLayer["Label"]["ColorText"].IsObject());
	assert(
			l_oLayer["Label"]["ColorText"].HasMember("R")
					&& l_oLayer["Label"]["ColorText"]["R"].IsInt()
					&& l_oLayer["Label"]["ColorText"]["R"].GetInt() < 256);
	r = l_oLayer["Label"]["ColorText"]["R"].GetInt();
	assert(
			l_oLayer["Label"]["ColorText"].HasMember("G")
					&& l_oLayer["Label"]["ColorText"]["G"].IsInt()
					&& l_oLayer["Label"]["ColorText"]["G"].GetInt() < 256);
	g = l_oLayer["Label"]["ColorText"]["G"].GetInt();
	assert(
			l_oLayer["Label"]["ColorText"].HasMember("B")
					&& l_oLayer["Label"]["ColorText"]["B"].IsInt()
					&& l_oLayer["Label"]["ColorText"]["B"].GetInt() < 256);
	b = l_oLayer["Label"]["ColorText"]["B"].GetInt();
	l_oSeedBuffer.ColorText = cocos2d::Color3B(r, g, b);
	//percent
	assert(
			l_oLayer["Label"].HasMember("FontSize")
					&& l_oLayer["Label"]["FontSize"].IsInt());
	l_oSeedBuffer.FontSize = (float) l_oLayer["Label"]["FontSize"].GetInt();
	assert(
			l_oLayer["Label"].HasMember("WidthPercent")
					&& l_oLayer["Label"]["WidthPercent"].IsInt());
	l_oSeedBuffer.WidthPercent =
			(float) l_oLayer["Label"]["WidthPercent"].GetInt() * 0.01;
	assert(
			l_oLayer["Label"].HasMember("HeightPercent")
					&& l_oLayer["Label"]["HeightPercent"].IsInt());
	l_oSeedBuffer.HeightPercent =
			(float) l_oLayer["Label"]["HeightPercent"].GetInt() * 0.01;
	assert(
			l_oLayer["Label"].HasMember("SizePercent")
					&& l_oLayer["Label"]["SizePercent"].IsInt());
	l_oSeedBuffer.SizePercent =
			(float) l_oLayer["Label"]["SizePercent"].GetInt() * 0.01;
	//Text
	assert(
			l_oLayer["Label"].HasMember("Text")
					&& l_oLayer["Label"]["Text"].IsString());
	l_sBuffer = l_oLayer["Label"]["Text"].GetString();
	l_oSeedBuffer.Text = l_sBuffer.c_str();

	return new LmLayer(l_oSeedBuffer);
}

LmReward* LmJsonParser::makeLmReward(const rapidjson::Value& l_oReward)
{
	LmRewardSeed l_oSeedBuffer;

	//use to deep copy string
	std::string l_sBufferString;

	assert(
			l_oReward["FilenameSpriteBackground"].IsString()
					&& l_oReward.HasMember("FilenameSpriteBackground"));
	l_sBufferString = l_oReward["FilenameSpriteBackground"].GetString();
	l_oSeedBuffer.FilenameSpriteBackground = l_sBufferString.c_str();

	assert(
			l_oReward["FilenameSpriteReward"].IsString()
					&& l_oReward.HasMember("FilenameSpriteReward"));
	l_sBufferString = l_oReward["FilenameSpriteReward"].GetString();
	l_oSeedBuffer.FilenameSpriteReward = l_sBufferString.c_str();

	assert(
			l_oReward["RewardScore"].IsInt()
					&& l_oReward.HasMember("RewardScore"));
	l_oSeedBuffer.RewardScore = l_oReward["RewardScore"].GetInt();

	assert(
			l_oReward["FilenameSound"].IsString()
					&& l_oReward.HasMember("FilenameSound"));
	l_sBufferString = l_oReward["FilenameSound"].GetString();
	l_oSeedBuffer.FilenameSound = l_sBufferString.c_str();

	//Label
	assert(l_oReward["Label"].IsObject() && l_oReward.HasMember("Label"));
	//color
	int r, g, b;
	assert(
			l_oReward["Label"].HasMember("ColorText")
					&& l_oReward["Label"]["ColorText"].IsObject());
	assert(
			l_oReward["Label"]["ColorText"].HasMember("R")
					&& l_oReward["Label"]["ColorText"]["R"].IsInt()
					&& l_oReward["Label"]["ColorText"]["R"].GetInt() < 256);
	r = l_oReward["Label"]["ColorText"]["R"].GetInt();
	assert(
			l_oReward["Label"]["ColorText"].HasMember("G")
					&& l_oReward["Label"]["ColorText"]["G"].IsInt()
					&& l_oReward["Label"]["ColorText"]["G"].GetInt() < 256);
	g = l_oReward["Label"]["ColorText"]["G"].GetInt();
	assert(
			l_oReward["Label"]["ColorText"].HasMember("B")
					&& l_oReward["Label"]["ColorText"]["B"].IsInt()
					&& l_oReward["Label"]["ColorText"]["B"].GetInt() < 256);
	b = l_oReward["Label"]["ColorText"]["B"].GetInt();
	l_oSeedBuffer.ColorText = cocos2d::Color3B(r, g, b);
	//percent
	assert(
			l_oReward["Label"].HasMember("FontSize")
					&& l_oReward["Label"]["FontSize"].IsInt());
	l_oSeedBuffer.FontSize = (float) l_oReward["Label"]["FontSize"].GetInt();
	assert(
			l_oReward["Label"].HasMember("WidthPercent")
					&& l_oReward["Label"]["WidthPercent"].IsInt());
	l_oSeedBuffer.WidthPercent =
			(float) l_oReward["Label"]["WidthPercent"].GetInt() * 0.01;
	assert(
			l_oReward["Label"].HasMember("HeightPercent")
					&& l_oReward["Label"]["HeightPercent"].IsInt());
	l_oSeedBuffer.HeightPercent =
			(float) l_oReward["Label"]["HeightPercent"].GetInt() * 0.01;
	assert(
			l_oReward["Label"].HasMember("SizePercent")
					&& l_oReward["Label"]["SizePercent"].IsInt());
	l_oSeedBuffer.SizePercent =
			(float) l_oReward["Label"]["SizePercent"].GetInt() * 0.01;
	//Text
	assert(
			l_oReward["Label"].HasMember("Text")
					&& l_oReward["Label"]["Text"].IsString());
	l_sBufferString = l_oReward["Label"]["Text"].GetString();
	l_oSeedBuffer.Text = l_sBufferString.c_str();

	return new LmReward(l_oSeedBuffer);
}

void LmJsonParser::initInteractionAttributesSceneOfTheGame()
{
	// Using a reference for consecutive access is handy and faster.
	assert(
			m_oDocument["Ludomuse"]["SceneArray"].IsArray()
					&& m_oDocument["Ludomuse"].HasMember("SceneArray"));
	const rapidjson::Value& l_aSceneArray =
			m_oDocument["Ludomuse"]["SceneArray"];

	int l_iIdScene = 0;

	for (int i = 0; i < l_aSceneArray.Size(); i++)
	{
		assert(l_aSceneArray[i].IsObject());
		assert(
				l_aSceneArray[i]["Id"].IsInt()
						&& l_aSceneArray[i].HasMember("Id"));
		l_iIdScene = l_aSceneArray[i]["Id"].GetInt();
		CCLOG("Initialization LmInteractionScene Id = %d", l_iIdScene);
		switch (l_iIdScene)
		{

		case LmRightSpotScene::s_iId:
			makeLmRightSpotScene(l_aSceneArray[i]);
			break;
		case LmQuizz_v1Scene::s_iId:
			makeLmQuizz_v1Scene(l_aSceneArray[i]);
			break;
		case LmFindGoodCategoryScene::s_iId:
			makeLmFindGoodCategoryScene(l_aSceneArray[i]);
			break;
		case LmHintScene::s_iId:
			makeLmHintScene(l_aSceneArray[i]);
			break;
		case LmQuizz_v2Scene::s_iId:
			makeLmQuizz_v2Scene(l_aSceneArray[i]);
			break;
		case LmTakePictureScene::s_iId:
			makeLmTakePictureScene(l_aSceneArray[i]);
			break;
		default:
			break;
		}
	}

	CCLOG("All LmInteractionScene have been initialized");

}

LmSetPoint* LmJsonParser::getLmSetPoint(const rapidjson::Value& l_oScene,
		const char* l_sNameSetPoint)
{
	LmSetPoint * l_pLmSetPoint = new LmSetPoint;

	//buffers
	std::vector<std::pair<std::string, int>> l_aImgBuffer;
	std::string l_sImgURLBuffer;
	int l_iAnchorPointBuffer;
	std::string l_sSoundURLBuffer;
	std::string l_sTextURLBuffer;

	std::string l_sBuffer;

	assert(
			l_oScene[l_sNameSetPoint].IsArray()
					&& l_oScene.HasMember(l_sNameSetPoint));
	for (int i = 0; i < l_oScene[l_sNameSetPoint].Size(); i++)
	{
		//for each layer
		assert(l_oScene[l_sNameSetPoint][i].IsObject());

		l_pLmSetPoint->add(makeLmLayer(l_oScene[l_sNameSetPoint][i]));

	}

	return l_pLmSetPoint;
}

void LmJsonParser::initSetPoint(const rapidjson::Value& l_oScene,
		LmInteractionScene* l_pInteractionScene)
{
	if (m_bIsParent)
	{
		//get the element we just push to set the introduction begin
		l_pInteractionScene->setPLmSetPointBegin(
				getLmSetPoint(l_oScene, "SetPointBeginParent"));
		//get the element we just push to set the introduction end
		l_pInteractionScene->setPLmSetPointEnd(
				getLmSetPoint(l_oScene, "SetPointEndParent"));
	}
	else
	{
		//get the element we just push to set the introduction begin
		l_pInteractionScene->setPLmSetPointBegin(
				getLmSetPoint(l_oScene, "SetPointBeginChild"));
		//get the element we just push to set the introduction end
		l_pInteractionScene->setPLmSetPointEnd(
				getLmSetPoint(l_oScene, "SetPointEndChild"));
	}
}

void LmJsonParser::initDescription(const rapidjson::Value& l_oScene,
		LmInteractionScene* l_pInteractionScene)
{
	//use to deep copy string
	std::string l_sBufferString;

	assert(
			l_oScene["Description"].IsString()
					&& l_oScene.HasMember("Description"));
	l_sBufferString = l_oScene["Description"].GetString();
	l_pInteractionScene->setSDescription(l_sBufferString.c_str());
}

void LmJsonParser::initInteractionAttributes(const rapidjson::Value& l_oScene,
		LmInteractionScene* l_pInteractionScene)
{
	//get the element we just push to set the introduction
	initSetPoint(l_oScene, l_pInteractionScene);

	initReward(l_oScene, l_pInteractionScene);

	initDescription(l_oScene, l_pInteractionScene);
}

void LmJsonParser::initReward(const rapidjson::Value& l_oScene,
		LmInteractionScene* l_pInteractionScene)
{

	if (m_bIsParent)
	{
		if (l_oScene.HasMember("RewardParent"))
		{
			l_pInteractionScene->setPLmReward(
					makeLmReward(l_oScene["RewardParent"]));
		}

		if (l_oScene.HasMember("RewardChild"))
		{
			l_pInteractionScene->setPLmRewardUser2(
					makeLmReward(l_oScene["RewardChild"]));
		}
	}
	else
	{
		if (l_oScene.HasMember("RewardParent"))
		{
			l_pInteractionScene->setPLmRewardUser2(
					makeLmReward(l_oScene["RewardParent"]));
		}

		if (l_oScene.HasMember("RewardChild"))
		{
			l_pInteractionScene->setPLmReward(
					makeLmReward(l_oScene["RewardChild"]));
		}
	}

}

void LmJsonParser::makeLmRightSpotScene(const rapidjson::Value& l_oScene)
{
	LmRightSpotSceneSeed l_SeedBuffer;

	//use to make deep copy
	std::string l_sBuffer;

	assert(
			l_oScene["FilenameSpriteBackground"].IsString()
					&& l_oScene.HasMember("FilenameSpriteBackground"));
	l_sBuffer = l_oScene["FilenameSpriteBackground"].GetString();
	l_SeedBuffer.FilenameSpriteBackground = l_sBuffer.c_str();

	assert(
			l_oScene["FilenameSpriteSendingArea"].IsString()
					&& l_oScene.HasMember("FilenameSpriteSendingArea"));
	l_sBuffer = l_oScene["FilenameSpriteSendingArea"].GetString();
	l_SeedBuffer.FilenameSpriteSendingArea = l_sBuffer.c_str();

	assert(
			l_oScene["FilenameMainImage"].IsString()
					&& l_oScene.HasMember("FilenameMainImage"));
	l_sBuffer = l_oScene["FilenameMainImage"].GetString();
	l_SeedBuffer.FilenameMainImage = l_sBuffer.c_str();

	assert(l_oScene["HoleOnX"].IsInt() && l_oScene.HasMember("HoleOnX"));
	l_SeedBuffer.HoleOnX = l_oScene["HoleOnX"].GetInt();

	assert(l_oScene["HoleOnY"].IsInt() && l_oScene.HasMember("HoleOnY"));
	l_SeedBuffer.HoleOnY = l_oScene["HoleOnY"].GetInt();

	int x = 0;
	int y = 0;
	assert(
			l_oScene["LocationOfHole"].IsArray()
					&& l_oScene.HasMember("LocationOfHole"));
	for (int i = 0; i < l_oScene["LocationOfHole"].Size(); i++)
	{
		assert(l_oScene["LocationOfHole"][i].IsObject());
		assert(
				l_oScene["LocationOfHole"][i]["x"].IsInt()
						&& l_oScene["LocationOfHole"][i].HasMember("x"));
		assert(
				l_oScene["LocationOfHole"][i]["y"].IsInt()
						&& l_oScene["LocationOfHole"][i].HasMember("y"));

		x = l_oScene["LocationOfHole"][i]["x"].GetInt();
		y = l_oScene["LocationOfHole"][i]["y"].GetInt();

		l_SeedBuffer.LocationOfHole.push_back(
		{ x, y });
	}

	//create the scene delete in the game manager
	m_aInteractionSceneOfTheGame.push_back(new LmRightSpotScene(l_SeedBuffer));

	initInteractionAttributes(l_oScene,
			m_aInteractionSceneOfTheGame.at(
					m_aInteractionSceneOfTheGame.size() - 1));

}

void LmJsonParser::makeLmQuizz_v1Scene(const rapidjson::Value& l_oScene)
{

	//buffer seed
	LmQuizz_v1SceneSeed l_SeedBuffer;

	//use to deep copy string
	std::string l_sBufferString;

	assert(
			l_oScene["FilenameSpriteBackground"].IsString()
					&& l_oScene.HasMember("FilenameSpriteBackground"));
	l_sBufferString = l_oScene["FilenameSpriteBackground"].GetString();
	l_SeedBuffer.FilenameSpriteBackground = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameSpriteBandTop"].IsString()
					&& l_oScene.HasMember("FilenameSpriteBandTop"));
	l_sBufferString = l_oScene["FilenameSpriteBandTop"].GetString();
	l_SeedBuffer.FilenameSpriteBandTop = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameSpriteAnswerBackground"].IsString()
					&& l_oScene.HasMember("FilenameSpriteAnswerBackground"));
	l_sBufferString = l_oScene["FilenameSpriteAnswerBackground"].GetString();
	l_SeedBuffer.FilenameSpriteAnswerBackground = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameSpriteAnswerCross"].IsString()
					&& l_oScene.HasMember("FilenameSpriteAnswerCross"));
	l_sBufferString = l_oScene["FilenameSpriteAnswerCross"].GetString();
	l_SeedBuffer.FilenameSpriteAnswerCross = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameAudioAnswerSelected"].IsString()
					&& l_oScene.HasMember("FilenameAudioAnswerSelected"));
	l_sBufferString = l_oScene["FilenameAudioAnswerSelected"].GetString();
	l_SeedBuffer.FilenameAudioAnswerSelected = l_sBufferString.c_str();

	assert(l_oScene["InGameScreen"].IsObject());
	assert(l_oScene.HasMember("InGameScreen"));
	l_SeedBuffer.InGameScreenParent = makeLmLayer(l_oScene["InGameScreen"]);
	assert(l_oScene["ReplayScreen"].IsObject());
	assert(l_oScene.HasMember("ReplayScreen"));
	l_SeedBuffer.ReplayScreen = makeLmLayer(l_oScene["ReplayScreen"]);

	assert(l_oScene["Questions"].IsArray());

	//buffers to construct an LmQuestion
	LmQuestionSeed l_SeedQuestionBuffer;

	for (int i = 0; i < l_oScene["Questions"].Size(); i++)
	{
		assert(l_oScene["Questions"][i].IsObject());

		assert(
				l_oScene["Questions"][i]["Question"].IsString()
						&& l_oScene["Questions"][i].HasMember("Question"));
		l_sBufferString = l_oScene["Questions"][i]["Question"].GetString();
		l_SeedQuestionBuffer.Question = l_sBufferString.c_str();

		assert(
				l_oScene["Questions"][i]["Answer1"].IsString()
						&& l_oScene["Questions"][i].HasMember("Answer1"));
		l_sBufferString = l_oScene["Questions"][i]["Answer1"].GetString();
		l_SeedQuestionBuffer.Answer1 = l_sBufferString.c_str();

		assert(
				l_oScene["Questions"][i]["Answer2"].IsString()
						&& l_oScene["Questions"][i].HasMember("Answer2"));
		l_sBufferString = l_oScene["Questions"][i]["Answer2"].GetString();
		l_SeedQuestionBuffer.Answer2 = l_sBufferString.c_str();

		assert(
				l_oScene["Questions"][i]["Answer3"].IsString()
						&& l_oScene["Questions"][i].HasMember("Answer3"));
		l_sBufferString = l_oScene["Questions"][i]["Answer3"].GetString();
		l_SeedQuestionBuffer.Answer3 = l_sBufferString.c_str();

		assert(
				l_oScene["Questions"][i]["Answer4"].IsString()
						&& l_oScene["Questions"][i].HasMember("Answer4"));
		l_sBufferString = l_oScene["Questions"][i]["Answer4"].GetString();
		l_SeedQuestionBuffer.Answer4 = l_sBufferString.c_str();

		assert(
				l_oScene["Questions"][i]["NumberRightAnswer"].IsInt()
						&& l_oScene["Questions"][i].HasMember(
								"NumberRightAnswer"));
		l_SeedQuestionBuffer.NumberGoodAnswer =
				l_oScene["Questions"][i]["NumberRightAnswer"].GetInt();

		l_SeedBuffer.Questions.push_back(new LmQuestion(l_SeedQuestionBuffer));
	}

	assert(
			l_oScene["AttemptByQuestion"].IsInt()
					&& l_oScene.HasMember("AttemptByQuestion"));
	l_SeedBuffer.AttemptByQuestion = l_oScene["AttemptByQuestion"].GetInt();

	assert(
			l_oScene["TimerDuration"].IsInt()
					&& l_oScene.HasMember("TimerDuration"));
	l_SeedBuffer.TimerDuration = (float) l_oScene["TimerDuration"].GetInt();
	l_SeedBuffer.TimerDuration *= 0.01; //conversion

	assert(
			l_oScene["TimerEnbaled"].IsBool()
					&& l_oScene.HasMember("TimerEnbaled"));
	l_SeedBuffer.TimerEnbaled = l_oScene["TimerEnbaled"].GetBool();

	m_aInteractionSceneOfTheGame.push_back(new LmQuizz_v1Scene(l_SeedBuffer));

	initInteractionAttributes(l_oScene,
			m_aInteractionSceneOfTheGame.at(
					m_aInteractionSceneOfTheGame.size() - 1));

}

void LmJsonParser::makeLmQuizz_v2Scene(const rapidjson::Value& l_oScene)
{

	//buffer seed
	LmQuizz_v2SceneSeed l_SeedBuffer;

	//use to deep copy string
	std::string l_sBufferString;

	assert(
			l_oScene["FilenameSpriteBackground"].IsString()
					&& l_oScene.HasMember("FilenameSpriteBackground"));
	l_sBufferString = l_oScene["FilenameSpriteBackground"].GetString();
	l_SeedBuffer.FilenameSpriteBackground = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameSpriteBandTop"].IsString()
					&& l_oScene.HasMember("FilenameSpriteBandTop"));
	l_sBufferString = l_oScene["FilenameSpriteBandTop"].GetString();
	l_SeedBuffer.FilenameSpriteBandTop = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameSpriteAnswerBackground"].IsString()
					&& l_oScene.HasMember("FilenameSpriteAnswerBackground"));
	l_sBufferString = l_oScene["FilenameSpriteAnswerBackground"].GetString();
	l_SeedBuffer.FilenameSpriteAnswerBackground = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameSpriteAnswerCross"].IsString()
					&& l_oScene.HasMember("FilenameSpriteAnswerCross"));
	l_sBufferString = l_oScene["FilenameSpriteAnswerCross"].GetString();
	l_SeedBuffer.FilenameSpriteAnswerCross = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameAudioGoodAnswer"].IsString()
					&& l_oScene.HasMember("FilenameAudioGoodAnswer"));
	l_sBufferString = l_oScene["FilenameAudioGoodAnswer"].GetString();
	l_SeedBuffer.FilenameAudioGoodAnswer = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameAudioBadAnswer"].IsString()
					&& l_oScene.HasMember("FilenameAudioBadAnswer"));
	l_sBufferString = l_oScene["FilenameAudioBadAnswer"].GetString();
	l_SeedBuffer.FilenameAudioBadAnswer = l_sBufferString.c_str();

	LmQuestionSeed l_SeedQuestionBuffer;

	/*
	 * PARENT QUESTION
	 */
	assert(
			l_oScene["QuestionsParent"].IsArray()
					&& l_oScene.HasMember("QuestionsParent"));

	for (int i = 0; i < l_oScene["QuestionsParent"].Size(); i++)
	{
		assert(l_oScene["QuestionsParent"][i].IsObject());

		assert(
				l_oScene["QuestionsParent"][i]["Question"].IsString()
						&& l_oScene["QuestionsParent"][i].HasMember(
								"Question"));
		l_sBufferString =
				l_oScene["QuestionsParent"][i]["Question"].GetString();
		l_SeedQuestionBuffer.Question = l_sBufferString.c_str();
		assert(
				l_oScene["QuestionsParent"][i]["NumberRightAnswer"].IsInt()
						&& l_oScene["QuestionsParent"][i].HasMember(
								"NumberRightAnswer"));
		l_SeedQuestionBuffer.NumberGoodAnswer =
				l_oScene["QuestionsParent"][i]["NumberRightAnswer"].GetInt();

		assert(
				l_oScene["QuestionsParent"][i]["Answer1"].IsString()
						&& l_oScene["QuestionsParent"][i].HasMember("Answer1"));
		l_sBufferString = l_oScene["QuestionsParent"][i]["Answer1"].GetString();
		l_SeedQuestionBuffer.Answer1 = l_sBufferString.c_str();

		assert(
				l_oScene["QuestionsParent"][i]["Answer2"].IsString()
						&& l_oScene["QuestionsParent"][i].HasMember("Answer2"));
		l_sBufferString = l_oScene["QuestionsParent"][i]["Answer2"].GetString();
		l_SeedQuestionBuffer.Answer2 = l_sBufferString.c_str();
		assert(
				l_oScene["QuestionsParent"][i]["Answer3"].IsString()
						&& l_oScene["QuestionsParent"][i].HasMember("Answer3"));
		l_sBufferString = l_oScene["QuestionsParent"][i]["Answer3"].GetString();
		l_SeedQuestionBuffer.Answer3 = l_sBufferString.c_str();

		assert(
				l_oScene["QuestionsParent"][i]["Answer4"].IsString()
						&& l_oScene["QuestionsParent"][i].HasMember("Answer4"));
		l_sBufferString = l_oScene["QuestionsParent"][i]["Answer4"].GetString();
		l_SeedQuestionBuffer.Answer4 = l_sBufferString.c_str();

		l_SeedBuffer.QuestionsParent.push_back(
				new LmQuestion(l_SeedQuestionBuffer));
	}
	/*
	 * CHILD QUESTION
	 */

	assert(
			l_oScene["QuestionsChild"].IsArray()
					&& l_oScene.HasMember("QuestionsChild"));

	for (int i = 0; i < l_oScene["QuestionsChild"].Size(); i++)
	{
		assert(l_oScene["QuestionsChild"][i].IsObject());

		assert(
				l_oScene["QuestionsChild"][i]["Question"].IsString()
						&& l_oScene["QuestionsChild"][i].HasMember("Question"));
		l_sBufferString = l_oScene["QuestionsChild"][i]["Question"].GetString();
		l_SeedQuestionBuffer.Question = l_sBufferString.c_str();
		assert(
				l_oScene["QuestionsChild"][i]["NumberRightAnswer"].IsInt()
						&& l_oScene["QuestionsChild"][i].HasMember(
								"NumberRightAnswer"));
		l_SeedQuestionBuffer.NumberGoodAnswer =
				l_oScene["QuestionsChild"][i]["NumberRightAnswer"].GetInt();

		assert(
				l_oScene["QuestionsChild"][i]["Answer1"].IsString()
						&& l_oScene["QuestionsChild"][i].HasMember("Answer1"));
		l_sBufferString = l_oScene["QuestionsChild"][i]["Answer1"].GetString();
		l_SeedQuestionBuffer.Answer1 = l_sBufferString.c_str();

		assert(
				l_oScene["QuestionsChild"][i]["Answer2"].IsString()
						&& l_oScene["QuestionsChild"][i].HasMember("Answer2"));
		l_sBufferString = l_oScene["QuestionsChild"][i]["Answer2"].GetString();
		l_SeedQuestionBuffer.Answer2 = l_sBufferString.c_str();

		assert(
				l_oScene["QuestionsChild"][i]["Answer3"].IsString()
						&& l_oScene["QuestionsChild"][i].HasMember("Answer3"));
		l_sBufferString = l_oScene["QuestionsChild"][i]["Answer3"].GetString();
		l_SeedQuestionBuffer.Answer3 = l_sBufferString.c_str();

		assert(
				l_oScene["QuestionsChild"][i]["Answer4"].IsString()
						&& l_oScene["QuestionsChild"][i].HasMember("Answer4"));
		l_sBufferString = l_oScene["QuestionsChild"][i]["Answer4"].GetString();
		l_SeedQuestionBuffer.Answer4 = l_sBufferString.c_str();

		l_SeedBuffer.QuestionsChild.push_back(
				new LmQuestion(l_SeedQuestionBuffer));

	}

	assert(
			l_oScene["AttemptByQuestion"].IsInt()
					&& l_oScene.HasMember("AttemptByQuestion"));
	l_SeedBuffer.AttemptByQuestion = l_oScene["AttemptByQuestion"].GetInt();

	assert(
			l_oScene["TimerDuration"].IsInt()
					&& l_oScene.HasMember("TimerDuration"));
	l_SeedBuffer.TimerDuration = (float) l_oScene["TimerDuration"].GetInt();
	l_SeedBuffer.TimerDuration *= 0.01; //conversion

	assert(
			l_oScene["TimerEnbaled"].IsBool()
					&& l_oScene.HasMember("TimerEnbaled"));
	l_SeedBuffer.TimerEnbaled = l_oScene["TimerEnbaled"].GetBool();

	m_aInteractionSceneOfTheGame.push_back(new LmQuizz_v2Scene(l_SeedBuffer));

	initInteractionAttributes(l_oScene,
			m_aInteractionSceneOfTheGame.at(
					m_aInteractionSceneOfTheGame.size() - 1));

}

void LmJsonParser::makeLmFindGoodCategoryScene(const rapidjson::Value& l_oScene)
{
//buffer seed
	LmFindGoodCategorySceneSeed l_SeedBuffer;

//use to deep copy string
	std::string l_sBufferString;

	assert(
			l_oScene["FilenameSpriteBackground"].IsString()
					&& l_oScene.HasMember("FilenameSpriteBackground"));
	l_sBufferString = l_oScene["FilenameSpriteBackground"].GetString();
	l_SeedBuffer.FilenameSpriteBackground = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameSpriteSendingArea"].IsString()
					&& l_oScene.HasMember("FilenameSpriteSendingArea"));
	l_sBufferString = l_oScene["FilenameSpriteSendingArea"].GetString();
	l_SeedBuffer.FilenameSpriteSendingArea = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameAudioBadCategory"].IsString()
					&& l_oScene.HasMember("FilenameAudioBadCategory"));
	l_sBufferString = l_oScene["FilenameAudioBadCategory"].GetString();
	l_SeedBuffer.FilenameAudioBadCategory = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameAudioGoodCategory"].IsString()
					&& l_oScene.HasMember("FilenameAudioGoodCategory"));
	l_sBufferString = l_oScene["FilenameAudioGoodCategory"].GetString();
	l_SeedBuffer.FilenameAudioGoodCategory = l_sBufferString.c_str();

	assert(l_oScene["Images"].IsArray() && l_oScene.HasMember("Images"));
	for (int i = 0; i < l_oScene["Images"].Size(); i++)
	{
		assert(l_oScene["Images"][i].IsObject());
		assert(
				l_oScene["Images"][i].HasMember("Id")
						&& l_oScene["Images"][i]["Id"].IsInt());
		assert(
				l_oScene["Images"][i].HasMember("FilenameImage")
						&& l_oScene["Images"][i]["FilenameImage"].IsString());
		l_sBufferString = l_oScene["Images"][i]["FilenameImage"].GetString();
		l_SeedBuffer.Images.push_back(
		{ l_oScene["Images"][i]["Id"].GetInt(), l_sBufferString.c_str() });
	}

	assert(
			l_oScene["Categories"].IsArray()
					&& l_oScene.HasMember("Categories"));
	for (int i = 0; i < l_oScene["Categories"].Size(); i++)
	{
		assert(l_oScene["Categories"][i].IsObject());
		assert(
				l_oScene["Categories"][i].HasMember("Id")
						&& l_oScene["Categories"][i]["Id"].IsInt());
		assert(
				l_oScene["Categories"][i].HasMember("FilenameCategorySprite")
						&& l_oScene["Categories"][i]["FilenameCategorySprite"].IsString());
		l_sBufferString =
				l_oScene["Categories"][i]["FilenameCategorySprite"].GetString();
		l_SeedBuffer.Categories.push_back(
		{ l_oScene["Categories"][i]["Id"].GetInt(), l_sBufferString.c_str() });
	}

	assert(
			l_oScene["NumberOfGoodImages"].IsInt()
					&& l_oScene.HasMember("NumberOfGoodImages"));
	l_SeedBuffer.NumberOfGoodImages = l_oScene["NumberOfGoodImages"].GetInt();

	m_aInteractionSceneOfTheGame.push_back(
			new LmFindGoodCategoryScene(l_SeedBuffer));

	initInteractionAttributes(l_oScene,
			m_aInteractionSceneOfTheGame.at(
					m_aInteractionSceneOfTheGame.size() - 1));
}

void LmJsonParser::makeLmHintScene(const rapidjson::Value& l_oScene)
{
//buffer seed
	LmHintSceneSeed l_SeedBuffer;

//use to deep copy string
	std::string l_sBufferString;

	assert(
			l_oScene["FilenameSpriteBackground"].IsString()
					&& l_oScene.HasMember("FilenameSpriteBackground"));
	l_sBufferString = l_oScene["FilenameSpriteBackground"].GetString();
	l_SeedBuffer.FilenameSpriteBackground = l_sBufferString.c_str();

	assert(
			l_oScene["FilenameSpriteMainImage"].IsString()
					&& l_oScene.HasMember("FilenameSpriteMainImage"));
	l_sBufferString = l_oScene["FilenameSpriteMainImage"].GetString();
	l_SeedBuffer.FilenameSpriteMainImage = l_sBufferString.c_str();

	assert(l_oScene.HasMember("Labels") && l_oScene["Labels"].IsArray());
	for (int i = 0; i < l_oScene["Labels"].Size(); i++)
	{
		assert(
				l_oScene["Labels"][i].HasMember("Id")
						&& l_oScene["Labels"][i]["Id"].IsInt());
		assert(
				l_oScene["Labels"][i].HasMember("WidthPercent")
						&& l_oScene["Labels"][i]["WidthPercent"].IsInt());
		assert(
				l_oScene["Labels"][i].HasMember("HeightPercent")
						&& l_oScene["Labels"][i]["HeightPercent"].IsInt());

		l_SeedBuffer.LabelsCoordonateHole.insert(
				{ l_oScene["Labels"][i]["Id"].GetInt(), Vec2(
						l_oScene["Labels"][i]["WidthPercent"].GetInt(),
						l_oScene["Labels"][i]["HeightPercent"].GetInt()) });

		assert(
				l_oScene["Labels"][i].HasMember("FilenameSpriteLabel")
						&& l_oScene["Labels"][i]["FilenameSpriteLabel"].IsString());
		l_sBufferString =
				l_oScene["Labels"][i]["FilenameSpriteLabel"].GetString();

		l_SeedBuffer.LabelsFilenameSprite.insert(
		{ l_oScene["Labels"][i]["Id"].GetInt(), l_sBufferString.c_str() });

		assert(
				l_oScene["Labels"][i].HasMember("FilenameSpriteHint")
						&& l_oScene["Labels"][i]["FilenameSpriteHint"].IsString());
		l_sBufferString =
				l_oScene["Labels"][i]["FilenameSpriteHint"].GetString();

		l_SeedBuffer.LabelsFilenameSpriteHint.insert(
		{ l_oScene["Labels"][i]["Id"].GetInt(), l_sBufferString.c_str() });

	}

	m_aInteractionSceneOfTheGame.push_back(new LmHintScene(l_SeedBuffer));

	initInteractionAttributes(l_oScene,
			m_aInteractionSceneOfTheGame.at(
					m_aInteractionSceneOfTheGame.size() - 1));
}

void LmJsonParser::makeLmTakePictureScene(const rapidjson::Value& l_oScene)
{
	//buffer seed
	LmTakePictureSceneSeed l_SeedBuffer;

	//use to deep copy string
	std::string l_sBufferString;

	assert(
			l_oScene["FilenameSpriteBackground"].IsString()
					&& l_oScene.HasMember("FilenameSpriteBackground"));
	l_sBufferString = l_oScene["FilenameSpriteBackground"].GetString();
	l_SeedBuffer.FilenameSpriteBackground = l_sBufferString.c_str();

	m_aInteractionSceneOfTheGame.push_back(
			new LmTakePictureScene(l_SeedBuffer));

	initInteractionAttributes(l_oScene,
			m_aInteractionSceneOfTheGame.at(
					m_aInteractionSceneOfTheGame.size() - 1));
}
