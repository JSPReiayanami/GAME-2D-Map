#include "StoryDirector.h"
#include "Tool/StringHelper.h"
#define __GAME_SAVE_ID "SAVE_ID"
GameerData::GameerData()
{
	m_GameTime = 0;
	m_GameSaveId = 0;
}

GameerData::~GameerData()
{

}

void GameerData::loadGameData(int gameSaveId)
{
	m_GameSaveId = gameSaveId;
}
void GameerData::saveGameData()
{

}

string GameerData::serializeToString()
{
	return "";
}
void GameerData::unserialize(const string & gameData)
{
	auto data = StringHelper::splitString(gameData, ";");
	if (data.size() > 0)
	{
		if (data.size() >= 1)
		{
			m_GameSaveId = atoi(data.at(0).c_str());
		}
		if (data.size() >= 2)
		{
			m_GameTime = atol(data.at(1).c_str());
		}
	}
}


//=========================

StoryDirector::StoryDirector()
{
	m_GameData = new GameerData();
	m_TimeScale = 10;
}

StoryDirector::~StoryDirector()
{
	CC_SAFE_DELETE(m_GameData);
}
StoryDirector * StoryDirector::m_StoryDirector = nullptr;
StoryDirector * StoryDirector::GetInstance()
{
	if (m_StoryDirector == nullptr)
	{
		m_StoryDirector = new StoryDirector();
	}
	return m_StoryDirector;
}

int StoryDirector::CreateNewGameSave()
{
	//UserDefault::getInstance()->setStringForKey(__GAME_SAVE_ID, "666");
	string gameData = "666;0;";
	//UserDefault::getInstance()->setStringForKey("GAME_SAVE_666", gameData);
	m_GameData->unserialize(gameData);
	m_GameTime = (double)m_GameData->m_GameTime;
	return 0;
}

void StoryDirector::update(float dt)
{
	m_GameTime += dt*m_TimeScale;
	m_GameData->m_GameTime = (long)m_GameTime;
}

long StoryDirector::GetGameTime()
{
	return m_GameData->m_GameTime;
}

float StoryDirector::GetTimeScale()
{
	return	m_TimeScale;
}

void StoryDirector::SetTimeScale(float timeScale)
{
	m_TimeScale = timeScale;
}
