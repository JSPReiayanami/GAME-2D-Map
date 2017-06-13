#include "Interface_Main.h"


//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataManager.h"
#include "Manager/SoundManager.h"
#else
#include "SceneManager.h"
#include "GameManager.h"
#include "DataManager.h"
#include "SoundManager.h"
#endif//判断路径的引用

#define  _Main_Src "Interface/Interface_Main.csb"
/*
@构造,析构
*/
Interface_Main::Interface_Main()
{
	m_LayerSrc = _Main_Src;
}
Interface_Main::~Interface_Main()
{

}

Scene * Interface_Main::createScene()
{
	Scene * scene = Scene::create();
	auto mainScene = Interface_Main::create();
    mainScene->setPositionY(__ResolutionPolicy_FIXED_WIDTH);
	scene->addChild(mainScene);
	return scene;
}
/*
@固定创建方法
*/
bool Interface_Main::init()
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
void Interface_Main::SetInterfaceName()
{
	this->setName("Interface_Main");
	CCLOG("Interface_Main::SetInterfaceName()");
}
/*
	@初始化资源
*/
void Interface_Main::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}
/*
@初始化按钮的回调事件
*/
void Interface_Main::InitButtonEvent()
{
	__AddTouchEventWithNameAndPNode("Button_GameStart", m_Root, Interface_Main::ButtonAboutBaseEvent)
	__AddTouchEventWithNameAndPNode("Button_Choice", m_Root, Interface_Main::ButtonAboutBaseEvent)
	__AddTouchEventWithNameAndPNode("Button_Music", m_Root, Interface_Main::MusicButtonEvent)
	__AddTouchEventWithNameAndPNode("Button_Buy", m_Root, Interface_Main::BuyButtonEvent)
	bool is_open = SoundManager::getInstance()->m_SoundIsOpen;
	auto music_btn = Helper::seekWidgetByName(m_Root, "Button_Music");
	music_btn->setBright(is_open);
}
/*
@按钮的回调事件
*/
void Interface_Main::ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		auto btn = (Button *)pSender;
		if (btn->getName() == "Button_GameStart")
		{
			GameManager::getInstance()->SetGameState(GameManager::GameStateType_Scene_1);

		}
		else if (btn->getName() == "Button_Choice")
		{
			SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType_ChoiceGame);
		}

	}
}
/*
@音效按键
*/
void Interface_Main::MusicButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		auto btn = (Button *)pSender;
		
		bool is_open = SoundManager::getInstance()->m_SoundIsOpen;
		if (is_open)
		{
			btn->setBright(false);
			SoundManager::getInstance()->CloseSound();
		}
		else{
			btn->setBright(true);
			SoundManager::getInstance()->OpenSound();
		}
	}
}


/*
@购买
*/
void Interface_Main::BuyButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{

	}
}