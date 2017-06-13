#include "Ui_Bag.h"
#include "Ui_BagItem.h"
#include "Manager/GameManager.h"
#define  _Ui_Bag_Src "UI/Ui_Bag.csb"
/*
@构造,析构
*/
Ui_Bag::Ui_Bag()
{
	m_LayerSrc = _Ui_Bag_Src;
	m_BagData.clear();
	m_BagSpace.clear();
	m_BagItem.clear();
	//--初始化空背包
	for (int i = 0; i <= __BagMaxNum; i++)
	{
		m_BagItem.insert(make_pair(i, nullptr));
		m_BagData.insert(make_pair(i, 0));
	}
}
Ui_Bag::~Ui_Bag()
{

}
/*
	@固定创建方法
*/
bool Ui_Bag::init()
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
void Ui_Bag::SetInterfaceName()
{
	this->setName("Ui_Bag");
	CCLOG("Ui_Bag");
}
/*
@初始化资源
*/
void Ui_Bag::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}
/*
@初始化控件
*/
void Ui_Bag::InitWidget()
{
	InitBagSpace();
}
/*
	@初始化背包格子
*/
void Ui_Bag::InitBagSpace()
{
	
	for (int i = 1; i <= 5; i ++)
	{
		ImageView * tp_img = nullptr;
		__GetChildAndAssignmentWithOB(tp_img, m_Root, StringUtils::format("Image_Space_%d", i).c_str(), ImageView);
		m_BagSpace.insert(make_pair(i, tp_img));
	}
}
/*
	@添加道具
*/
void Ui_Bag::AddPropToBag(PropData * p_data)
{
	//--添加图像
	auto bag_item = Ui_BagItem::CreateWithId(p_data->getMyId());
	//--找到最近的一个位置
	bool is_su = false;
	for (int i = 1; i <= __BagMaxNum;i++)
	{
		if (m_BagData.find(i) == m_BagData.cend())
		{
			//---这个数据位置是空的
			is_su = true;
			m_BagData[i] = p_data->getMyId();
			m_BagSpace.at(i)->addChild(bag_item);
			m_BagItem[i] = bag_item;
			break;
		}
	}
}
void Ui_Bag::AddItemToBagWithId(const int & _id)
{
	//--添加图像
	auto bag_item = Ui_BagItem::CreateWithId(_id);
	//--找到最近的一个位置
	bool is_su = false;
	for (int i = 1; i <= __BagMaxNum; i++)
	{
		if (m_BagData.at(i) == 0)
		{
			//---这个数据位置是空的
			is_su = true;
			m_BagData[i] = _id;
			m_BagSpace.at(i)->addChild(bag_item);
			m_BagItem[i] = bag_item;
			break;
		}
	}
}

/*
	@删除道具
*/
void Ui_Bag::DeletePropFromBag(int _id)
{
	
	for (int i = 1; i <= __BagMaxNum; i++)
	{
		if ( 0 != m_BagData.at(i))
		{
			//--找到要删除的道具
			if (m_BagData.at(i) == _id)
			{
				//--判断该道具是否是无限使用的
				if (1 == m_BagItem.at(i)->getMyValue())
				{
					//--是无限使用不删除
				}
				else if (0 == m_BagItem.at(i)->getMyValue())
				{
					//--只是用一次
					m_BagSpace.at(i)->removeAllChildrenWithCleanup(true);
					m_BagData[i] = 0;
					m_BagItem[i] = nullptr;
					if (_id == GameManager::getInstance()->GetCurChoicePropId())
					{
						GameManager::getInstance()->SetChoicePropId(0);
					}
				}//--可以拓展
				
			}
		}
	}
}
/*
	@清空背包
*/
void Ui_Bag::ClearBag()
{
	for (int i = 1; i <= __BagMaxNum; i++)
	{
		m_BagItem[i] = nullptr;
		m_BagData[i] = 0;
		m_BagSpace.at(i)->removeAllChildrenWithCleanup(true);
	}
}

/*
	@清除所有高亮的道具
*/
void Ui_Bag::ClearAllPropHightLight()
{
	for (int i = 1; i <= __BagMaxNum; i++)
	{
		if (m_BagItem.at(i) !=  nullptr)
		{
			m_BagItem.at(i)->SetIsChoice(false);
		}
	}
}
/*
	@是否背包有该Id的物品
*/
bool Ui_Bag::IsHavePropWithId(const int & _id)
{
	bool is_have = false;
	for (auto & bag_data : m_BagData)
	{
		if (bag_data.second == _id)
		{
			is_have = true;
		}
	}
	return is_have;
}

