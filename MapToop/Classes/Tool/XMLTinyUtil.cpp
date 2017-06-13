#include "XMLTinyUtil.h"
tinyxml2::XMLElement* XMLTinyUtil::loadRootElement(){


	string xmlfilepath = CCFileUtils::getInstance()->fullPathForFilename(path().c_str());
	ssize_t len = 0;
	unsigned char *data = CCFileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	document()->Parse((char*)data, len);

	//xml文件路径
	//std::string filePath = FileUtils::getInstance()->fullPathForFilename(path().c_str());
	//xmlDoc
	//document()->LoadFile(filePath.c_str());
	//得到根节点
	tinyxml2::XMLElement *rootEle = document()->RootElement();
	tinyxml2::XMLElement *itemEle = rootEle->FirstChildElement();

	return itemEle;
}


XMLTinyUtil* XMLTinyUtil::create(string path)
{
	auto obj = new XMLTinyUtil();
	if (obj)
	{
		//	obj->autorelease();
		obj->path(path);
		obj->document(new tinyxml2::XMLDocument());
	}
	else
		delete obj;
	return obj;
}
XMLTinyUtil::XMLTinyUtil()
{
	path("");
}
XMLTinyUtil::~XMLTinyUtil()
{
	deleteDocument();
}

void XMLTinyUtil::deleteDocument()
{
	delete document();
}
