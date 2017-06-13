#include "Interface_Login.h"
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"
#include "Ui/Ui_Notice_Text.h"
#define  _Login_Src "SceneRes/Login.csb"
/*
@构造,析构
*/
Interface_Login::Interface_Login()
{
	m_LayerSrc = _Login_Src;
}
Interface_Login::~Interface_Login()
{

}

Scene * Interface_Login::createScene()
{
	Scene * scene = Scene::create();
	auto mainScene = Interface_Login::create();
	scene->addChild(mainScene);
	return scene;
}
/*
	@固定创建方法
*/
bool Interface_Login::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
	InitButtonEvent();
	/*
		@测试区
	*/
	auto n_t = Ui_Notice_Text::create();
	n_t->setPosition(Vec2(0,__VisibleSize_Height/2));
	this->addChild(n_t);
	return true;
}
/*
	@设置自己的名字
*/
void Interface_Login::SetInterfaceName()
{
	this->setName("Interface_Login");
	CCLOG("Interface_Login::SetInterfaceName()");
}
/*
@初始化按钮的回调事件
*/
void Interface_Login::InitButtonEvent()
{
	__AddTouchEventWithNameAndPNode("Button_Start",m_Root,Interface_Login::ButtonAboutBaseEvent)
}
/*
@按钮的回调事件
*/
void Interface_Login::ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::BEGAN)
	{
		auto btn = (Button *)pSender;
		if (btn->getName() == "Button_Start")
		{
			GameManager::getInstance()->SetGameState(GameManager::GameStateType_Start);
		}
	}
}