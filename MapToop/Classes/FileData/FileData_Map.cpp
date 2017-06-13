#include "FileData_Map.h"
#include "Tool/TabFileReader.h"
#include "Tool/StringHelper.h"
FileDataMap::FileDataMap()
{
	m_Cfg.clear();
}

FileDataMap::~FileDataMap()
{

}
FileDataMap * FileDataMap::m_FileDataMap = nullptr;

FileDataMap * FileDataMap::GetInstance()
{
	if (m_FileDataMap == nullptr)
	{
		m_FileDataMap = new FileDataMap();
	}
	return m_FileDataMap;
}

bool FileDataMap::LoadDataFromFile(const char* fileName)
{
	TFReader file = TFReader::create(fileName);
	if (!file)
		return false;
	m_Cfg.clear();
	int count = (int)file.GetItemCount();
	for (int i = 0; i < count; ++i) {
		CfgMap cfg;
		file.Read(i, "Id", cfg.Id);
		file.Read(i, "ResName", cfg.MapSrcPath);
		file.Read(i, "DataName", cfg.MapDataPath);
		m_Cfg.insert(make_pair(cfg.Id, cfg));
	}
	return true;
}
const CfgMap* FileDataMap::GetCfg(int Id)
{
	if (m_Cfg.find(Id) != m_Cfg.end())
	{
		return &m_Cfg.at(Id);
	}
	return nullptr;
}