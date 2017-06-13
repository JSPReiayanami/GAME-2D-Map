#ifndef __DATAMANGER_H_
#define __DATAMANGER_H_
#include "cocos2d.h"
#include "Head.h"

#if (__HeadFileIncludeNum == __HeadFileHavePath)

#else

#endif//判断路径的引用


class DataManager :
	public cocos2d::Node
{
public:
	DataManager();
	~DataManager();
	/*
	*固定创建方法
	*/
	virtual bool init();
	CREATE_FUNC(DataManager);
	/*
	* 获取单例场景管理者
	*/
	static DataManager * getInstance();
	/*
		@读取配置文件
	*/
	void LoadConfigXML();
	/*
		@销毁数据释放内存
	*/
	void DeleteConfigXML();
	/*
		@当临时改变配置表后需要刷新,不可在游戏场景内刷新
	*/
	void RefreshConfigXML();
	/*
	* 单例的指针
	*/
	static DataManager * d_dataManger;
	/*
		@属性
	*/
	bool m_IsLoadXML;//是否已经读取了XML配置文件
	map<int, GameMoveItemData *> m_GameMoveItemData;
    map<int, GameMoveTimeItemData *> m_GameMoveTimeItemData;
    map<int, GameMixItemData *> m_GameMixItemData;
    map<int, ItemKindData *> m_ItemKindData;
    map<int,ItemChildData *> m_AllItemChildData;
    map<int,map<int,ItemChildData *>> m_KindItemChildData;
    map<int, NoticeStringData *> m_NoticeStringData;
	//========================
	/*
		@读取道具的表格数据
	*/
	void LoadGameItemXML();
	/*
		@销毁道具的表格数据
	*/
	void DeleteGameItemData();
	/*
        @读取移动类道具配置
	*/
	void LoadGameMoveItemXML();
	/*
        @读取移动类道具配置
	*/
	void DeleteGameMoveItemData();
    /*
        @读取范围时间类道具配置
     */
    void LoadGameMoveTimeItemXML();
    /*
        @读取范围时间类道具配置
     */
    void DeleteGameMoveTimeItemData();
    /*
        @读取搅拌类数据
     */
    void LoadGameMixItemXML();
    /*
        @删除搅拌类数据
     */
    void DeleteGameMixItemData();
    /*
        @读取种类道具数据
     */
    void LoadItemKindXML();
    /*
        @删除种类道具数据
     */
    void DeleteItemKindData();
    /*
        @读取子类道具数据
     */
    void LoadItemChildXML();
    /*
        @删除子类道具数据
     */
    void DeleteItemChildData();
    /*
     @提示类数据
     */
    void LoadNoticeStringXML();
    /*
     @删除提示类
     */
    void DeleteNoticeStringData();
	//===========================
	/*
		@获取移动类道具的数据根据Id
	*/
	GameMoveItemData * GetMoveItemDataWithId(const int & _id);
    /*
        @获取范围时间类道具的数据根据Id
     */
    GameMoveTimeItemData * GetMoveItemTimeDataWithId(const int & _id);
    /*
        @获取范围时间类道具的数据根据Id
     */
    GameMixItemData * GetGameMixItemDataWithId(const int & _id);
    /*
        @获取种类道具的数据根据Id
     */
    ItemKindData * GetItemKindDataWithId(const int & _id);
    /*
        @获取子类道具的数据根据Id
     */
    ItemChildData * GetItemChildDataWithId(const int & _id);
    map<int , ItemChildData *> GetKindItemChildWithType(const int & _type);
    /*
        @获取提示类的数据根据Id
     */
    NoticeStringData * GetNoticeStringDataWithId(const int & _id);
};
#endif

