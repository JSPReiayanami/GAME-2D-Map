#ifndef __GameButton_H__
#define __GameButton_H__
#include "Config/Head.h"
typedef function<void(Layout *)> GameButtonTouchFunc;
class GameButton : public Layout
{
public:
	GameButton();
	~GameButton();
	CREATE_FUNC(GameButton);
	bool init();
	void SetViewImageView(const string & viewRes);
	void SetTouchFunc(GameButtonTouchFunc * callfunc);
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
	void BtnTouchSelf(Ref * pSender, ui::Widget::TouchEventType eventType);
public:
	Button * m_Button;
	ImageView * m_ImageView;
	GameButtonTouchFunc * m_CallFunc;
};


#endif //!__GameButton_H__