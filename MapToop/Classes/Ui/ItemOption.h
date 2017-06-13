#ifndef __ItemOption_H__
#define __ItemOption_H__
#include <vector>
#include <map>
#include "cocos2d.h"
#include <iostream>
#include <random> 
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <time.h>
#include <math.h>
using namespace  cocos2d::ui;
using namespace cocostudio;
using namespace std;
USING_NS_CC;


class ItemOption : public Layout
{
public:
	ItemOption();
	~ItemOption();
	/*
	@固定创建方法
	*/
	virtual bool init();

	CREATE_FUNC(ItemOption);
	Layout * m_Layout;
	Layout * m_ViewLayout;
	Layout * m_Button1;
	Layout * m_Button2;
	Vec2 m_Vec2Last;
	Vec2 m_Vec2Cur;
	bool m_IsRun;
	int m_CurDir;
};
#endif // !__ItemOption_H__