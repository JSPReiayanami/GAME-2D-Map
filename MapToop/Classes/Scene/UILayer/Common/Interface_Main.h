#ifndef __Interface_Main_H__
#define __Interface_Main_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"

class Interface_Main : public Interface_Entity
{
public:
	Interface_Main();
	~Interface_Main();
	CREATE_FUNC(Interface_Main);
	CREATEPush_FUNC(Interface_Main, "Interface_Main");
	bool init();
	/*
	@设置自己的名字
	*/
	void SetInterfaceName(const string & name = "Interface_Main");
	/*
	@初始化资源
	*/
	void InitSrcLayout();
	/*
	@初始化控件
	*/
	void InitWidget();
	/*
	@初始化按钮回调
	*/
	void InitButtonEvent();

	void OnIntoScene();
	void OnExitScene();
	void OnClose();
	virtual void update(float delta);
public:
	Layout * m_LayoutView;
	Layout * m_LayoutButton;
	Text * m_TextTime;
};


#endif //!__Interface_Main_H__