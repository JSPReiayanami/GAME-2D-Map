#ifndef __Interface_GameChoice_H__
#define __Interface_GameChoice_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Interface_GameChoice : public Interface_Entity
{
public:
	/*
	@构造,析构
	*/
	Interface_GameChoice();
	~Interface_GameChoice();
	/*
	@场景建立
	*/
	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(Interface_GameChoice);
	ListView * m_ListChoice;
	/*
	@设置自己的名字
	*/
	void SetInterfaceName();
	/*
	@初始化资源
	*/
	void InitSrcLayout();
	/*
		@初始化控件
	*/
	void InitWidget();
	/*
	@初始化按钮的回调事件
	*/
	void InitButtonEvent();
	/*
	@按钮的回调事件
	*/
	void ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void BackEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@初始化关卡显示
	*/
	void InitLevelInList();
	//
	void AddChoiceLevel(map<int, bool> level_data);
	
};
#endif // !__MAINSCENE__