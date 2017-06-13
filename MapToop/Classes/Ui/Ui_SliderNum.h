#ifndef __Ui_SliderNum_H__
#define __Ui_SliderNum_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Controller_Square;
class Ui_SliderNum : public Interface_Entity
{
public:
	Ui_SliderNum();
	~Ui_SliderNum();
	CREATE_FUNC(Ui_SliderNum);
	/*
		@固定创建方法
	*/
	virtual bool init();

	/*
		@根节点
	*/
	Layout * m_Root;//根节点
	string   m_LayerSrc;//界面资源
	ImageView * m_ImageBackground;//背景
	Controller_Square * m_ControSquare;//控制器
	Button * m_ButtonChoice;//选择
	Text   * m_TextTop;
	Text   * m_TextMiddle;
	Text   * m_TextDown;
	int      m_MinNum;//最小的
	int      m_MaxNum;//最大的
	bool     m_IsNumChangeOver;
	bool     m_IsTouched;//是否被点击到
	map<int , Vec2> m_TextVec2;
	map<int, int>  m_Num;
	float    m_MoveTime;
	
	SEL_CallFunc_IntAndNode m_OutFunction;
	Ref * m_OutFunctioner;
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
		@触摸的方向
	*/
	void TouchSquare(Square_DirType s_type);
	/*
		@选择事件
	*/
	void TouchedEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@上下的移动
	*/
	void RunUp();
	void RunDown();
	/*
		@处理数字
	*/
	string CalculateNumIsAdd(string  str_num,bool _is);
	/*
		@获取当前的数字
	*/
	int GetCurViewNum();
	/*
		@注册数字改变回调
	*/
	void RegisterNumChangeCallback(SEL_CallFunc_IntAndNode callback, Ref * register_user);
	/*
		@调用外部函数
	*/
	void CallOutFunction();
	/*
		@改变外观
	*/
	void ChanageBackground(const string & src_back, const int & text_size = -1);
	/*
		@开启滑动
		@关闭滑动
	*/
	void OpenSlider();
	void CloseSlider();
};
#endif // !__Ui_BagItem_H__