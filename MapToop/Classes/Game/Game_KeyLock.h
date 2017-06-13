#ifndef __Game_KeyLock_H__
#define __Game_KeyLock_H__
#include "Config/Head.h"
class Game_KeyLock : public Layout
{
public:
	Game_KeyLock();
	~Game_KeyLock();
	/*
	@固定创建方法
	*/
	static Game_KeyLock * CreateWithNumData(const vector<int> & key_num,const int & text_size,const string & slider_src,const string & back_src = "",const float & slider_gap = 0);
	static Game_KeyLock * CreateWithId(const int & _id);
	virtual bool init();
	CREATE_FUNC(Game_KeyLock);
	/*
		@属性
	*/
	vector<int> m_KeyNum;
	vector<int> m_CurKey;
	GameItemData * m_Data;
	bool m_IsOpen;
	CC_SYNTHESIZE(int, m_Id, MyId);
	CC_SYNTHESIZE(Vec2,m_StartPos,MyStartPos);
	CC_SYNTHESIZE(int, m_TextSize, MyTextSize);
	CC_SYNTHESIZE(string, m_SliderSrc, MySliderSrc);
	CC_SYNTHESIZE(string, m_BackSrc, MyBackSrc)
	CC_SYNTHESIZE(float, m_SliderGap, MySliderGap)
	SEL_CallFunc_Bool m_OutFunction;
	Ref * m_OutFunctioner;
	/*
		@初始化密码
	*/
	void InitKeyNum(const vector<int> & key_num);
	/*
		@刷新显示
	*/
	void RefreshView();
	/*
		@接收滑动数字的改变
	*/
	void GetSliderNumChange(int num,Node * slider_num);
	/*
		@判断当前的数字和密码是否一致
	*/
	void JudgeIsOpen();
	/*
		@注册解密成功或者失败的回调事件
	*/
	void RegisterUnlockCallback(SEL_CallFunc_Bool callback, Ref * register_user);
	/*
		@调用外部函数
	*/
	void CallOutFunction();
	/*
		设置Id
	*/
	void SetKeyLockId(const int & _id);
	/*
		@关闭密码
	*/
	void CloseKeyLock();
	/*
		@点击后关闭密码
	*/
	void TouchCloseKeyLockEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
		 
};
#endif // !__MAINSCENE__