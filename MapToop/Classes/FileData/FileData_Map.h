#ifndef __FileData_Map_H__
#define __FileData_Map_H__
#include <string>
#include <vector>
#include <map>
struct CfgMap{
	int Id;
	std::string MapSrcPath;
	std::string MapDataPath;
};

class FileDataMap
{
public:
	FileDataMap();
	~FileDataMap();
	static FileDataMap * GetInstance();
	bool LoadDataFromFile(const char* fileName);
	const CfgMap* GetCfg(int Id);
private:
	static FileDataMap * m_FileDataMap;
	std::map<int, CfgMap> m_Cfg;
};


#endif // __FileData_Npc_H__