#ifndef __StoryDirector_H__
#define __StoryDirector_H__
#include "Config/Head.h"

class GameerData
{
public:
	GameerData();
	~GameerData();
public:
	void loadGameData(int gameSaveId);
	void saveGameData();
	string serializeToString();
	void unserialize(const string & gameData);
public:
	long m_GameTime;
	int  m_GameSaveId;

};



class StoryDirector
{
public:
	StoryDirector();
	~StoryDirector();
	static StoryDirector * GetInstance();
private:
	static StoryDirector * m_StoryDirector;
public:
	int CreateNewGameSave();
	void update(float dt);
	long GetGameTime();
	float GetTimeScale();
	void SetTimeScale(float timeScale);
private:
	GameerData * m_GameData;
	double m_GameTime;
	float  m_TimeScale;
};



#endif // __StoryDirector_H__