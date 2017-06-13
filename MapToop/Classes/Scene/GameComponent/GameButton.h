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
	@��ʼ����Դ
	*/
	void InitSrcLayout();
	/*
	@��ʼ���ؼ�
	*/
	void InitWidget();
	/*
	@��ʼ����ť�ص�
	*/
	void InitButtonEvent();
	void BtnTouchSelf(Ref * pSender, ui::Widget::TouchEventType eventType);
public:
	Button * m_Button;
	ImageView * m_ImageView;
	GameButtonTouchFunc * m_CallFunc;
};


#endif //!__GameButton_H__