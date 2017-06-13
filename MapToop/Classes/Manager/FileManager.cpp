#include "FileManager.h"
#include "cocos2d.h"
#include "FileData/FileData_Npc.h"
#include "FileData/FileData_Anim.h"
#include "FileData/FileData_Behaviour.h"
#include "FileData/FileData_TalkData.h"
#include "FileData/FileData_Map.h"
FileManager::FileManager()
{

}

FileManager::~FileManager()
{

}

FileManager * FileManager::m_FileManager = nullptr;
FileManager * FileManager::GetInstance()
{
	if (m_FileManager == nullptr)
	{
		m_FileManager = new FileManager();
	}

	return m_FileManager;
}

bool FileManager::LoadFileData()
{
	bool isSuccess = true;

	isSuccess = FileDataNpc::GetInstance()->LoadDataFromFile("FileData/Npc.txt");
	if (isSuccess == false)
	{
		CCLOG("[LoadFileData] Load failed [FileData/Npc.txt]");
	}

	isSuccess = FileDataAnim::GetInstance()->LoadDataFromFile("FileData/Anim.txt");
	if (isSuccess == false)
	{
		CCLOG("[LoadFileData] Load failed [FileData/Anim.txt]");
	}
	isSuccess = FileDataBehaviour::GetInstance()->LoadDataFromFile();
	if (isSuccess == false)
	{
		CCLOG("[LoadFileData] Load failed [FileDataBehaviour]");
	}
	isSuccess = FileDataBehaviour::GetInstance()->LoadDataFromFile();
	if (isSuccess == false)
	{
		CCLOG("[LoadFileData] Load failed [FileDataBehaviour]");
	}
	isSuccess = FileDataTalkData::GetInstance()->LoadDataFromFile("FileData/TalkData.txt");
	if (isSuccess == false)
	{
		CCLOG("[LoadFileData] Load failed [FileDataTalkData]");
	}
	isSuccess = FileDataMap::GetInstance()->LoadDataFromFile("FileData/Map.txt");
	if (isSuccess == false)
	{
		CCLOG("[LoadFileData] Load failed [FileDataMap]");
	}
	return isSuccess;
}