#include "FileData_Anim.h"
#include "Tool/TabFileReader.h"
#include "Tool/StringHelper.h"
FileDataAnim::FileDataAnim()
{
	m_Cfg.clear();
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
const CfgAnim* FileDataAnim::GetCfg(int Id)
{
	if (m_Cfg.find(Id) != m_Cfg.end())
	{
		return &m_Cfg.at(Id);
	}
	return nullptr;
}