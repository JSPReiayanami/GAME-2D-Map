#include "Ui_PassGame.h"
#include "Manager/GameManager.h"
#define  _Ui_PassGame_Src "UI/Ui_PassGane.csb"
/*
@����,����
*/
Ui_PassGame::Ui_PassGame()
{
	m_LayerSrc = _Ui_PassGame_Src;

}
Ui_PassGame::~Ui_PassGame()
{

}

/*
@�̶���������
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
@�����Լ�������
*/
void Ui_PassGame::SetInterfaceName()
{
	this->setName("Ui_PassGame");
	CCLOG("Ui_PassGame");
}
/*
@��ʼ����Դ
*/
void Ui_PassGame::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}
/*
@��ʼ���ؼ�
*/
void Ui_PassGame::InitWidget()
{
	
}
/*
@��ʼ����ť�Ļص��¼�
*/
void Ui_PassGame::InitButtonEvent()
{
	__AddTouchEventWithNameAndPNode("Button_Back", m_Root, Ui_PassGame::TouchCloseEvent)
}


/*
@��ť�Ļص��¼�
*/
void Ui_PassGame::TouchCloseEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (ui::Widget::TouchEventType::ENDED == eventType)
	{
		
		this->removeFromParentAndCleanup(true);
		GameManager::getInstance()->GameExit();
	}
}


