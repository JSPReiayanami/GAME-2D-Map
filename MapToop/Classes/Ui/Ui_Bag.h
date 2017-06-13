#ifndef __Ui_Bag_H__
#define __Ui_Bag_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Ui_BagItem;
class Ui_Bag : public Interface_Entity
{
public:
	Ui_Bag();
	~Ui_Bag();
	CREATE_FUNC(Ui_Bag);
	/*
	@固定创建方法
	*/
	virtual bool init();

	/*
	@根节点
	*/
	Layout * m_Root;//根节点
	string   m_LayerSrc;//界面资源
	map<int, ImageView *> m_BagSpace;//物品的格子
	map<int, int> m_BagData;//物品的数据
	map<int, Ui_BagItem*> m_BagItem;//物品的实例
	/*
	@设置自己的名字
	*/
	void SetInterfaceName();
	/*
	@初始化资源
	*/
	void InitSrcLayout();
	/*
	@初始化控件
	*/
	void InitWidget();
	/*
		@初始化背包格子
	*/
	void InitBagSpace();
	/*
		@添加道具
	*/
	void AddPropToBag(PropData * p_data);
	void AddItemToBagWithId(const int & _id);
	/*
		@删除道具
	*/
	void DeletePropFromBag(int _id);
	/*
		@清空背包
	*/
	void ClearBag();
	/*
		@清除所有高亮的道具
	*/
	void ClearAllPropHightLight();
	/*
		@是否背包有该Id的物品
	*/
	bool IsHavePropWithId(const int & _id);
};
#endif // !__Ui_Notice_Text_H__