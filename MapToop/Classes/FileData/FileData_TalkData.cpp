#include "FileData_TalkData.h"
#include "Tool/TabFileReader.h"
#include "Tool/StringHelper.h"
FileDataTalkData::FileDataTalkData()
{
	m_Cfg.clear();
}

FileDataTalkData::~FileDataTalkData()
{

}
FileDataTalkData * FileDataTalkData::m_FileDataTalkData = nullptr;

FileDataTalkData * FileDataTalkData::GetInstance()
{
	if (m_FileDataTalkData == nullptr)
	{
		m_FileDataTalkData = new FileDataTalkData();
	}
	return m_FileDataTalkData;
}

bool FileDataTalkData::LoadDataFromFile(const char* fileName)
{
	TFReader file = TFReader::create(fileName);
	if (!file)
		return false;
	m_Cfg.clear();
	int count = (int)file.GetItemCount();
	for (int i = 0; i < count; ++i) {
		CfgTalkData cfg;
		file.Read(i, "Id", cfg.Id);
		file.Read(i, "Content", cfg.Content);
		file.Read(i, "NextId", cfg.NextId);
		file.Read(i, "TalkType", cfg.TalkType);
		file.Read(i, "IsStartTalk", cfg.IsStartTalk);
		file.Read(i, "IsScenario", cfg.IsScenario);
		file.Read(i, "UseHeadNpcId", cfg.UseHeadNpcId);
		m_Cfg.insert(make_pair(cfg.Id, cfg));
	}
	return true;
}
const CfgTalkData* FileDataTalkData::GetCfg(int Id)
{
	if (m_Cfg.find(Id) != m_Cfg.end())
	{
		return &m_Cfg.at(Id);
	}
	return nullptr;
}