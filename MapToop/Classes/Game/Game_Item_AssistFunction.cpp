#include "Game_Item.h"
#include "Manager/DataManager.h"
#include "Manager/GameManager.h"
#include "Tool/StringHelper.h"
/*
@======================================================[一些辅助方法]=============================================
@=================================================================================================================
*/
void Game_Item::setMyValue(string s_value)
{
	m_Value = s_value;
	m_Data->setMyValue(s_value);
}
string Game_Item::getMyValue()
{
	return m_Value;
}
/*
@判断道具的前置条件
*/
bool Game_Item::IsCanActionForFriendId()
{
	bool _isCanChange = false;
	bool _isDeleteFriend = false;
	//--判断是否需要友元id才能改变
	if (m_Data->getMyFriendId() != 0 && m_Data->getMyFriendId() == GameManager::getInstance()->GetCurChoicePropId())
	{
		_isDeleteFriend = true;
		_isCanChange = true;
	}
	else if (m_Data->getMyFriendId() == 0)
	{
		_isCanChange = true;
	}
	if (_isCanChange)
	{
		//--删除道具
		if (_isDeleteFriend)
		{
			GameManager::getInstance()->DeleteBagItem(m_Data->getMyFriendId());
		}
	}
	return _isCanChange;
}
/*
@通知放大的场景中，某种道具已经获取完毕
@
*/
void Game_Item::NoticeBigMapGetBagItemAndNil(const int & _id, const Vec2 _vec2)
{
	//---把对应Id的的标记设置为0
	//---解析字段
	auto all_data = StringHelper::splitString(m_Value, ";");
	string new_data = "";
	if (all_data.size() >= 2)
	{
		new_data.append(StringUtils::format("%s;", all_data.at(0).c_str()));
		for (int i = 1; i < all_data.size(); i++)
		{
			auto new_item_data = StringHelper::splitString(all_data.at(i), ",");
			//---满足5个数据
			if (new_item_data.size() == 5 && atoi(new_item_data.at(0).c_str()) == 1)
			{
				//--检测id 和坐标是否一致
				if (atoi(new_item_data.at(1).c_str()) == _id && atoi(new_item_data.at(2).c_str()) == (int)_vec2.x && atoi(new_item_data.at(3).c_str()) == (int)_vec2.y)
				{
					//--如果id和坐标相等 相等
					new_data.append(StringUtils::format("0,%s,%s,%s,%s;", new_item_data.at(1).c_str(), new_item_data.at(2).c_str(), new_item_data.at(3).c_str(), new_item_data.at(4).c_str()));
				}
				else{
					new_data.append(StringUtils::format("%s;", all_data.at(i).c_str()));
				}
			}
			else
			{
				//-字段长度不符合或者已经变为0了，直接添加
				new_data.append(StringUtils::format("%s;", all_data.at(i).c_str()));
			}
		}
		m_Value = new_data;
	}
	else
	{
		CCLOG("[Game_Item::NoticeBigMapGetBagItemAndNil]:Delete Item Data Erroy");
	}


}


/*
@通知大地图的某种东西改变
*/
void Game_Item::NoticeBigMapGameItemChange(const int & _id, const Vec2 & _vec2,const string & change_data)
{
	//---解析字段
	auto all_data = StringHelper::splitString(m_Value, ";");
	string new_data = "";
	if (all_data.size() >= 2)
	{
		new_data.append(StringUtils::format("%s;", all_data.at(0).c_str()));
		for (int i = 1; i < all_data.size(); i++)
		{
			auto new_item_data = StringHelper::splitString(all_data.at(i), ",");
			//---满足5个数据
			if (new_item_data.size() == 5 && atoi(new_item_data.at(0).c_str()) == 1)
			{
				//--检测id 和坐标是否一致
				if (atoi(new_item_data.at(1).c_str()) == _id && atoi(new_item_data.at(2).c_str()) == (int)_vec2.x && atoi(new_item_data.at(3).c_str()) == (int)_vec2.y)
				{
					//--如果id和坐标相等 相等-改变成新的数据
					new_data.append(StringUtils::format("%s,%s", new_item_data.at(0).c_str(), change_data.c_str()));
				}
				else{
					new_data.append(StringUtils::format("%s;", all_data.at(i).c_str()));
				}
			}
			else
			{
				//-字段长度不符合或者已经变为0了，直接添加
				new_data.append(StringUtils::format("%s;", all_data.at(i).c_str()));
			}
		}
		m_Value = new_data;
	}
	else
	{
		CCLOG("[Game_Item::NoticeBigMapGetBagItemAndNil]:Change Item Id Erroy");
	}
}

/*
@把数据拿出来生成道具放到大地图上
*/
void Game_Item::AddGameItemToBigMaoWithValueAndStartIndex(const string & item_value, const int & strart_index)
{
	auto new_item_data = StringHelper::splitString(item_value, ",");
	//---满足5个数据
	if (new_item_data.size() == 5 && atoi(new_item_data.at(0).c_str()) == 1)
	{
		auto item = GameManager::getInstance()->AddGameItemToMapWithIdAndMapSrc(atoi(new_item_data.at(strart_index).c_str()));
		item->setPosition(Vec2(atoi(new_item_data.at(1 + strart_index).c_str()), atoi(new_item_data.at(2 + strart_index).c_str())));
		if (new_item_data.at(3 + strart_index) != "0")
		{
			item->setMyValue(new_item_data.at(3 + strart_index));
		}
	}
	else if (new_item_data.size() == 4)
	{
		auto item = GameManager::getInstance()->AddGameItemToMapWithIdAndMapSrc(atoi(new_item_data.at(strart_index).c_str()));
		item->setPosition(Vec2(atoi(new_item_data.at(1 + strart_index).c_str()), atoi(new_item_data.at(2 + strart_index).c_str())));
		if (new_item_data.at(3 + strart_index) != "0")
		{
			item->setMyValue(new_item_data.at(3 + strart_index));
		}
	}
}

/*
@!GameItemType_TouchPopLock! 回调，得到密码已经解锁
*/
void Game_Item::MyLockIsOpen(bool _is)
{
	if (_is)
	{
		PlaySelfEffect();
		//--添加进小地图
		auto value_data = StringHelper::splitString(m_Data->getMyValue(), ";");
		this->setVisible(false);
		/*if ( 2 == value_data.size())
		{
		this->setVisible(false);
		auto new_item_data = StringHelper::splitString(value_data.at(1), ",");
		auto item = GameManager::getInstance()->AddGameItemToMapWithId(atoi(new_item_data.at(0).c_str()));
		item->setPosition(Vec2(atoi(new_item_data.at(1).c_str()), atoi(new_item_data.at(2).c_str())));
		if (new_item_data.at(3) != "0")
		{
		item->setMyValue(new_item_data.at(3));
		}
		}*/
		for (int i = 1; i < value_data.size(); i ++)
		{
			auto new_item_data = StringHelper::splitString(value_data.at(i), ",");
			auto item = GameManager::getInstance()->AddGameItemToMapWithId(atoi(new_item_data.at(0).c_str()));
			item->setPosition(Vec2(atoi(new_item_data.at(1).c_str()), atoi(new_item_data.at(2).c_str())));
			if (new_item_data.at(3) != "0")
			{
				item->setMyValue(new_item_data.at(3));
			}
		}
	}
}
/*
@=================================================================================================================
@=================================================================================================================
*/