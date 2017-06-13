#ifndef __FileData_Npc_H__
#define __FileData_Npc_H__
#include <string>
#include <vector>
#include <map>
struct CfgNpc{
	int Id;
	int ModelImageId;
	std::string Name;
	std::vector<int> Behavior;
	float AnimSpeed;
	std::vector<int> TalksId;
};

class FileDataNpc
{
public:
	FileDataNpc();
	~FileDataNpc();
	static FileDataNpc * GetInstance();
	bool LoadDataFromFile(const char* fileName);
	const CfgNpc* GetCfg(int npcId);
	const std::map<int, CfgNpc> * GetAllCfg();
private:
	static FileDataNpc * m_FileDataNpc;
	std::map<int, CfgNpc> m_Cfg;
};


#endif // __FileData_Npc_H__