#ifndef __FileData_Behaviour_H__
#define __FileData_BehaviourH__
#include <string>
#include <vector>
#include <map>

struct CfgBeMonitor{
	int Id;
	int Loop;
	int MonitorLenth;
	int MonitorId;
	int MonitorDir;
};

struct CfgBeRandomRun{
	int Id;
	int Loop;
	int OneRunLenth;
	int RunLenth;
	int RunOdds;
};

struct CfgBeTrace{
	int Id;
	int Loop;
	int TraceLenth;
	int TraceId;
};
class FileDataBehaviour
{
public:
	FileDataBehaviour();
	~FileDataBehaviour();
	static FileDataBehaviour * GetInstance();
	bool LoadDataFromFile();
	const void* GetCfg(int beId,int & type);
private:
	bool LoadBeMonitor();
	bool LoadBeRandomRun();
	bool LoadBeTrace();
	static FileDataBehaviour * m_FileDataBehaviour;
	std::map<int, CfgBeMonitor> m_CfgBeMonitor;
	std::map<int, CfgBeRandomRun> m_CfgBeRandomRun;
	std::map<int, CfgBeTrace> m_CfgBeTrace;
};


#endif // __FileData_BehaviourH__