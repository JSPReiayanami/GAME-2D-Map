#include "GameButton.h"
GameButton::GameButton()
{
	m_Button = nullptr;
	m_ImageView = nullptr;
	m_CallFunc = nullptr;
}
GameButton::~GameButton()
{

}

bool GameButton::init()
{
	if (!Layout::init())
	{
		return false;
	}

	return true;
}

void GameButton::SetViewImageView(const string & viewRes)
{
	if (m_ImageView == nullptr)
	{
		m_ImageView = ImageView::create(viewRes);
		m_ImageView->setTouchEnabled(true);
		this->setContentSize(m_ImageView->getContentSize());
		this->addChild(m_ImageView, 5);
	}
	else
	{
		m_ImageView->loadTexture(viewRes);
	}

	if (m_Button == nullptr)
	{
		m_Button = Button::create(viewRes);
		m_Button->addTouchEventListener(CC_CALLBACK_2(GameButton::BtnTouchSelf,this));
		this->addChild(m_ImageView, 4);
	}


}

void GameButton::SetTouchFunc(GameButtonTouchFunc * callfunc)
{
	m_CallFunc = callfunc;
}

/*
@初始化资源
*/
void GameButton::InitSrcLayout()
{

}
/*
@初始化控件
*/
void GameButton::InitWidget()
{

}
/*
@初始化按钮回调
*/
void GameButton::InitButtonEvent()
{

}

void GameButton::BtnTouchSelf(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		if (m_CallFunc)
		{
			(*m_CallFunc)(this);
		}
	}
}
