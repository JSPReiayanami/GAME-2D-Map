#ifndef __3DListView_H__
#define __3DListView_H__
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

class ListView3D_Delegete
{
public:
	ListView3D_Delegete();
	~ListView3D_Delegete();

public:
	virtual Layout * GetPageLayoutWithIndex(int index);
	virtual void CurShowPage(Layout * layout,int index);
};

class ListView3D : public Layout
{
public:
	ListView3D();
	~ListView3D();
	/*
	@固定创建方法
	*/
	virtual bool init();

	CREATE_FUNC(ListView3D);
	/*
	@定义委托者
	@谁添加这个界面,谁就是委托者
	*/
	void SetDelegete(ListView3D_Delegete * delegete);
	/*
		@设定一个page多大
	*/
	void SetOnePageSize(Size size);
	Size GetOnePageSize();
	/*
	@设定几个页面
	*/
	void SetAllPageNum(int num);
protected:
private:
	void ListViewCallFunc(Ref* sender, ListView::EventType  pageType);
	void ScrollCallFunc(Ref* sender, ScrollView::EventType  pageType);
	void UpdateView();
	void ScaleWithInnerPosition(Vec2 vec);
	void CamberWithInnerPosition(Vec2 vec);
	void onEnter();
	void onExit();
	void update(float delta);
private:
	Size m_OnePageSize;//每一个page的大小
	int m_ShowNum;//显示几个
	ListView3D_Delegete * m_Delegete;//委托
	int m_AllNum;//一共几个页面
	map<int, Layout *> m_AllItem;
	//===
	ListView * m_ListView;
	int  m_CurIndex;
	Vec2 m_LastInnerVec;
	
};
#endif // !__3DListView_H__