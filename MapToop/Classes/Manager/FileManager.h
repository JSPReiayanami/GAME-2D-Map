#ifndef __FileManager_H__
#define __FileManager_H__
#include "Config/Head.h"
class FileManager
{
public:
	FileManager();
	~FileManager();
	static FileManager * GetInstance();
	bool LoadFileData();
private:
	static FileManager * m_FileManager;
};



#endif // __FileManager_H__