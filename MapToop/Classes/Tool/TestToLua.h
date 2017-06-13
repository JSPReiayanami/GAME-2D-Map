#ifndef __testlua__
#define __testlua__
#include <iostream>
#include <string>
#include <string.h>
using namespace std;
class testlua
{
private:
	static std::string testluatxt;
public:
	static std::string getname();
	static void setname(string txt);
};

#endif