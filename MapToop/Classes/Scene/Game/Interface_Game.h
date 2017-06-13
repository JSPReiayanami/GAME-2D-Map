#ifndef __Interface_Game_H__
#define __Interface_Game_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Ui_Bag;
class Game_Item;
class Interface_Game : public Interface_Entity
{
public:
	/*
	@构造,析构
	*/
	Interface_Game();
	~Interface_Game();
	/*
	@场景建立
	*/
	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(Interface_Game);
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
	/*
		@初始化背包
	*/
	void InitBag();
	/*
		@属性
	*/
	Layout * m_LayoutBag;//背包层
	Layout * m_LayoutProp;//道具层
	Layout * m_LayoutPropBig;//道具层放大的
	Layout * m_LayoutSpecialProp;//道具层放大的
	Layout * m_LayoutBaffle;//遮层
	Layout * m_LayoutButton;//按钮层
	Text   * m_TextLevel;//关卡标题
	ImageView * m_Background;//背景图片
	Ui_Bag * m_Bag;//背包
	/*
		@初始化关卡布局根据关卡
	*/
	void InitGameWithLevel();
	/*
		@添加一个道具到地图上
	*/
	void AddGameItemToMap(Game_Item * g_item);
	/*
		@添加一个道具到放大的地图层
		@物品
		@地图资源路径
	*/
	void AddGameItemToBigMap(Game_Item * g_item);
	/*
		@开启放大场景
	*/
	void OpenBigMap(const string & map_src);
	/*
		@关闭放大的场景
	*/
	void CloseBigMap();
	/*
		@大场景的关闭按钮事件
	*/
	void BigButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@添加一个东西进特殊道具层
	*/
	void AddPropToLayoutSpecialProp(Node * prop);
	/*
		@刷新关卡标题
	*/
	void RefreshGameTitle(const string & title);
	/*
		@过关提示
	*/
	void LevelNoticeEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
};
#endif // !__MAINSCENE__