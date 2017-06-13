#include "Ui_BagItem.h"
#include "Manager/DataManager.h"
#include "Manager/GameManager.h"
#define  _Ui_Bag_Src "UI/Ui_BagItem.csb"
/*
@构造,析构
*/
Ui_BagItem::Ui_BagItem():
m_Id(0),
m_IsChoice(false),
m_ButtonItem(nullptr),
m_TextName(nullptr),
m_ImageChoice(nullptr)
{
	m_LayerSrc = _Ui_Bag_Src;
}
Ui_BagItem::~Ui_BagItem()
{

}
Ui_BagItem * Ui_BagItem::CreateWithId(const int & _id)
{
	auto *ui_bagitem = new(std::nothrow) Ui_BagItem();
	if (ui_bagitem)
	{
		ui_bagitem->setMyId(_id);

	}
	if (ui_bagitem->init())
	{
		ui_bagitem->autorelease();
		return ui_bagitem;
	}
	else
	{
		delete ui_bagitem;
		ui_bagitem = nullptr;
		return nullptr;
	}
}
/*
@固定创建方法
*/
bool Ui_BagItem::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}
	RefreshView();
	return true;
}
/*
@设置自己的名字
*/
void Ui_BagItem::SetInterfaceName()
{
	this->setName("Ui_BagItem");
	CCLOG("Ui_BagItem");
}
/*
@初始化资源
*/
void Ui_BagItem::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}
/*
@初始化控件
*/
void Ui_BagItem::InitWidget()
{
	__GetChildAndAssignmentWithOB(m_ButtonItem, m_Root, "Button_Item", Button)
	__GetChildAndAssignmentWithOB(m_TextName, m_Root, "Text_Name", Text)
	__GetChildAndAssignmentWithOB(m_ImageChoice, m_Root, "Image_Choice", ImageView)
}
/*
@初始化按钮的回调事件
*/
void Ui_BagItem::InitButtonEvent()
{
	__AddTouchEventWithNameAndPNode("Button_Item", m_Root, Ui_BagItem::TouchEvent)
}
/*
@刷新显示
*/
void Ui_BagItem::RefreshView()
{
	auto item_data = DataManager::getInstance()->GetGameItemDataWithId(m_Id);
	if (item_data)
	{
		m_ButtonItem->loadTextureNormal(item_data->getMyBagIco().c_str());
		m_TextName->setString(item_data->getMyName());
		m_Value = atoi(item_data->getMyValue().c_str());
	}
	CC_SAFE_DELETE(item_data);
}
/*
@按钮的回调事件
*/
void Ui_BagItem::TouchEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::BEGAN)
	{
		CCLOG("Touch BagItem Id : %d",m_Id);
		bool _is = false;
		if (m_IsChoice)
		{
			GameManager::getInstance()->SetChoicePropId(0);
			_is = false;
		}
		else
		{
			GameManager::getInstance()->SetChoicePropId(m_Id);
			_is = true;
		}
		
		SetIsChoice(_is);
	}
	
}

void Ui_BagItem::SetIsChoice(bool _is)
{
	if (m_IsChoice != _is)
	{
		m_ImageChoice->setVisible(_is);
		m_IsChoice = _is;
	}
}


