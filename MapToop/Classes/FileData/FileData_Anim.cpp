#include "FileData_Anim.h"
#include "Tool/TabFileReader.h"
#include "Tool/StringHelper.h"
FileDataAnim::FileDataAnim()
{
	m_Cfg.clear();
	m_SpineCfg.clear();

}

FileDataAnim::~FileDataAnim()
{

}
FileDataAnim * FileDataAnim::m_FileDataAnim = nullptr;

FileDataAnim * FileDataAnim::GetInstance()
{
	if (m_FileDataAnim == nullptr)
	{
		m_FileDataAnim = new FileDataAnim();
	}
	return m_FileDataAnim;
}

bool FileDataAnim::LoadDataFromFile(const char* fileName)
{
	TFReader file = TFReader::create(fileName);
	if (!file)
		return false;
	m_Cfg.clear();
	int count = (int)file.GetItemCount();
	for (int i = 0; i < count; ++i) {
		CfgAnim cfg;
		file.Read(i, "Id", cfg.Id);
		file.Read(i, "AnimSpeed", cfg.AnimSpeed);
		file.Read(i, "BAnim", cfg.BAnim);
		file.Read(i, "BAnimNum", cfg.BAnimNum);
		file.Read(i, "FAnim", cfg.FAnim);
		file.Read(i, "FAnimNum", cfg.FAnimNum);
		file.Read(i, "LRAnim", cfg.LRAnim);
		file.Read(i, "LRAnimNum", cfg.LRAnimNum);
		m_Cfg.insert(make_pair(cfg.Id, cfg));
	}
	return true;
}

bool FileDataAnim::LoadDataFromFileSpine(const char* fileName)
{
	TFReader file = TFReader::create(fileName);
	if (!file)
		return false;
	m_SpineCfg.clear();
	int count = (int)file.GetItemCount();
	for (int i = 0; i < count; ++i) {
		CfgSpineAnim cfg;
		file.Read(i, "Id", cfg.Id);
		file.Read(i, "AnimSpeed", cfg.SpineJson);
		file.Read(i, "BAnim", cfg.SpineRes);
		file.Read(i, "BAnimNum", cfg.SpineScale);
		m_SpineCfg.insert(make_pair(cfg.Id, cfg));
	}
	return true;
}
const CfgAnim* FileDataAnim::GetCfg(int Id)
{
	if (m_Cfg.find(Id) != m_Cfg.end())
	{
		return &m_Cfg.at(Id);
	}
	return nullptr;
}

const CfgSpineAnim* FileDataAnim::GetSpineCfg(int Id)
{
	if (m_SpineCfg.find(Id) != m_SpineCfg.end())
	{
		return &m_SpineCfg.at(Id);
	}
	return nullptr;
}

std::string FileDataAnim::GetActionName(ActionType actionType)
{
	if (m_SpineActionName.find(actionType) != m_SpineActionName.end())
	{
		return m_SpineActionName.at(actionType);
	}

	return "";
}

void FileDataAnim::InitActionName()
{
	m_SpineActionName.clear();
	m_SpineActionName.insert(std::make_pair(Action_Walk, "Walk"));
	m_SpineActionName.insert(std::make_pair(Action_Run, "Run"));
	m_SpineActionName.insert(std::make_pair(Action_Attack, "Attack"));
	m_SpineActionName.insert(std::make_pair(Action_BeAttack, "BeAttack"));
	m_SpineActionName.insert(std::make_pair(Action_Skill, "Skill"));
}