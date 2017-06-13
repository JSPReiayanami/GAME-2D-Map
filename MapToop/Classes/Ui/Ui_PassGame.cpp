#include "Ui_PassGame.h"
#include "Manager/GameManager.h"
#define  _Ui_PassGame_Src "UI/Ui_PassGane.csb"
/*
@构造,析构
*/
Ui_PassGame::Ui_PassGame()
{
	m_LayerSrc = _Ui_PassGame_Src;

}
Ui_PassGame::~Ui_PassGame()
{

}

/*
@固定创建方法
*/
bool Ui_PassGame::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}

	return true;
}
/*
@设置自己的名字
*/
void Ui_PassGame::SetInterfaceName()
{
	this->setName("Ui_PassGame");
	CCLOG("Ui_PassGame");
}
/*
@初始化资源
*/
void Ui_PassGame::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}
/*
@初始化控件
*/
void Ui_PassGame::InitWidget()
{
	
}
/*
@初始化按钮的回调事件
*/
void Ui_PassGame::InitButtonEvent()
{
	__AddTouchEventWithNameAndPNode("Button_Back", m_Root, Ui_PassGame::TouchCloseEvent)
}


/*
@按钮的回调事件
*/
void Ui_PassGame::TouchCloseEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (ui::Widget::TouchEventType::ENDED == eventType)
	{
		
		this->removeFromParentAndCleanup(true);
		GameManager::getInstance()->GameExit();
	}
}


