#include "Interface_Game.h"
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataManager.h"
#include "Config/UiConfig.h"
#include "Game/Game_Item.h"
#include "Ui/Ui_Bag.h"
#include "Tool/StringHelper.h"
#define  _Game_Src "Interface/Interface_Game.csb"
#define  _BigButton_Src ""
/*
@构造,析构
*/
Interface_Game::Interface_Game()
{
	m_LayerSrc = _Game_Src;
}
Interface_Game::~Interface_Game()
{

}

Scene * Interface_Game::createScene()
{
	Scene * scene = Scene::create();
	auto mainScene = Interface_Game::create();
	scene->addChild(mainScene);
	
	return scene;
}
/*
@固定创建方法
*/
bool Interface_Game::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}
	InitBag();
	
	return true;
}
/*
@设置自己的名字
*/
void Interface_Game::SetInterfaceName()
{
	this->setName("Interface_Game");
	CCLOG("Interface_Game::SetInterfaceName()");
}
/*
	@初始化资源
*/
void Interface_Game::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}
/*
	@初始化控件
*/
void Interface_Game::InitWidget()
{
	__GetChildAndAssignment(m_LayoutProp,m_Root,"Panel_Prop");
	__GetChildAndAssignment(m_LayoutPropBig, m_Root, "Panel_BigProp");
	__GetChildAndAssignment(m_LayoutBag, m_Root, "Panel_Bag");
	__GetChildAndAssignment(m_LayoutBaffle, m_Root, "Panel_Baffle");
	__GetChildAndAssignment(m_LayoutButton, m_Root, "Panel_Button");
	__GetChildAndAssignmentWithOB(m_Background,m_Root,"Image_Background",ImageView)
	__GetChildAndAssignment(m_LayoutSpecialProp, m_Root, "Panel_SpecialProp")
	__GetChildAndAssignmentWithOB(m_TextLevel, m_Root, "Text_Level", Text)
}
/*
	@初始化按钮的回调事件
*/
void Interface_Game::InitButtonEvent()
{
	__AddTouchEventWithNameAndPNode("Button_Back", m_LayoutButton, Interface_Game::ButtonAboutBaseEvent)
	__AddTouchEventWithNameAndPNode("Button_NextLevel", m_LayoutButton, Interface_Game::ButtonAboutBaseEvent)
	__AddTouchEventWithNameAndPNode("Button_UpLevel", m_LayoutButton, Interface_Game::ButtonAboutBaseEvent)
	__AddTouchEventWithNameAndPNode("Button_Notice", m_LayoutButton, Interface_Game::LevelNoticeEvent)
	
}
/*
	@按钮的回调事件
*/
void Interface_Game::ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::BEGAN)
	{
		auto btn = (Button *)pSender;
		//返回按钮
		if (btn->getName() == "Button_Back")
		{
			GameManager::getInstance()->SetGameState(GameManager::GameStateType::GameStateType_Exit);
		}
		else if (btn->getName() == "Button_NextLevel")
		{
			GameManager::getInstance()->GoToNextLevel();
		}
		else if (btn->getName() == "Button_UpLevel")
		{
			GameManager::getInstance()->GoToUpLevel();
		}
	}
}
/*
	@初始化背包
*/
void Interface_Game::InitBag()
{
	m_Bag = Ui_Bag::create();
	m_LayoutBag->addChild(m_Bag);
}
/*
@初始化关卡布局根据关卡
*/
void Interface_Game::InitGameWithLevel()
{
	m_LayoutProp->removeAllChildrenWithCleanup(true);
	m_LayoutSpecialProp->removeAllChildrenWithCleanup(true);
	CloseBigMap();
	auto level_num = GameManager::getInstance()->m_GameLevelNum;
	auto level_prop = DataManager::getInstance()->GetLevelPropData(level_num);
	//--加载关卡背景
	auto level_data = DataManager::getInstance()->GetLevelData(level_num);
	if (level_data)
	{
		m_Background->loadTexture(level_data->getMySrc());
		RefreshGameTitle(level_data->getMyName());
	}
	//--判断是不是特殊关卡(时间的)
	bool is_special = false;
	auto level_value = StringHelper::splitString(level_data->getMyValue(), ";");
	map<int, bool> can_bron;
	can_bron.clear();
	if (level_value.size() >= 2)
	{
		if (level_value.at(0) == "1")
		{
			is_special = true;//开启特殊关卡的生成模式
			//--
			auto id_bron = StringHelper::splitString(level_value.at(1), ",");
			for (auto & str_id : id_bron)
			{
				can_bron.insert(make_pair(atoi(str_id.c_str()), true));
			}
		}
	}
	//--开始布局道具信息
	for (auto & prop : level_prop)
	{
		if (false == is_special && prop->getMyIsCanGetKey() == 0)
		{
			auto item = Game_Item::CreateWithPropData(prop->CloneThis());
			AddGameItemToMap(item);
		}
		else{
			if (can_bron.find(prop->getMyId()) != can_bron.end())
			{
				//---
				auto item = Game_Item::CreateWithPropData(prop->CloneThis());
				AddGameItemToMap(item);
			}
		}
		
	}

}
/*
@添加一个道具到地图上
*/
void Interface_Game::AddGameItemToMap(Game_Item * g_item)
{
	m_LayoutProp->addChild(g_item);
}

/*
@添加一个道具到放大的地图层
@物品
@地图资源路径
*/
void Interface_Game::AddGameItemToBigMap(Game_Item * g_item)
{
	//--添加子节点
	m_LayoutPropBig->addChild(g_item);
}
/*
@开启放大场景
*/
void Interface_Game::OpenBigMap(const string & map_src)
{
	m_LayoutPropBig->removeAllChildrenWithCleanup(true);
	//--背景增加
	auto back_sp = Sprite::create(map_src);
	back_sp->setPosition(__VisibleSize / 2);
	m_LayoutPropBig->addChild(back_sp);
	//--添加一个按钮
	auto big_btn = Button::create(map_src);
	big_btn->setOpacity(0);
	big_btn->addTouchEventListener(CC_CALLBACK_2(Interface_Game::BigButtonEvent, this));
	big_btn->setPosition(__VisibleSize / 2);
	m_LayoutPropBig->addChild(big_btn);
	//--遮住下面
	m_LayoutPropBig->setTouchEnabled(true);
}
/*
@关闭放大的场景
*/
void Interface_Game::CloseBigMap()
{
	m_LayoutPropBig->removeAllChildrenWithCleanup(true);
	m_LayoutPropBig->setTouchEnabled(false);
}
/*
@大场景的关闭按钮事件
*/
void Interface_Game::BigButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		GameManager::getInstance()->CloseBigMap();
	}
}
/*
@添加一个东西进特殊道具层
*/
void Interface_Game::AddPropToLayoutSpecialProp(Node * prop)
{
	m_LayoutSpecialProp->addChild(prop);
}
/*
@刷新关卡标题
*/
void Interface_Game::RefreshGameTitle(const string & title)
{
	m_TextLevel->setString(title);
}
/*
@过关提示
*/
void Interface_Game::LevelNoticeEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		//--弹出提示
		GameManager::getInstance()->PopPassLevelNotice();
	}
}