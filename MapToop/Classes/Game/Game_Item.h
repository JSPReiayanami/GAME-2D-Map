#ifndef __Game_Item_H__
#define __Game_Item_H__
#include "Config/Head.h"
class Game_Item : public Layout
{
public:
	Game_Item();
	~Game_Item();
	/*
		@固定创建方法
	*/
	static Game_Item * CreateWithId(const int &_id);
	static Game_Item * CreateWithPropData(const PropData * p_data);
	virtual bool init();
	CREATE_FUNC(Game_Item);
	/*
		@根节点
	*/
	Button * m_Sprite;//显示图形
	GameItemData * m_Data;
	string   m_AnimationSrc;//动画资源
	CC_SYNTHESIZE(int, m_Id, MyId);
	string m_Value;
	void setMyValue(string s_value);
	string getMyValue();
	CC_SYNTHESIZE(int, m_IsCanGetKey, MyIsCanGetKey);
	CC_SYNTHESIZE(Game_Item *, m_BigItem, MyBigItem);//放大的东西的指针
	/*
		@设置自己id
	*/
	void SetId(const int & _id);
	/*
		@被点击到事件
	*/
	void TouchEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@道具改变
	*/
	void ChageThisWithPropData(PropData * p_data);
	/*
		@根据类型点击到了的类型分下下去各自的事件
	*/
	void DoActionWithType();
	/*
		GameItemType_Door = 1,	//门
		GameItemType_Key,	// 钥匙
		GameItemType_TouchChange,//点击后改变道具类
		GameItemType_TouchGet ,//点击获取类道具并且自己不消失的,并且获取到的道具消失
		GameItemType_TouchMoveLeft,//点击左移类道具
		GameItemType_TouchMoveRight,//点击右移类道具
		GameItemType_TouchGetAndNil,	//上点击获取后，然后消失
		GameItemType_View,	//单纯展示用的
		GameItemType_MoveWithPointsAndTouchGetAndNil,	//根据一些点来移动，然后点击后会消失
		GameItemType_TouchAddToMapAndNil,	//点击后增加一个道具到地图上
		GameItemType_TouchBigAddToMap,	//点击后增加一个道具到放大后的地图上
		GameItemType_BigTouchGetAndNil,	//大地图上点击获取后，然后消失
		GameItemType_BigTouchChange,	//大地图上点击后，后变为其他东西
		GameItemType_BigTouchGetAndChange,	//大地图上点击后获取到道具后变为其他东西
		GameItemType_TouchPopLock,//点击弹出一个密码锁
		GameItemType_TouchChangeAndSetPosVlaue,//点击后改变道具类并且设置位置和值(小地图或大地图都适用)
		GameItemType_TouchAndGetPropWithFriendId,//点击后根据友元id获取到不同的道具，然后自己消失
		GameItemType_TouchGetAndChange,//小地图获取然后变为其他东西
		GameItemType_BigTouchChangeBigItemOrGet,//大地图上，点击后改变外面放大物品，然后判断是否能获取到东西
		GameItemType_TouchCombinationAndChangeAdd,//大地图上，点击后改变外面放大物品，然后判断是否能获取到东西,最后添加东西到小地图上
		GameItemType_BigTouchChangeOutAndChangeSelf,//大地图上，点击后改变外面放大物品，然后改变自己,上一个大地图的值不会继承
		GameItemType_TimeItem,//时间类道具
		GameItemType_CountdownItem,//倒计时道具
		GameItemType_Max,//最大的枚举值，不用
	*/
	void DoorAction();
	void KeyAction();
	void TouchChangeAction();
	void TouchGetAction();
	void TouchMoveLeftAction();
	void TouchMoveRightAction();
	void TouchGetAndNil();
	void TouchAddToMapAndNil();
	void TouchBigAddToMap();
	void BigTouchGetAndNil();
	void BigTouchChange();
	void BigTouchGetAndChange();
	void TouchPopLock();
	void TouchChangeAndSetPosVlaue();
	void TouchAndGetPropWithFriendId();
	void TouchGetAndChange();
	void BigTouchChangeBigItemOrGet();
	void TouchCombinationAndChangeAdd();
	void BigTouchChangeOutAndChangeSelf();
	void TimeItem();

	/*
		============================================
	*/
	/*
		@判断道具的前置条件
	*/
	bool IsCanActionForFriendId();
	/*
		@通知放大的场景中，某种道具已经获取完毕
		@
	*/
	void NoticeBigMapGetBagItemAndNil(const int & _id, const Vec2 _vec2);
	/*
		@通知大地图的某种东西改变
	*/
	void NoticeBigMapGameItemChange(const int & _id, const Vec2 & _vec2,const string & change_data);
	/*
		@把数据拿出来生成道具放到大地图上
	*/
	void AddGameItemToBigMaoWithValueAndStartIndex(const string & item_value,const int & strart_index);
	/*
		@!GameItemType_TouchPopLock! 回调，得到密码已经解锁
	*/
	void MyLockIsOpen(bool _is);
	/*
		============================================
	*/
	//===特殊的会动的道具==
	/*
		@根据类型分发各自的移动或者其他事件
	*/
	void RunActionWithType();
	/*
		@子事件
	*/
	void RunMoveWithPoints();
	/*
		@倒计时事件
	*/
	void RunCountdownItem();
	/*
		@播放声效
	*/
	void PlaySelfEffect();
	/*
		@帧函数事件
	*/
	void update(float delta);
	/*
		@倒计时
	*/
	void CountdownItem(float delta);
	/*
		@身上的特效
	*/
	void InitEffects();

};
#endif // !__MAINSCENE__