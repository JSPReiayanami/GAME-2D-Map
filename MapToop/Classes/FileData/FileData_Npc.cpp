#include "FileData_Npc.h"
#include "Tool/TabFileReader.h"
#include "Tool/StringHelper.h"
FileDataNpc::FileDataNpc()
{
	m_Cfg.clear();
}

FileDataNpc::~FileDataNpc()
{

}
FileDataNpc * FileDataNpc::m_FileDataNpc = nullptr;

FileDataNpc * FileDataNpc::GetInstance()
{
	if (m_FileDataNpc == nullptr)
	{
		m_FileDataNpc = new FileDataNpc();
	}
	return m_FileDataNpc;
}

bool FileDataNpc::LoadDataFromFile(const char* fileName)
{
	TFReader file = TFReader::create(fileName);
	if (!file)
		return false;
	m_Cfg.clear();
	int count = (int)file.GetItemCount();
	for (int i = 0; i < count; ++i) {
		CfgNpc cfg;
		file.Read(i, "Id", cfg.Id);
		file.Read(i, "ModelImageId", cfg.ModelImageId);
		file.Read(i, "Name", cfg.Name);
		//==
		std::string BeStr = "";
		file.Read(i, "Behavior", BeStr);
		cfg.Behavior.clear();
		vector<string> BeStrs = StringHelper::splitString(BeStr, ",");
		for (auto & str : BeStrs)
		{
			cfg.Behavior.push_back(atoi(str.c_str()));
		}
		//==
		file.Read(i, "AnimSpeed", cfg.AnimSpeed);
		//==
		std::string TalkIdStr = "";
		file.Read(i, "TalkId", TalkIdStr);
		cfg.TalksId.clear();
		vector<string> TalkIdStrs = StringHelper::splitString(TalkIdStr, ",");
		for (auto & str : TalkIdStrs)
		{
			cfg.TalksId.push_back(atoi(str.c_str()));
		}
		m_Cfg.insert(make_pair(cfg.Id, cfg));
	}
	return true;
}
const CfgNpc* FileDataNpc::GetCfg(int npcId)
{
	if (m_Cfg.find(npcId) != m_Cfg.end())
	{
		return &m_Cfg.at(npcId);
	}
	return nullptr;
}

const std::map<int, CfgNpc> * FileDataNpc::GetAllCfg()
{
	return &m_Cfg;
}