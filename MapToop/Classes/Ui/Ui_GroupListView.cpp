//
//  Ui_GroupListView.cpp
//  MakePizza
//
//  Created by xcode5 on 16/3/16.
//
//

#include "Ui_GroupListView.h"
//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataManager.h"
#include "Manager/SoundManager.h"
#else
#include "SceneManager.h"
#include "GameManager.h"
#include "DataManager.h"
#include "SoundManager.h"
#endif//判断路径的引用

#define  _Ui_GroupListView_Src "Ui/Ui_GroupListView.csb"
/*
 @构造,析构
 */
Ui_GroupListView::Ui_GroupListView():
m_CurChoiceType(0)
{
    m_LayerSrc = _Ui_GroupListView_Src;
}
Ui_GroupListView::~Ui_GroupListView()
{
    
}
Ui_GroupListView * Ui_GroupListView::CreateWithId(const vector<int> & _id)
{
    auto * ui_g_list = new(std::nothrow) Ui_GroupListView();
    if (ui_g_list)
    {
        ui_g_list->SetKindId(_id);
    }
    if (ui_g_list->init())
    {
        ui_g_list->autorelease();
        //game_move_item->RunActionWithType();
        return ui_g_list;
    }
    else
    {
        delete ui_g_list;
        ui_g_list = nullptr;
        return nullptr;
    }
}
/*
 @固定创建方法
 */
bool Ui_GroupListView::init()
{
    if (!Interface_Entity::init())
    {
        return false;
    }
    this->setTouchEnabled(false);
    InitView();
    return true;
}
/*
 @设置自己的名字
 */
void Ui_GroupListView::SetInterfaceName()
{
    this->setName("Ui_GroupListView");
    CCLOG("Ui_GroupListView::SetInterfaceName()");
}
/*
	@初始化资源
 */
void Ui_GroupListView::InitSrcLayout()
{
    m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
    this->addChild(m_Root);
    //---
    __GetChildAndAssignmentWithOB(m_KindDown, m_Root, "Button_KindDown", Button);
    __GetChildAndAssignmentWithOB(m_KindUp, m_Root, "Button_KindUp", Button);
    __GetChildAndAssignmentWithOB(m_ChildDown, m_Root, "Button_ChildDown", Button);
    __GetChildAndAssignmentWithOB(m_ChildUp, m_Root, "Button_ChildUp", Button);
    //--
    __GetChildAndAssignmentWithOB(m_ListKind, m_Root, "ListView_Kind", ListView);
    __GetChildAndAssignmentWithOB(m_ListChild, m_Root, "ListView_Child", ListView);
    m_KindUp->setVisible(false);
    m_ChildUp->setVisible(false);
}
/*
 @初始化按钮的回调事件
 */
void Ui_GroupListView::InitButtonEvent()
{
    m_ListKind->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(Ui_GroupListView::ItemKindSelectEvent, this));
    m_ListKind->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(Ui_GroupListView::ItemKindEventScrollView,this));
    m_ListChild->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(Ui_GroupListView::ItemChildSelectEvent, this));
    m_ListChild->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(Ui_GroupListView::ItemChildEventScrollView,this));
}
/*
 @按钮的回调事件
 */
void Ui_GroupListView::ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if (eventType == ui::Widget::TouchEventType::ENDED)
    {
        auto btn = (Button *)pSender;

        
    }
}
/*
 @初始化数据
 */
void Ui_GroupListView::InitData()
{
    
}
/*
 @初始化显示
 */
void Ui_GroupListView::InitView()
{
    //---初始化父种类链表
    m_ListKind->removeAllItems();
    for(auto & ite : m_KindId)
    {
        auto i_k_data = DataManager::getInstance()->GetItemKindDataWithId(ite);
        if (nullptr != i_k_data) {
            auto img = ImageView::create(i_k_data->getMySrc());
            img->setTouchEnabled(true);
            img->setTag(i_k_data->getMyType());
            m_ListKind->pushBackCustomItem(img);
            if(m_CurChoiceType == 0)
            {
                m_CurChoiceType = img->getTag();
            }
        }
        
    }
    //--刷新子类链表
}
/*
    @显示对应种类的物品
 */
void Ui_GroupListView::ShowKindTypeChild(const int & type)
{
    
}

/*
    @设置种类ID组合
 */
void Ui_GroupListView::SetKindId(const vector<int> & _id)
{
    for (auto & ite : _id )
    {
        m_KindId.push_back(ite);
    }
}
/*
 @父类点击事件获取
 */
void Ui_GroupListView::ItemKindSelectEvent(Ref *pSender, ListView::EventType type)
{
    switch (type)
    {
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
            //CCLOG("select child start index = %ld", listView->getCurSelectedIndex());
            break;
        }
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
            //CCLOG("select child end index = %ld", listView->getCurSelectedIndex());
            //--选中对应的种类的子类物品
            m_CurChoiceType = listView->getItem(listView->getCurSelectedIndex())->getTag();
            CCLOG("[Choice Type]:%d",m_CurChoiceType);
            break;
        }
        default:
            break;
    }
}
/*
 @父类链表的滑动事件
 */
void Ui_GroupListView::ItemKindEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            CCLOG("SCROLL_TO_BOTTOM");
            m_KindUp->setVisible(true);
            m_KindDown->setVisible(false);
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            CCLOG("SCROLL_TO_TOP");
            m_KindUp->setVisible(false);
            m_KindDown->setVisible(true);
            break;
        default:
            m_KindUp->setVisible(true);
            m_KindDown->setVisible(true);
            break;
    }
}

/*
 @子类点击事件获取
 */
void Ui_GroupListView::ItemChildSelectEvent(Ref *pSender, ListView::EventType type)
{
    switch (type)
    {
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
            //CCLOG("select child start index = %ld", listView->getCurSelectedIndex());
            break;
        }
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
            //CCLOG("select child end index = %ld", listView->getCurSelectedIndex());
            //--选中对应的种类的子类物品

            break;
        }
        default:
            break;
    }
}
/*
 @子类链表的滑动事件
 */
void Ui_GroupListView::ItemChildEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            CCLOG("SCROLL_TO_BOTTOM");
            m_ChildUp->setVisible(true);
            m_ChildDown->setVisible(false);
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            CCLOG("SCROLL_TO_TOP");
            m_ChildUp->setVisible(false);
            m_ChildDown->setVisible(true);
            break;
        default:
            m_ChildUp->setVisible(true);
            m_ChildDown->setVisible(true);
            break;
    }
}
/*
 @刷新子类的链表
 */
void Ui_GroupListView::RefreshChildListView()
{
    
}

