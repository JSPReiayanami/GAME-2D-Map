#include "TestToLua.h"
std::string testlua::testluatxt = "";
string testlua::getname()
{
	return "testlua";
}
void testlua::setname(string txt)
{
	testluatxt = txt;
}