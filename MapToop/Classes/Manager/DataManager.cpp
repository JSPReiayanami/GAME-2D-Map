#include "DataManager.h"

//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Tool/XMLTinyUtil.h"
#include "Tool/StringHelper.h"
#else
#include "XMLTinyUtil.h"
#include "StringHelper.h"
#endif//判断路径的引用
USING_NS_CC;


#define  __GameMoveItemConfigSrc "Config/GameMoveItem.xml"
#define  __GameMoveTimeItemConfigSrc "Config/GameMoveTimeItem.xml"
#define  __GameGameMixItemConfigSrc "Config/GameMixItem.xml"
#define  __ItemKindConfigSrc "Config/ItemKind.xml"
#define  __ItemChildConfigSrc "Config/ItemChild.xml"
#define  __NoticeStringConfigSrc "Config/NoticeString.xml"
DataManager::DataManager():
m_IsLoadXML(false)
{
	m_GameMoveItemData.clear();
}


DataManager::~DataManager()
{
}

//===== get the manger ====
DataManager * DataManager::d_dataManger = nullptr;
DataManager * DataManager::getInstance(){
	if (d_dataManger == nullptr)
	{
		d_dataManger = new  DataManager();
	}
	return d_dataManger;
}

bool DataManager::init(){
	if (!Node::init())
	{
		return false;
	}
	//LoadConfigXML();
	return true;
}
/*
@读取配置文件
*/
void DataManager::LoadConfigXML(){
	if (m_IsLoadXML == false)
	{
		
		m_IsLoadXML = true;
		LoadGameMoveItemXML();
        LoadGameMoveTimeItemXML();
        LoadGameMixItemXML();
        LoadItemKindXML();
        LoadItemChildXML();
        LoadNoticeStringXML();
		CCLOG("XML Is Load End");
	}
	else{
		CCLOG("XML Is Loaded");
	}
}

/*
@销毁数据释放内存
*/
void DataManager::DeleteConfigXML()
{
	if (m_IsLoadXML)
	{
		DeleteGameMoveItemData();
        DeleteGameMoveTimeItemData();
        DeleteGameMixItemData();
        DeleteItemKindData();
        DeleteItemChildData();
        DeleteNoticeStringData();
		m_IsLoadXML = false;
		CCLOG("[DeleteConfigXML]:Delete Is Ok");
	}
	else{
		CCLOG("[DeleteConfigXML]:Delete Error");
	}
}

/*
@当临时改变配置表后需要刷新,不可在游戏场景内刷新
*/
void DataManager::RefreshConfigXML()
{
	DeleteConfigXML();
	LoadConfigXML();
}
/*
@读取道具的表格数据
*/
void DataManager::LoadGameItemXML()
{
	
}

/*
@销毁道具的表格数据
*/
void DataManager::DeleteGameItemData()
{

}

/*
@读取移动类道具配置
*/
void DataManager::LoadGameMoveItemXML()
{
	CCLOG("strat LoadGameMoveItemXML is start");
	auto xmlTiny = XMLTinyUtil::create(__GameMoveItemConfigSrc);
	auto iteEntity = xmlTiny->loadRootElement();
	while (iteEntity)
	{
		GameMoveItemData * game_move_item = new GameMoveItemData();
		auto attribute = iteEntity->FirstAttribute();
		//--道具的属性
		while (attribute)
		{
			if (!strcmp(attribute->Name(), "Id"))
			{
				game_move_item->setMyId(attribute->IntValue());
			}else
			if (!strcmp(attribute->Name(), "FriendId"))
			{
				game_move_item->setMyFriendId(attribute->IntValue());
			}else
			if (!strcmp(attribute->Name(), "Name"))
			{
				game_move_item->setMyName(attribute->Value());
			}else
			if (!strcmp(attribute->Name(), "Description"))
			{
				game_move_item->setMyDescription(attribute->Value());
			}else
			if (!strcmp(attribute->Name(), "Src"))
			{
				game_move_item->setMySrc(attribute->Value());
			}else
			if (!strcmp(attribute->Name(), "Value"))
			{
				game_move_item->setMyValue(attribute->Value());
			}
			else
			if (!strcmp(attribute->Name(), "MusicId"))
			{
				game_move_item->setMyMusicId(attribute->IntValue());
			}
			else
			if (!strcmp(attribute->Name(), "SPosX"))
			{
				Vec2 pos = game_move_item->getMySPos();
				game_move_item->setMySPos(Vec2(attribute->FloatValue(), pos.y));
			}
			else
			if (!strcmp(attribute->Name(), "SPosY"))
			{
				Vec2 pos = game_move_item->getMySPos();
				game_move_item->setMySPos(Vec2(pos.x, attribute->FloatValue()));
			}
			else
			if (!strcmp(attribute->Name(), "EndSrc"))
			{
				game_move_item->setMyEndSrc(attribute->Value());
			}
            else
            if (!strcmp(attribute->Name(), "EndRect"))
            {
                auto end_rect_v = attribute->Value();
                auto end_r_data = StringHelper::splitString(end_rect_v, ",");
                CCLOG("Size[%f][%f]",__TransformPercentWidthToViewSize(atof(end_r_data.at(2).c_str())),__TransformPercentHeightToViewSize(atof(end_r_data.at(3).c_str())));
                game_move_item->setMyEndRect(
                                             Rect(
                                                  atof(end_r_data.at(0).c_str()),
                                                  atof(end_r_data.at(1).c_str()),
                                                  __TransformPercentWidthToViewSize(atof(end_r_data.at(2).c_str())),
                                                  __TransformPercentHeightToViewSize(atof(end_r_data.at(3).c_str()))
                                                  )
                                             );
            }
			else
			if (!strcmp(attribute->Name(), "Type"))
			{
				game_move_item->setMyType(attribute->IntValue());
			}
			attribute = attribute->Next();
		}
		
		iteEntity = iteEntity->NextSiblingElement();
		m_GameMoveItemData.insert(make_pair(game_move_item->getMyId(), game_move_item));
	}
	CC_SAFE_DELETE(xmlTiny);
}
/*
@读取移动类道具配置
*/
void DataManager::DeleteGameMoveItemData()
{
	for (auto & item_d : m_GameMoveItemData)
	{
		CC_SAFE_DELETE(item_d.second);
	}
	m_GameMoveItemData.clear();
}




/*
 @读取范围时间类道具配置
 */
void DataManager::LoadGameMoveTimeItemXML()
{
    CCLOG("strat LoadGameItemXML is start");
    auto xmlTiny = XMLTinyUtil::create(__GameMoveTimeItemConfigSrc);
    auto iteEntity = xmlTiny->loadRootElement();
    while (iteEntity)
    {
        GameMoveTimeItemData * game_move_time_item = new GameMoveTimeItemData();
        auto attribute = iteEntity->FirstAttribute();
        //--道具的属性
        while (attribute)
        {
            if (!strcmp(attribute->Name(), "Id"))
            {
                game_move_time_item->setMyId(attribute->IntValue());
            }else
            if (!strcmp(attribute->Name(), "FriendId"))
            {
                game_move_time_item->setMyFriendId(attribute->IntValue());
            }else
            if (!strcmp(attribute->Name(), "Name"))
            {
                game_move_time_item->setMyName(attribute->Value());
            }else
            if (!strcmp(attribute->Name(), "Description"))
            {
                game_move_time_item->setMyDescription(attribute->Value());
            }else
            if (!strcmp(attribute->Name(), "Src"))
            {
                game_move_time_item->setMySrc(attribute->Value());
            }else
            if (!strcmp(attribute->Name(), "Value"))
            {
                game_move_time_item->setMyValue(attribute->Value());
            }
            else
            if (!strcmp(attribute->Name(), "MusicId"))
            {
                game_move_time_item->setMyMusicId(attribute->IntValue());
            }
            else
            if (!strcmp(attribute->Name(), "SPosX"))
            {
                Vec2 pos = game_move_time_item->getMySPos();
                game_move_time_item->setMySPos(Vec2(attribute->FloatValue(), pos.y));
            }
            else
            if (!strcmp(attribute->Name(), "SPosY"))
            {
                Vec2 pos = game_move_time_item->getMySPos();
                game_move_time_item->setMySPos(Vec2(pos.x, attribute->FloatValue()));
            }
            else
            if (!strcmp(attribute->Name(), "PointRect"))
            {
                auto end_rect_v = attribute->Value();
                auto end_r_data = StringHelper::splitString(end_rect_v, ",");
                game_move_time_item->setMyPointRect(
                                                  Rect(
                                                       atof(end_r_data.at(0).c_str()),
                                                       atof(end_r_data.at(1).c_str()),
                                                       __TransformPercentWidthToViewSize(atof(end_r_data.at(2).c_str())),
                                                       __TransformPercentHeightToViewSize(atof(end_r_data.at(3).c_str()))
                                                       )
                                                  );
            }
            else
            if (!strcmp(attribute->Name(), "EndRect"))
            {
                auto end_rect_v = attribute->Value();
                auto end_r_data = StringHelper::splitString(end_rect_v, ",");
                game_move_time_item->setMyEndRect(
                                         Rect(
                                              __TransformPercentWidthToViewSize(atof(end_r_data.at(0).c_str())),
                                              __TransformPercentHeightToViewSize(atof(end_r_data.at(1).c_str())),
                                              __TransformPercentWidthToViewSize(atof(end_r_data.at(2).c_str())),
                                              __TransformPercentHeightToViewSize(atof(end_r_data.at(3).c_str()))
                                              )
                                         );
            }
            else
            if (!strcmp(attribute->Name(), "Type"))
            {
                game_move_time_item->setMyType(attribute->IntValue());
            }
            attribute = attribute->Next();
        }
        
        iteEntity = iteEntity->NextSiblingElement();
        m_GameMoveTimeItemData.insert(make_pair(game_move_time_item->getMyId(), game_move_time_item));
    }
    CC_SAFE_DELETE(xmlTiny);
}
/*
 @读取范围时间类道具配置
 */
void DataManager::DeleteGameMoveTimeItemData()
{
    for (auto & item_d : m_GameMoveTimeItemData)
    {
        CC_SAFE_DELETE(item_d.second);
    }
    m_GameMoveTimeItemData.clear();
}

/*
 @读取搅拌类数据
 */
void DataManager::LoadGameMixItemXML()
{
    CCLOG("strat LoadGameMixItemXML is start");
    auto xmlTiny = XMLTinyUtil::create(__GameGameMixItemConfigSrc);
    auto iteEntity = xmlTiny->loadRootElement();
    while (iteEntity)
    {
        GameMixItemData * g_data = new GameMixItemData();
        auto attribute = iteEntity->FirstAttribute();
        //--道具的属性
        while (attribute)
        {
        if (!strcmp(attribute->Name(), "Id"))
        {
            g_data->setMyId(attribute->IntValue());
        }else
        if (!strcmp(attribute->Name(), "FriendId"))
        {
            g_data->setMyFriendId(attribute->IntValue());
        }else
        if (!strcmp(attribute->Name(), "Name"))
        {
            g_data->setMyName(attribute->Value());
        }else
        if (!strcmp(attribute->Name(), "Description"))
        {
            g_data->setMyDescription(attribute->Value());
        }else
        if (!strcmp(attribute->Name(), "Src"))
        {
            g_data->setMySrc(attribute->Value());
        }else
        if (!strcmp(attribute->Name(), "Value"))
        {
            g_data->setMyValue(attribute->Value());
        }
        else
        if (!strcmp(attribute->Name(), "MusicId"))
        {
            g_data->setMyMusicId(attribute->IntValue());
        }
        else
        if (!strcmp(attribute->Name(), "SPosX"))
        {
            Vec2 pos = g_data->getMySPos();
            g_data->setMySPos(Vec2(attribute->FloatValue(), pos.y));
        }
        else
        if (!strcmp(attribute->Name(), "SPosY"))
        {
            Vec2 pos = g_data->getMySPos();
            g_data->setMySPos(Vec2(pos.x, attribute->FloatValue()));
        }
        else
        if (!strcmp(attribute->Name(), "Type"))
        {
            g_data->setMyType(attribute->IntValue());
        }
            attribute = attribute->Next();
        }
        
        iteEntity = iteEntity->NextSiblingElement();
        m_GameMixItemData.insert(make_pair(g_data->getMyId(), g_data));
    }
    CC_SAFE_DELETE(xmlTiny);
}
/*
 @删除搅拌类数据
 */
void DataManager::DeleteGameMixItemData()
{
    for (auto & item_d : m_GameMixItemData)
    {
        CC_SAFE_DELETE(item_d.second);
    }
    m_GameMixItemData.clear();
}


/*
 @读取种类道具数据
 */
void DataManager::LoadItemKindXML()
{
    CCLOG("strat LoadGameMixItemXML is start");
    auto xmlTiny = XMLTinyUtil::create(__ItemKindConfigSrc);
    auto iteEntity = xmlTiny->loadRootElement();
    while (iteEntity)
    {
        ItemKindData * i_data = new ItemKindData();
        auto attribute = iteEntity->FirstAttribute();
        //--道具的属性
        while (attribute)
        {
            if (!strcmp(attribute->Name(), "Id"))
            {
                i_data->setMyId(attribute->IntValue());
            }else
            if (!strcmp(attribute->Name(), "Name"))
            {
                i_data->setMyName(attribute->Value());
            }else
            if (!strcmp(attribute->Name(), "Description"))
            {
                i_data->setMyDescription(attribute->Value());
            }else
            if (!strcmp(attribute->Name(), "Src"))
            {
                i_data->setMySrc(attribute->Value());
            }
            else
            if (!strcmp(attribute->Name(), "MusicId"))
            {
                i_data->setMyMusicId(attribute->IntValue());
            }
            else
            if (!strcmp(attribute->Name(), "Type"))
            {
                i_data->setMyType(attribute->IntValue());
            }
            attribute = attribute->Next();
        }
        
        iteEntity = iteEntity->NextSiblingElement();
        m_ItemKindData.insert(make_pair(i_data->getMyId(), i_data));
    }
    CC_SAFE_DELETE(xmlTiny);
}
/*
 @删除种类道具数据
 */
void DataManager::DeleteItemKindData()
{
    for (auto & item_d : m_ItemKindData)
    {
        CC_SAFE_DELETE(item_d.second);
    }
    m_ItemKindData.clear();
}



/*
 @读取子类道具数据
 */
void DataManager::LoadItemChildXML()
{
    CCLOG("strat LoadItemChildXML is start");
    auto xmlTiny = XMLTinyUtil::create(__ItemChildConfigSrc);
    auto iteEntity = xmlTiny->loadRootElement();
    while (iteEntity)
    {
        ItemChildData * i_data = new ItemChildData();
        auto attribute = iteEntity->FirstAttribute();
        //--道具的属性
        while (attribute)
        {
            if (!strcmp(attribute->Name(), "Id"))
            {
                i_data->setMyId(attribute->IntValue());
            }else
            if (!strcmp(attribute->Name(), "Name"))
            {
                i_data->setMyName(attribute->Value());
            }else
            if (!strcmp(attribute->Name(), "Description"))
            {
                i_data->setMyDescription(attribute->Value());
            }else
            if (!strcmp(attribute->Name(), "Src"))
            {
                i_data->setMySrc(attribute->Value());
            }
            else
            if (!strcmp(attribute->Name(), "MusicId"))
            {
                i_data->setMyMusicId(attribute->IntValue());
            }
            else
            if (!strcmp(attribute->Name(), "Type"))
            {
                i_data->setMyType(attribute->IntValue());
            }
            else
            if (!strcmp(attribute->Name(), "Tag"))
            {
                i_data->setMyTag(attribute->IntValue());
            }
            else
            if (!strcmp(attribute->Name(), "Ico"))
            {
                i_data->setMyIco(attribute->Value());
            }
            else
            if (!strcmp(attribute->Name(), "IsLock"))
            {
                if ( 0 == attribute->IntValue())
                {
                    i_data->setMyIsLock(false);
                }else{
                    i_data->setMyIsLock(true);
                }
                
            }
            attribute = attribute->Next();
        }
        
        iteEntity = iteEntity->NextSiblingElement();
        m_AllItemChildData.insert(make_pair(i_data->getMyId(), i_data));
    }
    //--分类
    for (auto & i_data : m_AllItemChildData)
    {
        auto data = i_data.second;
        if (m_KindItemChildData.find(data->getMyType()) == m_KindItemChildData.end())
        {
            //--第一次
            map<int,ItemChildData *> temp;
            temp.clear();
            temp.insert(make_pair(data->getMyId(), data));
            m_KindItemChildData.insert(make_pair(data->getMyType(),temp));
            
        }else{
            m_KindItemChildData.at(data->getMyType()).insert(make_pair(data->getMyId(), data));
        }
    }
    CC_SAFE_DELETE(xmlTiny);
}
/*
 @删除子类道具数据
 */
void DataManager::DeleteItemChildData()
{
    for (auto & item_d : m_AllItemChildData)
    {
        CC_SAFE_DELETE(item_d.second);
    }
    m_AllItemChildData.clear();
    m_KindItemChildData.clear();
}


/*
 @提示类数据
 */
void DataManager::LoadNoticeStringXML()
{
    
    CCLOG("strat LoadNoticeStringXML is start");
    auto xmlTiny = XMLTinyUtil::create(__NoticeStringConfigSrc);
    auto iteEntity = xmlTiny->loadRootElement();
    while (iteEntity)
    {
        NoticeStringData * i_data = new NoticeStringData();
        auto attribute = iteEntity->FirstAttribute();
        //--道具的属性
        while (attribute)
        {
            if (!strcmp(attribute->Name(), "Id"))
            {
                i_data->setMyId(attribute->IntValue());
            }else
            if (!strcmp(attribute->Name(), "Description"))
            {
                i_data->setMyDescription(attribute->Value());
            }else
            if (!strcmp(attribute->Name(), "Value"))
            {
                i_data->setMyValue(attribute->Value());
            }
            attribute = attribute->Next();
        }
        
        iteEntity = iteEntity->NextSiblingElement();
        m_NoticeStringData.insert(make_pair(i_data->getMyId(), i_data));
    }
    CC_SAFE_DELETE(xmlTiny);
}
/*
 @删除提示类
 */
void DataManager::DeleteNoticeStringData()
{
    for (auto & item_d : m_NoticeStringData)
    {
        CC_SAFE_DELETE(item_d.second);
    }
    m_NoticeStringData.clear();
}


//========

/*
 @获取移动类道具的数据根据Id
 */
GameMoveItemData * DataManager::GetMoveItemDataWithId(const int & _id)
{
    GameMoveItemData * g_move_item = nullptr;
    if (m_GameMoveItemData.find(_id) != m_GameMoveItemData.end())
    {
        g_move_item = m_GameMoveItemData.at(_id);
    }
    return g_move_item;
}
/*
 @获取范围时间类道具的数据根据Id
 */
GameMoveTimeItemData * DataManager::GetMoveItemTimeDataWithId(const int & _id)
{
    GameMoveTimeItemData * g_move_item = nullptr;
    if (m_GameMoveTimeItemData.find(_id) != m_GameMoveTimeItemData.end())
    {
        g_move_item = m_GameMoveTimeItemData.at(_id);
    }
    return g_move_item;
}

/*
 @获取范围时间类道具的数据根据Id
 */
GameMixItemData * DataManager::GetGameMixItemDataWithId(const int & _id)
{
    GameMixItemData * data = nullptr;
    if (m_GameMixItemData.find(_id) != m_GameMixItemData.end())
    {
        data = m_GameMixItemData.at(_id);
    }
    return data;
}
/*
 @获取种类道具的数据根据Id
 */
ItemKindData * DataManager::GetItemKindDataWithId(const int & _id)
{
    ItemKindData * data = nullptr;
    if (m_ItemKindData.find(_id) != m_ItemKindData.end())
    {
        data = m_ItemKindData.at(_id);
    }
    return data;
}
/*
 @获取子类道具的数据根据Id
 */
ItemChildData * DataManager::GetItemChildDataWithId(const int & _id)
{
    ItemChildData * data = nullptr;
    if (m_AllItemChildData.find(_id) != m_AllItemChildData.end())
    {
        data = m_AllItemChildData.at(_id);
    }
    return data;
}

map<int , ItemChildData *> DataManager::GetKindItemChildWithType(const int & _type)
{
    map<int , ItemChildData *> data;
    data.clear();
    if (m_KindItemChildData.find(_type) != m_KindItemChildData.end())
    {
        data = m_KindItemChildData.at(_type);
    }
    return data;
}

/*
 @获取提示类的数据根据Id
 */
NoticeStringData * DataManager::GetNoticeStringDataWithId(const int & _id)
{
    NoticeStringData * data = nullptr;
    if (m_NoticeStringData.find(_id) != m_NoticeStringData.end())
    {
        data = m_NoticeStringData.at(_id);
    }
    return data;
}
