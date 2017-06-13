#include "Interface_GameChoice.h"
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataManager.h"
#include "Manager/SoundManager.h"
#define  _Interface_GameChoice_Src "Interface/Interface_LevelChoice.csb"
#define  _Ui_LevelChoice "UI/Ui_LevelChoice.csb"
/*
@构造,析构
*/
Interface_GameChoice::Interface_GameChoice()
{
	m_LayerSrc = _Interface_GameChoice_Src;
}
Interface_GameChoice::~Interface_GameChoice()
{

}

Scene * Interface_GameChoice::createScene()
{
	Scene * scene = Scene::create();
	auto mainScene = Interface_GameChoice::create();
	scene->addChild(mainScene);
	return scene;
}
/*
@固定创建方法
*/
bool Interface_GameChoice::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}
	InitLevelInList();
	return true;
}
/*
@设置自己的名字
*/
void Interface_GameChoice::SetInterfaceName()
{
	this->setName("Interface_GameChoice");
	CCLOG("Interface_GameChoice::SetInterfaceName()");
}
/*
@初始化资源
*/
void Interface_GameChoice::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}

/*
@初始化控件
*/
void Interface_GameChoice::InitWidget()
{
	__GetChildAndAssignmentWithOB(m_ListChoice, m_Root, "ListView_Choice", ListView)
}
/*
@初始化按钮的回调事件
*/
void Interface_GameChoice::InitButtonEvent()
{
	__AddTouchEventWithNameAndPNode("Button_Back", m_Root, Interface_GameChoice::BackEvent);
}
/*
@按钮的回调事件
*/
void Interface_GameChoice::ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		auto btn = (Button *)pSender;
		GameManager::getInstance()->SetCurLevelNum(btn->getTag());
		GameManager::getInstance()->SetGameState(GameManager::GameStateType_Start);
	}
}
void Interface_GameChoice::BackEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType_Menu);
	}
}
/*
@初始化关卡显示
*/
void Interface_GameChoice::InitLevelInList()
{
	auto level_data = DataManager::getInstance()->GetLevelOpenData();
	m_ListChoice->removeAllItems();
	map<int, bool> temp_data;
	temp_data.clear();
	for (auto & ite : level_data)
	{
		temp_data.insert(make_pair(ite.first, ite.second));
		if (temp_data.size() == 6)
		{
			//--
			AddChoiceLevel(temp_data);
			temp_data.clear();
		}
		
	}
	if (temp_data.size() > 0 )
	{
		AddChoiceLevel(temp_data);
		temp_data.clear();
	}

}
void Interface_GameChoice::AddChoiceLevel(map<int, bool> level_data)
{
	auto choice_layer = (Layout *)CSLoader::createNode(_Ui_LevelChoice);
	int count = 0;
	for (auto & level_ : level_data)
	{
		count++;
		auto level_info = DataManager::getInstance()->GetLevelData(level_.first);
		string pan_name = StringUtils::format("Panel_Level_%d", count);
		auto pan = (Layout *)Helper::seekWidgetByName(choice_layer, pan_name);
		auto button_choice = (Button *)(Helper::seekWidgetByName(pan, "Button_Choice"));
		button_choice->setTag(level_.first);
		button_choice->loadTextureNormal(level_info->getMySmallMapSrc());
		auto img = (ImageView *)Helper::seekWidgetByName(pan, "Image_Lock");
		auto text_name = (Text *)Helper::seekWidgetByName(pan, "Text_Name");
		if (level_.second == true)
		{
			button_choice->addTouchEventListener(CC_CALLBACK_2(Interface_GameChoice::ButtonAboutBaseEvent, this));
			img->setVisible(false);
		}
		else{
			button_choice->setTouchEnabled(false);
		}
		text_name->setString(level_info->getMyName());
	}
	auto new_layout = Layout::create();
	new_layout->setContentSize(choice_layer->getContentSize());
	new_layout->addChild(choice_layer);
	m_ListChoice->pushBackCustomItem(new_layout);
	for (int i = count+1; i < 6;i++)
	{
		string pan_name = StringUtils::format("Panel_Level_%d", i);
		auto pan = (Layout *)Helper::seekWidgetByName(choice_layer, pan_name);
		pan->setVisible(false);
	}
}