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
	@�̶���������
	*/
	virtual bool init();

	CREATE_FUNC(ListView3D);
	/*
	@����ί����
	@˭����������,˭����ί����
	*/
	void SetDelegete(ListView3D_Delegete * delegete);
	/*
		@�趨һ��page���
	*/
	void SetOnePageSize(Size size);
	Size GetOnePageSize();
	/*
	@�趨����ҳ��
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
	Size m_OnePageSize;//ÿһ��page�Ĵ�С
	int m_ShowNum;//��ʾ����
	ListView3D_Delegete * m_Delegete;//ί��
	int m_AllNum;//һ������ҳ��
	map<int, Layout *> m_AllItem;
	//===
	ListView * m_ListView;
	int  m_CurIndex;
	Vec2 m_LastInnerVec;
	
};
#endif // !__3DListView_H__