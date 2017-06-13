#ifndef __Interface_Login_H__
#define __Interface_Login_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Interface_Login : public Interface_Entity
{
public:
	/*
		@构造,析构
	*/
	Interface_Login();
	~Interface_Login();
	/*
	@场景建立
	*/
	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(Interface_Login);
protected:
private:
	/*
		@设置自己的名字
	*/
	virtual void SetInterfaceName();
	/*
		@初始化按钮的回调事件
	*/
	void InitButtonEvent();
	/*
		@按钮的回调事件
	*/
	void ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
};
#endif // !__MAINSCENE__