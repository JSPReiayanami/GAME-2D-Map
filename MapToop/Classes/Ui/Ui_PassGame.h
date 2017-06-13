#ifndef __Ui_PassGame_H__
#define __Ui_PassGame_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Ui_PassGame : public Interface_Entity
{
public:
	Ui_PassGame();
	~Ui_PassGame();
	CREATE_FUNC(Ui_PassGame);
	/*
	@固定创建方法
	*/
	virtual bool init();

	/*
	@根节点
	*/
	Layout * m_Root;//根节点
	string   m_LayerSrc;//界面资源
	CC_SYNTHESIZE(int, m_Id, MyId);//物品的id
	ListView * m_ListText;
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
	void TouchCloseEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
};
#endif // !__Ui_BagItem_H__