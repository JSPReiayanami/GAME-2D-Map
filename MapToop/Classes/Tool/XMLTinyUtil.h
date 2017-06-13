#ifndef _XMLTINYUTIL_H_
#define _XMLTINYUTIL_H_
#include "tinyxml2.h"
#include "Head.h"
using namespace tinyxml2;
class XMLTinyUtil : public Ref
{
public:
	tinyxml2::XMLElement* loadRootElement();
	static XMLTinyUtil* create(string path);
	XMLTinyUtil();
	~XMLTinyUtil();
	void deleteDocument();
private:
	MENBER(string, path);
	MENBER(tinyxml2::XMLDocument *, document);

};

#endif




