#include "Game_Item.h"
#include "Manager/DataManager.h"
#include "Manager/GameManager.h"
#include "Manager/SoundManager.h"
#include "Tool/StringHelper.h"
#include "Game_KeyLock.h"
Game_Item::Game_Item() :
m_AnimationSrc(""),
m_Id(0),
m_Data(nullptr),
m_BigItem(nullptr)
{
	
}
Game_Item::~Game_Item()
{
	CC_SAFE_DELETE(m_Data);
}
Game_Item * Game_Item::CreateWithId(const int &_id)
{

	auto *game_item = new(std::nothrow) Game_Item(); 
	if (game_item) 
	{ 
		game_item->SetId(_id);
		
	}
	if (game_item->init())
	{
		game_item->autorelease();
		game_item->RunActionWithType();
		return game_item;
	}
	else 
	{ 
		delete game_item;
		game_item = nullptr;
		return nullptr;
	}
}

Game_Item * Game_Item::CreateWithPropData(const PropData * p_data)
{
	auto *game_item = new(std::nothrow) Game_Item();
	if (game_item)
	{
		game_item->SetId(p_data->getMyId());
		
	}
	if (game_item->init())
	{
		game_item->setPosition(p_data->getMyVec2());
		game_item->setMyIsCanGetKey(p_data->getMyIsCanGetKey());
		if (p_data->getMyValue() != "0")
		{
			game_item->setMyValue(p_data->getMyValue());
		}
		else{
			game_item->setMyValue(game_item->m_Data->getMyValue());
		}
		game_item->autorelease();
		game_item->RunActionWithType();
		CC_SAFE_DELETE(p_data);
		return game_item;
	}
	else
	{
		CC_SAFE_DELETE(p_data);
		delete game_item;
		game_item = nullptr;
		return nullptr;
	}
}
/*
@固定创建方法
*/
bool Game_Item::init(){
	if (!Layout::init())
	{
		return false;
	}
	m_Data = DataManager::getInstance()->GetGameItemDataWithId(m_Id);
	if (m_Data)
	{
		m_Sprite = Button::create(m_Data->getMySrc().c_str(), m_Data->getMySrc().c_str(), m_Data->getMySrc().c_str());
		m_Sprite->setAnchorPoint(Vec2(0,0));
		m_Sprite->addTouchEventListener(CC_CALLBACK_2(Game_Item::TouchEvent,this));
		this->setContentSize(m_Sprite->getContentSize());
		this->addChild(m_Sprite);
		this->setAnchorPoint(Vec2(0.5, 0.5));
		setMyValue(m_Data->getMyValue());
		if (m_Data->getMyItemType() == GameItemType_View || m_Data->getMyItemType() == GameItemType_CountdownItem)
		{
			this->setTouchEnabled(false);
			m_Sprite->setTouchEnabled(false);
		}
		InitEffects();
	}
	return true;
}

/*
@设置自己id
*/
void Game_Item::SetId(const int & _id)
{
	m_Id = _id;
}
/*
@被点击到事件
*/
void Game_Item::TouchEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("[%s] is touch", m_Data->getMyName().c_str());
		DoActionWithType();
	}
}
/*
@道具改变
*/
void Game_Item::ChageThisWithPropData(PropData * p_data)
{
	m_Data = DataManager::getInstance()->GetGameItemDataWithId(p_data->getMyId());
	m_Sprite->loadTextures(m_Data->getMySrc(), m_Data->getMySrc(), m_Data->getMySrc());
	this->SetId(p_data->getMyId());
	this->setPosition(p_data->getMyVec2());
	this->setMyIsCanGetKey(p_data->getMyIsCanGetKey());
	if (p_data->getMyValue() != "0")
	{
		this->setMyValue(p_data->getMyValue());
	}
	else{
		this->setMyValue(m_Data->getMyValue());
	}
	
}
/*
@根据类型点击到了的类型分下下去各自的事件
GameItemType_Door = 1,	//门
GameItemType_Key,	// 钥匙
GameItemType_TouchChange,//点击后改变道具类
GameItemType_TouchGet ,//点击获取类道具并且自己不消失的,并且获取到的道具消失
GameItemType_TouchMoveLeft,//点击左移类道具
GameItemType_TouchMoveRight,//点击右移类道具
GameItemType_TouchGetAndNil,	//上点击获取后，然后消失
GameItemType_View,	//单纯展示用的
GameItemType_MoveWithPointsAndTouchGetAndNil,	//根据一些点来移动，然后点击后会消失
GameItemType_TouchAddToMapAndNil,	//点击后增加一个道具到地图上
GameItemType_TouchBigAddToMap,	//点击后增加一个道具到放大后的地图上
GameItemType_BigTouchGetAndNil,	//大地图上点击获取后，然后消失
GameItemType_BigTouchChange,	//大地图上点击后，后变为其他东西
GameItemType_BigTouchGetAndChange,	//大地图上点击后获取到道具后变为其他东西
GameItemType_TouchPopLock,//点击弹出一个密码锁
GameItemType_TouchGetAndChange,//小地图获取然后变为其他东西
GameItemType_TouchCombinationAndChangeAdd,//大地图上，点击后改变外面放大物品，然后判断是否能获取到东西,最后添加东西到小地图上
GameItemType_Max,//最大的枚举值，不用
*/
void Game_Item::DoActionWithType()
{
	switch (m_Data->getMyItemType())
	{
	case  GameItemType::GameItemType_Door://门
		DoorAction();
		break;
	case  GameItemType::GameItemType_TouchChange:// 钥匙
		TouchChangeAction();
		break;
	case  GameItemType::GameItemType_TouchGet://点击获取类道具并且自己不消失的,并且获取到的道具消失
		TouchGetAction();
		break;
	case  GameItemType::GameItemType_TouchGetAndNil ://点击获取后，然后消失
		TouchGetAndNil();
		break;
	case  GameItemType::GameItemType_MoveWithPointsAndTouchGetAndNil:
		TouchGetAndNil();
		break;
	case  GameItemType::GameItemType_TouchAddToMapAndNil://点击后增加一个道具到地图上
		TouchAddToMapAndNil();
		break;
	case  GameItemType::GameItemType_TouchBigAddToMap:
		TouchBigAddToMap();
		break;
	case  GameItemType::GameItemType_BigTouchGetAndNil:
		BigTouchGetAndNil();
		break;
	case  GameItemType::GameItemType_BigTouchGetAndChange:
		BigTouchGetAndChange();
		break;
	case  GameItemType::GameItemType_BigTouchChange:
		BigTouchChange();
		break;
	case  GameItemType::GameItemType_TouchPopLock:
		TouchPopLock();
		break;
	case  GameItemType::GameItemType_TouchChangeAndSetPosVlaue:
		TouchChangeAndSetPosVlaue();
		break;
	case  GameItemType::GameItemType_TouchAndGetPropWithFriendId:
		TouchAndGetPropWithFriendId();
		break;
	case  GameItemType::GameItemType_BigTouchChangeBigItemOrGet:
		BigTouchChangeBigItemOrGet();
		break;
	case  GameItemType::GameItemType_TouchCombinationAndChangeAdd:
		TouchCombinationAndChangeAdd();
		break;
	case  GameItemType::GameItemType_BigTouchChangeOutAndChangeSelf:
		BigTouchChangeOutAndChangeSelf();
		break;
	case  GameItemType::GameItemType_TimeItem:
		TimeItem();
		break;
	default:
		break;
	}
}

//--门
void Game_Item::DoorAction()
{
	int s_value = atoi(m_Value.c_str());
	if (s_value == 1)
	{
		PlaySelfEffect();
		GameManager::getInstance()->SetGameState(GameManager::GameStateType_Pass);
		
	}
	else if (s_value == 0)
	{
		int cur_pid = GameManager::getInstance()->GetCurChoicePropId();
		if (cur_pid == m_Data->getMyFriendId())
		{
			PlaySelfEffect();
			GameManager::getInstance()->SetGameState(GameManager::GameStateType_Pass);
		}
		else{
			//--没有正确id没有打开门
		}
	}
	else{
		//--可拓展
	}
}
void Game_Item::KeyAction()
{

}
void Game_Item::TouchChangeAction()
{
	if (IsCanActionForFriendId())
	{
		PlaySelfEffect();
		int change_id = atoi(m_Value.c_str());
		auto item_data = DataManager::getInstance()->GetGameItemDataWithId(change_id);
		if (item_data)
		{
			auto p_data = new PropData();
			p_data->setMyId(item_data->getMyId());
			p_data->setMyIsCanGetKey(item_data->getMyGetId());
			p_data->setMyValue(item_data->getMyValue());
			p_data->setMyVec2(this->getPosition());
			this->ChageThisWithPropData(p_data);
			CC_SAFE_DELETE(p_data);
		}
		else{
			CCLOG("Can't find this item that id is %d", change_id);
		}
	}
}
void Game_Item::TouchGetAction()
{
	if (IsCanActionForFriendId())
	{
		//---通知背包获得东西
		if (m_Data->getMyGetId() != 0)
		{
			PlaySelfEffect();
			m_Sprite->loadTextures(m_Data->getMyValue().c_str(), m_Data->getMyValue().c_str(), m_Data->getMyValue().c_str());
			GameManager::getInstance()->AddBagItem(m_Data->getMyGetId());
			m_Data->setMyGetId(0);
		}
		else{
			//--没有东西可以拿了
		}
	}
}
void Game_Item::TouchMoveLeftAction()
{

}
void Game_Item::TouchMoveRightAction()
{

}
void Game_Item::TouchGetAndNil()
{
	if (IsCanActionForFriendId())
	{
		
		//---通知背包获得东西
		if (m_Data->getMyGetId() != 0)
		{
			PlaySelfEffect();
			//m_Sprite->loadTextures(m_Value.c_str(), m_Value.c_str(), m_Value.c_str());
			GameManager::getInstance()->AddBagItem(m_Data->getMyGetId());
			m_Data->setMyGetId(0);
			this->setVisible(false);
		}
	}
}
/*
	@获取value里面的值根据","拆分
	@拆分出来必须是4个长度的数组
	@第一个是道具Id
	@第二，三个是坐标
	@第四个是值
*/
void Game_Item::TouchAddToMapAndNil()
{
	if (IsCanActionForFriendId())
	{
		PlaySelfEffect();
		//---通知背包获得东西
		if (m_Data->getMyGetId() != 0)
		{
			
			//m_Sprite->loadTextures(m_Value.c_str(), m_Value.c_str(), m_Value.c_str());
			GameManager::getInstance()->AddBagItem(m_Data->getMyGetId());
			m_Data->setMyGetId(0);
			this->setVisible(false);
		}
		//---通知游戏管理地图添加新的道具
		auto all_item_data = StringHelper::splitString(m_Value, ";");
		for (int i = 0; i < all_item_data.size(); i++)
		{
			auto new_item_data = StringHelper::splitString(all_item_data.at(i), ",");
			if (new_item_data.size() == 4)
			{
				auto item = GameManager::getInstance()->AddGameItemToMapWithId(atoi(new_item_data.at(0).c_str()));
				item->setPosition(Vec2(atoi(new_item_data.at(1).c_str()), atoi(new_item_data.at(2).c_str())));
				if (new_item_data.at(3) != "0")
				{
					item->setMyValue(new_item_data.at(3));
				}
				this->setVisible(false);
			}
		}
	}
}
/*
@获取value里面的值根据","拆分
@拆分出来必须是5个长度的数组
@第一个是放大的图片资源路径
@第二个是道具Id
@第三，四个是坐标
@第五个是值
*/
void Game_Item::TouchBigAddToMap()
{
	
	//---解析字段
	auto all_data = StringHelper::splitString(m_Value, ";");
	if (all_data.size() >= 1)
	{
		//---打开大地图
		GameManager::getInstance()->OpenBigMap(all_data.at(0),this);
		for (int i = 1; i < all_data.size(); i ++)
		{
			auto new_item_data = StringHelper::splitString(all_data.at(i), ",");
			//---满足5个数据
			if (new_item_data.size() == 5 && atoi(new_item_data.at(0).c_str()) == 1)
			{
				AddGameItemToBigMaoWithValueAndStartIndex(all_data.at(i), 1);
			}
			
		}
		
		
	}
	
}

void Game_Item::BigTouchGetAndNil()
{
	if (IsCanActionForFriendId())
	{
		//---通知背包获得东西
		if (m_Data->getMyGetId() != 0)
		{
			PlaySelfEffect();
			//m_Sprite->loadTextures(m_Value.c_str(), m_Value.c_str(), m_Value.c_str());
			GameManager::getInstance()->AddBagItem(m_Data->getMyGetId());
			GameManager::getInstance()->AddBagItemInBigMap(m_Data->getMyId(),this->getPosition());
			m_Data->setMyGetId(0);
			this->setVisible(false);
		}
	}
}
void Game_Item::BigTouchChange()
{
	if (IsCanActionForFriendId())
	{
		PlaySelfEffect();
		//---通知大地图东西改变-改变数据
		GameManager::getInstance()->BigMapGameItemChange(m_Data->getMyId(), this->getPosition(),m_Value.c_str());
		this->setVisible(false);
		//---改变的东西添加上去-改变显示
		AddGameItemToBigMaoWithValueAndStartIndex(m_Value, 0);
	}
}
void Game_Item::BigTouchGetAndChange()
{
	if (IsCanActionForFriendId())
	{
		//---通知大地图东西改变
		
		//---通知背包获得东西
		if (m_Data->getMyGetId() != 0)
		{
			PlaySelfEffect();
			//m_Sprite->loadTextures(m_Value.c_str(), m_Value.c_str(), m_Value.c_str());
			GameManager::getInstance()->AddBagItem(m_Data->getMyGetId());
			m_Data->setMyGetId(0);
			GameManager::getInstance()->BigMapGameItemChange(m_Data->getMyId(), this->getPosition(), m_Value.c_str());
			this->setVisible(false);
			//---改变的东西添加上去-改变显示
			AddGameItemToBigMaoWithValueAndStartIndex(m_Value, 0);
		}
	}
}

void Game_Item::TouchPopLock()
{
	if (IsCanActionForFriendId())
	{
		//--生成密码锁
		//--添加进小地图
		//PlaySelfEffect();
		auto value_data = StringHelper::splitString(m_Data->getMyValue(), ";");
		if (2 <= value_data.size())
		{
			auto key_lock = Game_KeyLock::CreateWithId(atoi(value_data.at(0).c_str()));
			key_lock->RegisterUnlockCallback(CallFunc_Bool_selector(Game_Item::MyLockIsOpen),this);
			GameManager::getInstance()->AddPropToLayoutSpecialProp(key_lock);
		}
		
	}
}

void Game_Item::TouchChangeAndSetPosVlaue()
{
	if (IsCanActionForFriendId())
	{
		PlaySelfEffect();
		auto new_item_data = StringHelper::splitString(m_Value, ",");
		if (new_item_data.size() == 4)
		{
			int change_id = atoi(new_item_data.at(0).c_str());
			auto item_data = DataManager::getInstance()->GetGameItemDataWithId(change_id);
			if (item_data)
			{
				auto p_data = new PropData();
				p_data->setMyId(atoi(new_item_data.at(0).c_str()));
				p_data->setMyIsCanGetKey(item_data->getMyGetId());
				p_data->setMyVec2(Vec2(atoi(new_item_data.at(1).c_str()), atoi(new_item_data.at(2).c_str())));
				if (new_item_data.at(3) != "0")
				{
					p_data->setMyValue(new_item_data.at(3));
				}
				else{
					p_data->setMyValue("0");
				}
				this->ChageThisWithPropData(p_data);
				CC_SAFE_DELETE(p_data);
			}
			else{
				CCLOG("Can't find this item that id is %d", change_id);
			}
		}
	}
}

void Game_Item::TouchAndGetPropWithFriendId()
{
	//--反正会消失
	if (1 == m_Data->getMyIsBigMap())
	{
		//
		GameManager::getInstance()->AddBagItemInBigMap(m_Data->getMyId(), this->getPosition());
	}
	else{
		this->setVisible(false);
	}
	if (GameManager::getInstance()->IsHavePropWithId(m_Data->getMyFriendId()))
	{
		GameManager::getInstance()->DeleteBagItem(m_Data->getMyFriendId());
		GameManager::getInstance()->AddBagItem(m_Data->getMyGetId());
	}
	else{
		GameManager::getInstance()->AddBagItem(atoi(m_Data->getMyValue().c_str()));
	}
}

void Game_Item::TouchGetAndChange()
{
	if (IsCanActionForFriendId())
	{

	}
	else{

	}
}

void Game_Item::BigTouchChangeBigItemOrGet()
{
	if (IsCanActionForFriendId())
	{
		if (m_Data->getMyId() != 0)
		{
			PlaySelfEffect();
			GameManager::getInstance()->AddBagItem(m_Data->getMyGetId());
			m_Data->setMyGetId(0);
			this->setVisible(false);
			//--通知大地图东西被拿了
			GameManager::getInstance()->AddBagItemInBigMap(m_Id, this->getPosition());
			//--改变变大母体
			auto c_data = StringHelper::splitString(m_Data->getMyValue(), ",");
			if (c_data.size() == 4)
			{
				Vec2 pos;
				pos.x = atof(c_data.at(1).c_str());
				pos.y = atof(c_data.at(2).c_str());
				GameManager::getInstance()->ChangeBigMapItemShape(atoi(c_data.at(0).c_str()), pos);
			}
		}
		
		
	}
}

void Game_Item::TouchCombinationAndChangeAdd()
{
	if (IsCanActionForFriendId())
	{

		//---
		auto value_data = StringHelper::splitString(m_Data->getMyValue(), ";");
		int cur_id = GameManager::getInstance()->GetCurChoicePropId();
		bool is_can = true;
		string new_data = "";
		for (int i = 0; i < value_data.size();i++)
		{
			auto temp_data = StringHelper::splitString(value_data.at(i),",");
			if (temp_data.at(0) != "a" && temp_data.at(0) != "0" && temp_data.size() >= 5)
			{
				
				if (atoi(temp_data.at(0).c_str()) == cur_id)
				{
					//--
					GameManager::getInstance()->DeleteBagItem(cur_id);
					//--生成
					auto item = Game_Item::CreateWithId(atoi(temp_data.at(1).c_str()));
					this->addChild(item);
					item->setPosition(Vec2(atoi(temp_data.at(2).c_str()), atoi(temp_data.at(3).c_str())));
					//--修改
					new_data.append(StringUtils::format("0,%s,%s,%s,%s;", temp_data.at(1).c_str(), temp_data.at(2).c_str(), temp_data.at(3).c_str(), temp_data.at(4).c_str()));
				}
				else{
					new_data.append(StringUtils::format("%s;", value_data.at(i).c_str()));
					is_can = false;
				}
			}
			else{
				new_data.append(StringUtils::format("%s;", value_data.at(i).c_str()));
			}
		}
		////--检测组装完毕没
		//for (int i = 0; i < value_data.size(); i++)
		//{
		//	auto temp_data = StringHelper::splitString(value_data.at(i), ",");
		//	if (temp_data.at(0) != "a" && temp_data.at(0) != "0" && temp_data.size() >= 5)
		//	{
		//		is_can = false;
		//	}
		//}
		
		if (is_can)
		{
			for (int i = 0; i < value_data.size(); i++)
			{
				auto temp_data = StringHelper::splitString(value_data.at(i), ",");
				if (temp_data.at(0) == "a")
				{
					//--添加到小地图上
					auto item = GameManager::getInstance()->AddGameItemToMapWithId(atoi(temp_data.at(1).c_str()));
					item->setPosition(Vec2(atof(temp_data.at(2).c_str()), atof(temp_data.at(3).c_str())));
				}
			}
			PlaySelfEffect();
		}
		setMyValue(new_data);
	}
}


void Game_Item::BigTouchChangeOutAndChangeSelf()
{
	if (IsCanActionForFriendId())
	{
		PlaySelfEffect();
		this->setVisible(false);
		//--改变变大母体
		auto c_data = StringHelper::splitString(m_Data->getMyValue(), ";");
		auto cb_data = StringHelper::splitString(c_data.at(0), ",");
		if (cb_data.size() == 4)
		{
			Vec2 pos;
			pos.x = atof(cb_data.at(1).c_str());
			pos.y = atof(cb_data.at(2).c_str());
			GameManager::getInstance()->ChangeBigMapItemShape(atoi(cb_data.at(0).c_str()), pos, cb_data.at(3));
		}
		//--添加东西到大地图上
		for (int i = 1; i < c_data.size(); i++)
		{
			//---改变的东西添加上去-改变显示
			AddGameItemToBigMaoWithValueAndStartIndex(c_data.at(i), 0);
		}
	}
}

void Game_Item::TimeItem()
{
	if (IsCanActionForFriendId())
	{
		//--开启关卡的特殊模式
		GameManager::getInstance()->OpenCurLevelSpecial();
		PlaySelfEffect();
		//---通知背包获得东西
		if (m_Data->getMyGetId() != 0)
		{
			//m_Sprite->loadTextures(m_Value.c_str(), m_Value.c_str(), m_Value.c_str());
			GameManager::getInstance()->AddBagItem(m_Data->getMyGetId());
			m_Data->setMyGetId(0);
			this->setVisible(false);
		}
		//---通知游戏管理地图添加新的道具
		auto all_item_data = StringHelper::splitString(m_Value, ";");
		for (int i = 0; i < all_item_data.size(); i++)
		{
			auto new_item_data = StringHelper::splitString(all_item_data.at(i), ",");
			DataManager::getInstance()->SetItemIdTime(atoi(new_item_data.at(0).c_str()));
			if (new_item_data.size() == 4)
			{
				auto item = GameManager::getInstance()->AddGameItemToMapWithId(atoi(new_item_data.at(0).c_str()));
				item->setPosition(Vec2(atoi(new_item_data.at(1).c_str()), atoi(new_item_data.at(2).c_str())));
				if (new_item_data.at(3) != "0")
				{
					item->setMyValue(new_item_data.at(3));
				}
				this->setVisible(false);
			}
		}
		

	}
}
//===特殊的会动的道具==
/*
	@根据类型分发各自的移动或者其他事件
	
	GameItemType_Door = 1,	//门
	GameItemType_Key,	// 钥匙
	GameItemType_TouchChange,//点击后改变道具类
	GameItemType_TouchGet ,//点击获取类道具并且自己不消失的,并且获取到的道具消失
	GameItemType_TouchMoveLeft,//点击左移类道具
	GameItemType_TouchMoveRight,//点击右移类道具
	GameItemType_TouchGetAndNil,	//上点击获取后，然后消失
	GameItemType_View,	//单纯展示用的
	GameItemType_MoveWithPointsAndTouchGetAndNil,	//根据一些点来移动，然后点击后会消失
	GameItemType_TouchAddToMapAndNil,	//点击后增加一个道具到地图上
	GameItemType_BigTouchChange,	//大地图上点击后，后变为其他东西
	GameItemType_BigTouchGetAndChange,	//大地图上点击后获取到道具后变为其他东西
	GameItemType_TouchPopLock,//点击弹出一个密码锁
	GameItemType_Max,//最大的枚举值，不用
	
*/
void Game_Item::RunActionWithType()
{
	switch (m_Data->getMyItemType())
	{
	case GameItemType::GameItemType_MoveWithPointsAndTouchGetAndNil:
		RunMoveWithPoints();
		break;
	case GameItemType::GameItemType_CountdownItem:
		RunCountdownItem();
		break;
	default:
		break;
	}
}

void Game_Item::RunMoveWithPoints()
{
	
}
/*
@倒计时事件
*/
void Game_Item::RunCountdownItem()
{
	this->scheduleUpdate();
	string time_data;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time_data = "2016,3,10,14,02,30";
#else
	time_data = DataManager::getInstance()->GetItemTimeWithId(m_Id);
#endif  
	
	
	//--解析出来再根据当前时间比对，算出剩余时间
	auto time_v = StringHelper::splitString(time_data, ",");
	int years = atoi(time_v.at(0).c_str());
	int month = atoi(time_v.at(1).c_str());
	int days = atoi(time_v.at(2).c_str());
	int hour = atoi(time_v.at(3).c_str());
	int minute = atoi(time_v.at(4).c_str());
	int second = atoi(time_v.at(5).c_str());
	auto last_sd = new StringHelper::SDate(years,month,days,hour,minute,second);
	//--
	time_t now_time;
	time(&now_time);
	struct tm * t_time = localtime(&now_time);
	int now_years = 1900 + t_time->tm_year;
	int now_month = 1 + t_time->tm_mon;
	int now_days = t_time->tm_mday;
	int now_hour = t_time->tm_hour;
	int now_minute = t_time->tm_min;
	int now_second = t_time->tm_sec;
	auto now_sd = new StringHelper::SDate(now_years, now_month, now_days, now_hour, now_minute, now_second);

	auto dele_sd = now_sd->TimeBetweenTwoS(*now_sd, *last_sd);
	CC_SAFE_DELETE(last_sd);
	CC_SAFE_DELETE(now_sd);
	//--
	auto v_data = StringHelper::splitString(m_Data->getMyValue(), ";");
	auto time_f = atof(v_data.at(0).c_str());
	string new_data = "";
	time_f -= dele_sd;
	if (time_f <= 0)
	{
		//--开始添加后面道具
		this->unscheduleUpdate();
		GameManager::getInstance()->CloseCurLevelSpecial();
		for (int i = 1; i < v_data.size(); i++)
		{
			auto item_data = StringHelper::splitString(v_data.at(i), ",");
			auto item = GameManager::getInstance()->AddGameItemToMapWithId(atoi(item_data.at(0).c_str()));
			item->setPosition(Vec2(atoi(item_data.at(1).c_str()), atoi(item_data.at(2).c_str())));
			if (item_data.at(3) != "0")
			{
				item->setMyValue(item_data.at(3));
			}
		}

	}
	else{

	}
	new_data.append(StringUtils::format("%0.4f;", time_f));
	for (int i = 1; i < v_data.size(); i++)
	{
		new_data.append(StringUtils::format("%s;", v_data.at(i).c_str()));
	}
	m_Data->setMyValue(new_data);
}

/*
@播放声效
*/
void Game_Item::PlaySelfEffect()
{
	SoundManager::getInstance()->PlayEffectMusic(m_Data->getMyMusicId());
}



/*
@帧函数事件
*/
void Game_Item::update(float delta)
{
	switch (m_Data->getMyItemType())
	{
	case GameItemType_CountdownItem:
		CountdownItem(delta);
		break;
	default:
		break;
	}
}
/*
@倒计时
*/
void Game_Item::CountdownItem(float delta)
{
	auto v_data = StringHelper::splitString(m_Data->getMyValue(), ";");
	auto time_f = atof(v_data.at(0).c_str());
	string new_data = "";
	time_f -= delta;
	if (time_f <= 0)
	{
		//--开始添加后面道具
		this->unscheduleUpdate();
		GameManager::getInstance()->CloseCurLevelSpecial();
		for (int i = 1; i < v_data.size(); i++)
		{
			auto item_data = StringHelper::splitString(v_data.at(i), ",");
			auto item = GameManager::getInstance()->AddGameItemToMapWithId(atoi(item_data.at(0).c_str()));
			item->setPosition(Vec2(atoi(item_data.at(1).c_str()), atoi(item_data.at(2).c_str())));
			if (item_data.at(3) != "0")
			{
				item->setMyValue(item_data.at(3));
			}
		}
		
	}
	else{
		
	}
	new_data.append(StringUtils::format("%0.4f;", time_f));
	for (int i = 1; i < v_data.size(); i++)
	{
		new_data.append(StringUtils::format("%s;", v_data.at(i).c_str()));
	}
	m_Data->setMyValue(new_data);
}

/*
@身上的特效
*/
void Game_Item::InitEffects()
{
	auto b_data = StringHelper::splitString(m_Data->getMyBagIco(),",");
	if (b_data.size() >=4)
	{
		if (b_data.at(0) == "Particle")
		{
			switch (atoi(b_data.at(1).c_str()))
			{
				//--火焰特效
			case 1:
				auto fire_p = ParticleFire::create();
				fire_p->setTexture(Director::getInstance()->getTextureCache()->addImage("ItemIco/4_qiqiu.png"));
				fire_p->setPosition(Vec2(atof(b_data.at(2).c_str()), atof(b_data.at(3).c_str())));
				this->addChild(fire_p);
				break;
			}
		}
	}

}