#ifndef __FileData_TalkData_H__
#define __FileData_TalkData_H__
#include <string>
#include <vector>
#include <map>
struct CfgTalkData{
	int Id;
	std::string Content;
	int NextId;
	int TalkType;
	std::string IsStartTalk;
	bool IsScenario;
	int UseHeadNpcId;
};

class FileDataTalkData
{
public:
	FileDataTalkData();
	~FileDataTalkData();
	static FileDataTalkData * GetInstance();
	bool LoadDataFromFile(const char* fileName);
	const CfgTalkData* GetCfg(int Id);
private:
	static FileDataTalkData * m_FileDataTalkData;
	std::map<int, CfgTalkData> m_Cfg;
};


#endif // __FileData_Npc_H__