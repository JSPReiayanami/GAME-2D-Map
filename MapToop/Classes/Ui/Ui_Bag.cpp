#include "Ui_Bag.h"
#include "Ui_BagItem.h"
#include "Manager/GameManager.h"
#define  _Ui_Bag_Src "UI/Ui_Bag.csb"
/*
@����,����
*/
Ui_Bag::Ui_Bag()
{
	m_LayerSrc = _Ui_Bag_Src;
	m_BagData.clear();
	m_BagSpace.clear();
	m_BagItem.clear();
	//--��ʼ���ձ���
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
	@�̶���������
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
@�����Լ�������
*/
void Ui_Bag::SetInterfaceName()
{
	this->setName("Ui_Bag");
	CCLOG("Ui_Bag");
}
/*
@��ʼ����Դ
*/
void Ui_Bag::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}
/*
@��ʼ���ؼ�
*/
void Ui_Bag::InitWidget()
{
	InitBagSpace();
}
/*
	@��ʼ����������
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
	@��ӵ���
*/
void Ui_Bag::AddPropToBag(PropData * p_data)
{
	//--���ͼ��
	auto bag_item = Ui_BagItem::CreateWithId(p_data->getMyId());
	//--�ҵ������һ��λ��
	bool is_su = false;
	for (int i = 1; i <= __BagMaxNum;i++)
	{
		if (m_BagData.find(i) == m_BagData.cend())
		{
			//---�������λ���ǿյ�
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
	//--���ͼ��
	auto bag_item = Ui_BagItem::CreateWithId(_id);
	//--�ҵ������һ��λ��
	bool is_su = false;
	for (int i = 1; i <= __BagMaxNum; i++)
	{
		if (m_BagData.at(i) == 0)
		{
			//---�������λ���ǿյ�
			is_su = true;
			m_BagData[i] = _id;
			m_BagSpace.at(i)->addChild(bag_item);
			m_BagItem[i] = bag_item;
			break;
		}
	}
}

/*
	@ɾ������
*/
void Ui_Bag::DeletePropFromBag(int _id)
{
	
	for (int i = 1; i <= __BagMaxNum; i++)
	{
		if ( 0 != m_BagData.at(i))
		{
			//--�ҵ�Ҫɾ���ĵ���
			if (m_BagData.at(i) == _id)
			{
				//--�жϸõ����Ƿ�������ʹ�õ�
				if (1 == m_BagItem.at(i)->getMyValue())
				{
					//--������ʹ�ò�ɾ��
				}
				else if (0 == m_BagItem.at(i)->getMyValue())
				{
					//--ֻ����һ��
					m_BagSpace.at(i)->removeAllChildrenWithCleanup(true);
					m_BagData[i] = 0;
					m_BagItem[i] = nullptr;
					if (_id == GameManager::getInstance()->GetCurChoicePropId())
					{
						GameManager::getInstance()->SetChoicePropId(0);
					}
				}//--������չ
				
			}
		}
	}
}
/*
	@��ձ���
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
	@������и����ĵ���
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
	@�Ƿ񱳰��и�Id����Ʒ
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

