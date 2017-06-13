#include "FileData_Behaviour.h"
#include "Tool/TabFileReader.h"
#include "Tool/StringHelper.h"
FileDataBehaviour::FileDataBehaviour()
{
	m_CfgBeMonitor.clear();
	m_CfgBeRandomRun.clear();
}

FileDataBehaviour::~FileDataBehaviour()
{

}
FileDataBehaviour * FileDataBehaviour::m_FileDataBehaviour = nullptr;

FileDataBehaviour * FileDataBehaviour::GetInstance()
{
	if (m_FileDataBehaviour == nullptr)
	{
		m_FileDataBehaviour = new FileDataBehaviour();
	}
	return m_FileDataBehaviour;
}

bool FileDataBehaviour::LoadDataFromFile()
{
	bool isSu = LoadBeMonitor();
	if (isSu == false) return isSu;
	isSu = LoadBeRandomRun();
	if (isSu == false) return isSu;
	isSu = LoadBeTrace();
	if (isSu == false) return isSu;
	return true;
}
const void * FileDataBehaviour::GetCfg(int beId,int & type)
{
	type = -1;
	if (m_CfgBeMonitor.find(beId) != m_CfgBeMonitor.end())
	{
		type = 1;
		return &m_CfgBeMonitor.at(beId);
	}
	if (m_CfgBeRandomRun.find(beId) != m_CfgBeRandomRun.end())
	{
		type = 2;
		return &m_CfgBeRandomRun.at(beId);
	}
	if (m_CfgBeTrace.find(beId) != m_CfgBeTrace.end())
	{
		type = 3;
		return &m_CfgBeTrace.at(beId);
	}
	return nullptr;
}

bool FileDataBehaviour::LoadBeMonitor()
{
	TFReader file = TFReader::create("FileData/BeMonitorNodes.txt");
	if (!file)
		return false;
	m_CfgBeMonitor.clear();
	int count = (int)file.GetItemCount();
	for (int i = 0; i < count; ++i) {
		CfgBeMonitor cfg;
		file.Read(i, "Id", cfg.Id);
		file.Read(i, "Loop", cfg.Loop);
		file.Read(i, "MonitorDir", cfg.MonitorDir);
		file.Read(i, "MonitorId", cfg.MonitorId);
		file.Read(i, "MonitorLenth", cfg.MonitorLenth);
		m_CfgBeMonitor.insert(make_pair(cfg.Id, cfg));
	}
	return true;
}
bool FileDataBehaviour::LoadBeRandomRun()
{
	TFReader file = TFReader::create("FileData/BeRandomRun.txt");
	if (!file)
		return false;
	m_CfgBeRandomRun.clear();
	int count = (int)file.GetItemCount();
	for (int i = 0; i < count; ++i) {
		CfgBeRandomRun cfg;
		file.Read(i, "Id", cfg.Id);
		file.Read(i, "Loop", cfg.Loop);
		file.Read(i, "OneRunLenth", cfg.OneRunLenth);
		file.Read(i, "RunLenth", cfg.RunLenth);
		file.Read(i, "RunOdds", cfg.RunOdds);
		m_CfgBeRandomRun.insert(make_pair(cfg.Id, cfg));
	}
	return true;
}

bool FileDataBehaviour::LoadBeTrace()
{
	TFReader file = TFReader::create("FileData/BeTrace.txt");
	if (!file)
		return false;
	m_CfgBeTrace.clear();
	int count = (int)file.GetItemCount();
	for (int i = 0; i < count; ++i) {
		CfgBeTrace cfg;
		file.Read(i, "Id", cfg.Id);
		file.Read(i, "Loop", cfg.Loop);
		file.Read(i, "TraceId", cfg.TraceId);
		file.Read(i, "TraceLenth", cfg.TraceLenth);
		m_CfgBeTrace.insert(make_pair(cfg.Id, cfg));
	}
	return true;
}