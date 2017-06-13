#ifndef __Interface_Main_H__
#define __Interface_Main_H__
#include "Head.h"
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Entity/Interface_Entity.h"
#else
#include "Interface_Entity.h"
#endif//判断路径的引用
class Interface_Main : public Interface_Entity
{
public:
	/*
	@构造,析构
	*/
	Interface_Main();
	~Interface_Main();
	/*
	@场景建立
	*/
	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(Interface_Main);
	/*
		@设置自己的名字
	*/
	void SetInterfaceName();
	/*
		@初始化资源
	*/
	void InitSrcLayout();
	/*
		@初始化按钮的回调事件
	*/
	void InitButtonEvent();
	/*
		@按钮的回调事件
	*/
	void ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@音效按键
	*/
	void MusicButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@购买
	*/
	void BuyButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
};
#endif // !__MAINSCENE__