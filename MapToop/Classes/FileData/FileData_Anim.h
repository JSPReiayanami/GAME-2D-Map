#ifndef __FileData_Anim_H__
#define __FileData_Anim_H__
#include <string>
#include <vector>
#include <map>
struct CfgAnim{
	int Id;
	float AnimSpeed;
	int FAnimNum;
	int LRAnimNum;
	int BAnimNum;
	std::string FAnim;
	std::string LRAnim;
	std::string BAnim;
};

struct CfgSpineAnim {
	int Id;
	std::string SpineJson;
	std::string SpineRes;
	float SpineScale;
};

class FileDataAnim
{
public:
	FileDataAnim();
	~FileDataAnim();
	static FileDataAnim * GetInstance();
	bool LoadDataFromFile(const char* fileName);
	bool LoadDataFromFileSpine(const char* fileName);
	const CfgAnim* GetCfg(int Id);
	const CfgSpineAnim* GetSpineCfg(int Id);
	std::string GetActionName(ActionType actionType);
private:
	void InitActionName();
	static FileDataAnim * m_FileDataAnim;
	std::map<int, CfgAnim> m_Cfg;
	std::map<int, CfgSpineAnim> m_SpineCfg;
	std::map<ActionType, std::string> m_SpineActionName;
};


#endif // __FileData_Npc_H__