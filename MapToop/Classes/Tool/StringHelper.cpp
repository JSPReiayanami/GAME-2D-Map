#include "StringHelper.h"

/*
	@splitString函数的参数说明：
	@返回值是分割后字符串数组大小
	@strSrc 原始字符串
	@strDelims 自定义的分割字符串的分割数组
*/
vector<string>  StringHelper::splitString(const string & strSrc, const std::string& strDelims)
{
	typedef std::string::size_type ST;
	string delims = strDelims;
	vector<string> strDest;
	std::string STR;
	if (delims.empty()) delims = "/n/r";


	ST pos = 0, LEN = strSrc.size();
	while (pos < LEN){
		STR = "";
		while ((delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN)) ++pos;
		if (pos == LEN) return strDest;
		while ((delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN)) STR += strSrc[pos++];
		//std::cout << "[" << STR << "]";  
		if (!STR.empty()) strDest.push_back(STR);
	}
	return strDest;
}

/*
@splitString函数的参数说明：
@返回值是分割后字符串数组大小
@strSrc 原始字符串
@strDelimsBegin 自定义的分割字符串的分割数组
@strDelimsEnd 自定义的分割字符串的分割数组
*/
vector<std::string>  StringHelper::splitString(const std::string & strSrc, const std::string& strDelimsBegin, const std::string& strDelimsEnd)
{
	string str_C = strSrc;
	vector<string> strDest;
	strDest.clear();
	while (true)
	{
		size_t index_b = 0;
		size_t index_e = 0;
		index_b = str_C.find(strDelimsBegin);
		index_e = str_C.find(strDelimsEnd);
		if (index_e <= index_b) break;
		//--获取一段
		string one_c = str_C.substr(index_b + strDelimsBegin.size(), index_e - index_b - strDelimsEnd.size());
		str_C = str_C.substr(index_e + strDelimsEnd.size(), str_C.length());
		strDest.push_back(one_c);
	}
	return strDest;
}
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//===
void __getAllFileInPath(const std::string & path, vector<std::string> & files)
{
	//文件句柄
	intptr_t  hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	cout << path.c_str() << endl;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		_findnext(hFile, &fileinfo);
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				cout << fileinfo.name << endl;
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					__getAllFileInPath(p.assign(path).append("\\").append(fileinfo.name), files);
				continue;
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
//===
void StringHelper::getAllFileInPath(const std::string & path, vector<std::string> & files)
{
	if (path == "") return;
	size_t head = path.length() + 1;//因为还有2个//的位置
	__getAllFileInPath(path, files);
	//--去头
	for (int index = 0; index < files.size(); index++)
	{
		if (files[index].length() <= head) continue;
		files[index] = files[index].substr(head, files[index].length());
		files[index] = replace_all_distinct(files[index], "\\", "/");

	}
}
/*
@获取指定路径下的所有指定类型的文件
*/
void StringHelper::getAllFileInPathWithType(const std::string & path, vector<std::string> & files, const std::string & type)
{
	getAllFileInPath(path, files);
	vector<std::string>::iterator ite = files.begin();
	while (ite != files.end())
	{
		vector<string> allDec = splitString((*ite), ".");
		if (allDec.back() != type){
			ite = files.erase(ite);
		}
		else
		{
			ite++;
		}
	}

}
#endif//(CC_PLATFORM_ANDROID || CC_PLATFORM_WIN32)

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//===
void __getAllFileInPath(const std::string & path, vector<std::string> & files)
{
	//文件句柄
	intptr_t  hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	cout << path.c_str() << endl;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		_findnext(hFile, &fileinfo);
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				cout << fileinfo.name << endl;
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					__getAllFileInPath(p.assign(path).append("\\").append(fileinfo.name), files);
				continue;
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
//===
void StringHelper::getAllFileInPath(const std::string & path, vector<std::string> & files)
{
	if (path == "") return;
	size_t head = path.length() + 1;//因为还有2个//的位置
	__getAllFileInPath(path, files);
	//--去头
	for (int index = 0; index < files.size(); index++)
	{
		if (files[index].length() <= head) continue;
		files[index] = files[index].substr(head, files[index].length());
		files[index] = replace_all_distinct(files[index], "\\", "/");

	}
}
/*
@获取指定路径下的所有指定类型的文件
*/
void StringHelper::getAllFileInPathWithType(const std::string & path, vector<std::string> & files, const std::string & type)
{
	getAllFileInPath(path, files);
	vector<std::string>::iterator ite = files.begin();
	while (ite != files.end())
	{
		vector<string> allDec = splitString((*ite), ".");
		if (allDec.back() != type){
			ite = files.erase(ite);
		}
		else
		{
			ite++;
		}
	}

}
#endif//(CC_PLATFORM_IOS)

std::string& StringHelper::replace_all(std::string& str, const std::string& old_value, const std::string& new_value)
{
	while (true)   {
		std::string::size_type pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else break;
	}
	return str;
}
std::string& StringHelper::replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value)
{
	for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length())   {
		if ((pos = str.find(old_value, pos)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

/*
@去掉某个点后面的字符返回新的，但是不会改变原本的
*/
std::string StringHelper::deleteStrTypeBack(std::string& str, const std::string& type)
{
	std::size_t type_pos = str.find(type);
	if (type_pos != 0 && type_pos < str.length()){
		return str.substr(0, type_pos);
	}
	return str;
}

using namespace StringHelper;
SDate::SDate()
{
	year = 0;
	month = 0;
	day = 0;
	hour = 0;
	minute = 0;
	second = 0;
}

SDate::SDate(short year, short month, short day, short hour, short minute, short second)
{
	this->year = year;
	this->month = month;
	this->day = day;
	this->hour = hour;
	this->minute = minute;
	this->second = second;
}

SDate::~SDate()
{
}

int SDate::IsLeapYear(short year)
{
	return year % 4 == 0 && year % 100 || year % 400 == 0;
}

int SDate::DaysOfOneYear(SDate sd)
{
	switch (sd.month - 1) {
	case 11:
		sd.day += 30;
	case 10:
		sd.day += 31;
	case 9:
		sd.day += 30;
	case 8:
		sd.day += 31;
	case 7:
		sd.day += 31;
	case 6:
		sd.day += 30;
	case 5:
		sd.day += 31;
	case 4:
		sd.day += 30;
	case 3:
		sd.day += 31;
	case 2:
		sd.day += IsLeapYear(sd.year) ? 29 : 28;
	case 1:
		sd.day += 31;
	}
	return sd.day;
}

int SDate::SDateToAbsDays(SDate sd)
{
	int years = sd.year - 1;
	int days = years * 365 + years / 4 - years / 100 + years / 400;
	days += DaysOfOneYear(sd);
	return days;
}

int SDate::DaysBetweenTwoDate(SDate sd1, SDate sd2)   //天数差
{
	return SDateToAbsDays(sd2) - SDateToAbsDays(sd1);
}

Delta SDate::TimeBetweenTwoDate(SDate sd1, SDate sd2)  //两个日期的时间差
{
	Delta t_delta;
	t_delta.seconds = sd2.second - sd1.second;
	if (t_delta.seconds < 0){
		t_delta.seconds += 60;
		t_delta.minutes = -1;
	}
	t_delta.minutes += (sd2.minute - sd1.minute);
	if (t_delta.minutes < 0){
		t_delta.minutes += 60;
		t_delta.hours = -1;
	}
	t_delta.hours += (sd2.hour - sd1.hour);
	if (t_delta.hours < 0){
		t_delta.hours += 24;
		t_delta.days = -1;
	}
	t_delta.days = DaysBetweenTwoDate(sd1, sd2);
	// 	if (t_delta.days<0)  //输入日期有误
	// 		return -1;
	return t_delta;
}
int SDate::TimeBetweenTwoS(SDate sd1, SDate sd2)  //两个日期的时间差秒
{
	int d_year = sd1.year - sd2.year;
	int d_month = sd1.month - sd2.month;
	int d_days = sd1.day - sd2.day;
	int d_hour = sd1.hour - sd2.hour;
	int d_mines = sd1.minute - sd2.minute;
	int d_sec = sd1.second - sd2.second;
	int time_s = 0;
	//---
	if (d_year == 0 && d_month == 0 && d_hour == 0 && d_mines == 0 && d_days == 0 )
	{
		//--
		time_s = abs(d_sec);
	}
	else if (d_year == 0 && d_month == 0 && d_hour == 0 && d_days == 0)
	{
		time_s = abs(d_sec);
		time_s += abs(d_mines) * 60;
	}
	else if (d_year == 0 && d_month == 0 && d_days == 0)
	{
		time_s = abs(d_sec);
		time_s += abs(d_mines) * 60;
		time_s += abs(d_hour) * 3600;
	}
	else if (d_year == 0 && d_month == 0 )
	{
		time_s = abs(d_sec);
		time_s += abs(d_mines) * 60;
		time_s += abs(d_hour) * 3600;
		time_s += abs(d_days) * 3600 * 24;
	}
	else if (d_year == 0)
	{
		time_s = abs(d_sec);
		time_s += abs(d_mines) * 60;
		time_s += abs(d_hour) * 3600;
		time_s += abs(d_days) * 3600 * 24;
		time_s += abs(d_month) * 30 * 3600 * 24;
	}
	else{
		time_s = abs(d_sec);
		time_s += abs(d_mines) * 60;
		time_s += abs(d_hour) * 3600;
		time_s += abs(d_days) * 3600 * 24;
		time_s += abs(d_month) * 30 * 3600 * 24;
		time_s += abs(year) * 365 * 30 * 3600 * 24;
	}

	return time_s;
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//本函数用于获取当前系统的时间，使用前请传入获取时间的string的引用
void StringHelper::getSystemTime(string & stime) {
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X ", localtime(&t));
	stime = tmp;
}
time_t StringHelper::getSystemTime() {
	time_t t = time(0);
	localtime(&t);
	return t;
}
time_t StringHelper::convert_string_to_time_t(const std::string & time_string)
{
	struct tm tm1;
	time_t time1;
	int i = sscanf(time_string.c_str(), "%d/%d/%d %d:%d:%d",
		&(tm1.tm_year),
		&(tm1.tm_mon),
		&(tm1.tm_mday),
		&(tm1.tm_hour),
		&(tm1.tm_min),
		&(tm1.tm_sec),
		&(tm1.tm_wday),
		&(tm1.tm_yday));

	tm1.tm_year -= 1900;
	tm1.tm_mon--;
	tm1.tm_isdst = -1;
	time1 = mktime(&tm1);

	return time1;
}

double StringHelper::timeDelete(time_t start, time_t end) {
	double cost;
	cost = difftime(end, start);

	return cost;
}
#endif//(CC_PLATFORM_WIN32)


//long 转换为 时间 第X天 XX:XX:XX
string StringHelper::longToStringTime(long time, int type)
{
	int day = time / (60 * 60 * 24);
	long oneDayTime = (time % (60 * 60 * 24));
	int hours = oneDayTime / 3600;
	int minute = (oneDayTime - hours * 3600) / 60;
	int second = (oneDayTime - hours * 3600) % 60;
	string timeStr = "";
	if (type >= 3)
	{
		timeStr.append(StringUtils::format("%dDay ", day).c_str());
	}

	if (type >= 2)
	{
		if (hours > 0)
		{
			if (hours < 10)
			{
				timeStr.append("0");
			}
			timeStr.append(StringUtils::format("%d:", hours).c_str());
		}
		else
		{
			timeStr.append("00:");
		}
	}

	if (type >= 1)
	{
		if (minute > 0)
		{
			if (minute < 10)
			{
				timeStr.append("0");
			}
			timeStr.append(StringUtils::format("%d:", minute).c_str());
		}
		else
		{
			timeStr.append("00:");
		}
	}

	if (second > 0)
	{
		if (second<10)
		{
			timeStr.append("0");
		}
		timeStr.append(StringUtils::format("%d", second).c_str());
	}
	else
	{
		timeStr.append("00");
	}
	return timeStr;
}