#ifndef __Ui_BagItem_H__
#define __Ui_BagItem_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Ui_BagItem : public Interface_Entity
{
public:
	Ui_BagItem();
	~Ui_BagItem();
	static Ui_BagItem * CreateWithId(const int & _id);
	CREATE_FUNC(Ui_BagItem);
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
	CC_SYNTHESIZE(int, m_Value, MyValue);//物品的选中状态
	CC_SYNTHESIZE(bool, m_IsChoice, MyIsChoice);//物品的选中状态
	Button * m_ButtonItem;
	Text   * m_TextName;
	ImageView * m_ImageChoice;
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
		@刷新显示
	*/
	void RefreshView();
	/*
		@按钮的回调事件
	*/
	void TouchEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@设置是否被选中
	*/
	void SetIsChoice(bool _is);
};
#endif // !__Ui_BagItem_H__