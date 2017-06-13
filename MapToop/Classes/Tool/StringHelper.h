#ifndef _StringHelperL_H_
#define _StringHelperL_H_
#include "Config/Head.h"

namespace StringHelper
{
	/*
	@splitString函数的参数说明：
	@返回值是分割后字符串数组大小
	@strSrc 原始字符串
	@strDelims 自定义的分割字符串的分割数组
	*/
	vector<std::string>  splitString(const std::string & strSrc, const std::string& strDelims);
	/*
	@splitString函数的参数说明：
	@返回值是分割后字符串数组大小
	@strSrc 原始字符串
	@strDelimsBegin 自定义的分割字符串的分割数组
	@strDelimsEnd 自定义的分割字符串的分割数组
	*/
	vector<std::string>  splitString(const std::string & strSrc, const std::string& strDelimsBegin, const std::string& strDelimsEnd);
	/*
		@获取指定路径下的所有文件
	*/
	void getAllFileInPath(const std::string & path, vector<std::string> & files);
	/*
		@获取指定路径下的所有指定类型的文件
	*/
	void getAllFileInPathWithType(const std::string & path, vector<std::string> & files, const std::string & type);
	/*
		@字符串替换
	*/
	std::string& replace_all(std::string& str, const std::string& old_value, const std::string& new_value);
	std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);
	/*
		@去掉某个点后面的字符返回新的，但是不会改变原本的
	*/
	std::string deleteStrTypeBack(std::string& str, const std::string& type);

	struct Delta     //日期差结构体
	{
		Delta()
		{
			days = 0;
			hours = 0;
			minutes = 0;
			seconds = 0;
		}
		short days;      //相差的天数
		short hours;     //相差的小时数
		short minutes;   //相差的分钟数
		short seconds;   //相差的秒数
	};

	//日期类
	class SDate
	{
	public:
		SDate();   //无参构造函数
		SDate(short year, short month, short day, short hour, short minute, short second);
		~SDate();
	private:
		short year;
		short month;
		short day;
		short hour;
		short minute;
		short second;

	private:
		int IsLeapYear(short year);  //是否是闰年
		int DaysOfOneYear(SDate sd); //该年天数
		int SDateToAbsDays(SDate sd);//日期转换成绝对天数
		int DaysBetweenTwoDate(SDate sd1, SDate sd2);   //两个日期的天数差
	public:
		Delta TimeBetweenTwoDate(SDate sd1, SDate sd2);   //两个日期的时间差
		int TimeBetweenTwoS(SDate sd1, SDate sd2);   //两个日期的时间差秒
	};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//时间相关
	//本函数用于获取当前系统的时间，使用前请传入获取时间的string的引用
	void getSystemTime(string & stime);

	time_t getSystemTime();

	time_t convert_string_to_time_t(const std::string & time_string);

	double timeDelete(time_t start, time_t end);
#endif//(CC_PLATFORM_WIN32)


	//long 转换为 时间 第X天 XX:XX:XX
	string longToStringTime(long time, int type = 0);
}

#endif
