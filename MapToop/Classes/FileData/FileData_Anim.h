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

class FileDataAnim
{
public:
	FileDataAnim();
	~FileDataAnim();
	static FileDataAnim * GetInstance();
	bool LoadDataFromFile(const char* fileName);
	const CfgAnim* GetCfg(int Id);
private:
	static FileDataAnim * m_FileDataAnim;
	std::map<int, CfgAnim> m_Cfg;
};


#endif // __FileData_Npc_H__