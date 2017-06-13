#include "Interface_Game_Scene_1.h"


//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataManager.h"
#include "Manager/SoundManager.h"
#include "Game/Game_MoveItem"
#include "Game/Game_MoveTimeItem.h"
#include "Game/Game_MixItem.h"
#include "Manager/NoticeManager.h"
#else
#include "SceneManager.h"
#include "GameManager.h"
#include "DataManager.h"
#include "SoundManager.h"
#include "Game_MoveItem.h"
#include "Game_MoveTimeItem.h"
#include "Game_MixItem.h"
#include "NoticeManager.h"
#include "Ui_StringNotice.h"
#endif//判断路径的引用

#define  _Interface_Game_Scene_1_Src "Interface/Interface_GameScene_1.csb"
#define  _Interface_Game_Scene_1_TimePorpId 20001
#define  _Interface_Game_Scene_1_MixPorpId 30001
#define  _MixDosingVec2 Vec2(577,750)
/*
 @构造,析构
 */
Interface_Game_Scene_1::Interface_Game_Scene_1():
m_MixItem(nullptr),
m_IsCanMix(false),
m_ImageChoiceDosing(nullptr),
m_DosingTag(1),
m_RectEndItem(0,0,0,0)
{
    m_LayerSrc = _Interface_Game_Scene_1_Src;
    m_PropIds.clear();
    m_PropState.clear();
    m_Props.clear();
    m_DosingSrcs.clear();
    for(int i = 10001 ; i <= 10005 ; i++)
    {
        m_PropIds.push_back(i);
        m_PropState.insert(make_pair(i, false));
    }
    m_DosingSrcs.insert(make_pair(1,"Game/Scene_1/fengwei_yumi.png"));
    m_DosingSrcs.insert(make_pair(2,"Game/Scene_1/fengwei_yancong.png"));
    m_DosingSrcs.insert(make_pair(3,"Game/Scene_1/fengwei_heiganlan.png"));
    m_DosingSrcs.insert(make_pair(4,"Game/Scene_1/fengwei_ganlan.png"));
    m_DosingSrcs.insert(make_pair(5,"Game/Scene_1/fengwei_fanqie.png"));
    
}
Interface_Game_Scene_1::~Interface_Game_Scene_1()
{
    
}

Scene * Interface_Game_Scene_1::createScene()
{
    Scene * scene = Scene::create();
    auto mainScene = Interface_Game_Scene_1::create();
    mainScene->setPositionY(__ResolutionPolicy_FIXED_WIDTH);
    scene->addChild(mainScene);
    return scene;
}
/*
 @固定创建方法
 */
bool Interface_Game_Scene_1::init()
{
    if (!Interface_Entity::init())
    {
        return false;
    }
    InitProp();
    return true;
}
/*
 @设置自己的名字
 */
void Interface_Game_Scene_1::SetInterfaceName()
{
    this->setName("Interface_Game_Scene_1");
    CCLOG("Interface_Game_Scene_1::SetInterfaceName()");
}
/*
	@初始化资源
 */
void Interface_Game_Scene_1::InitSrcLayout()
{
    m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
    this->addChild(m_Root);
    //--
    __GetChildAndAssignment(m_LayoutProp, m_Root, "Panel_Prop");
    __GetChildAndAssignment(m_LayoutPropUnder,m_Root,"Panel_PropUnder");
    __GetChildAndAssignment(m_LayoutChoiceDosing,m_Root,"Panel_ChoiceDosing");
    __GetChildAndAssignment(m_LayoutMoveItemEnd,m_LayoutProp,"Panel_MoveItemEnd");
    __GetChildAndAssignmentWithOB(m_ButtonShowDosing, m_Root, "Button_ShowDosing", Button);
    //--
    HideDosingLayout();
    m_ButtonShowDosing->setVisible(false);
    m_RectEndItem.origin = m_LayoutMoveItemEnd->getPosition();
    m_RectEndItem.size = m_LayoutMoveItemEnd->getContentSize() / 2;
}
/*
 @初始化按钮的回调事件
 */
void Interface_Game_Scene_1::InitButtonEvent()
{
    m_ButtonShowDosing->addTouchEventListener(CC_CALLBACK_2(Interface_Game_Scene_1::ShowOrHideDosingEvent, this));
    for (int i = 1; i<= 5; i++) {
        __AddTouchEventWithNameAndPNode(StringUtils::format("Button_Type_%d",i).c_str(), m_LayoutChoiceDosing, Interface_Game_Scene_1::DosingEvent);
    }
     __AddTouchEventWithNameAndPNode("Button_Close", m_LayoutChoiceDosing, Interface_Game_Scene_1::HideDosingEvent);
}
/*
 @初始化道具布置
 */
void Interface_Game_Scene_1::InitProp()
{
    //---
    for(auto & prop_id : m_PropIds)
    {
        auto g_move = Game_MoveItem::CreateWithId(prop_id);
        g_move->RegisterCallback(CallFunc_IntAndNode_selector(Interface_Game_Scene_1::PropStartRunAction), this);
        g_move->BandEndRect(m_RectEndItem.origin, m_RectEndItem.size);
        m_LayoutProp->addChild(g_move,99);
        m_Props.insert(make_pair(prop_id, g_move));
    }
    //---
    OpenNextPropMove();
}
/*
 @道具回调通知进入最终区域
 */
void Interface_Game_Scene_1::PropStartRunAction(int _id,Node * game_move_item)
{
    if(m_PropState.find(_id) != m_PropState.end())
    {
        m_PropState.at(_id) = true;
    }
    //--开启下一个Id道具的移动
    OpenNextPropMove();
}

/*
 @开启下一个道具的移动
 */
void Interface_Game_Scene_1::OpenNextPropMove()
{
    bool is_all_over = true;
    for (auto & prop_s : m_PropState)
    {
        if(prop_s.second == false)
        {
            
            is_all_over = false;
            m_Props.at(prop_s.first)->OpenOrCloseMove(true);
            m_PropState.at(prop_s.first) = true;
            break;
        }
    }
    //--判断都移动完毕没--开启勺子搅拌功能
    if(true == is_all_over)
    {
        
        //...
        OpenMixModel();
    }
}

/*
 @开启搅拌步骤
 */
void Interface_Game_Scene_1::OpenMixModel()
{
    //m_LayoutProp->removeAllChildren();
    //--初始化一个时间范围类道具
    auto move_time = Game_MoveTimeItem::CreateWithId(_Interface_Game_Scene_1_TimePorpId);
    move_time->OpenOrCloseMove(true);
    move_time->RegisterCallback(CallFunc_IntAndNode_selector(Interface_Game_Scene_1::PropMixAction), this);
    move_time->BandEndRect(m_RectEndItem.origin, m_RectEndItem.size);
    m_LayoutProp->addChild(move_time,100);
    //--
    m_ButtonShowDosing->setVisible(true);
    ShowDosingLayout();
    NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10000002,m_Root);
}

/*
    @勺子回调
 */
void Interface_Game_Scene_1::PropMixAction(int value,Node * game_move_item)
{
    //CCLOG("[Interface_Game_Scene_1::PropMixAction] value:%d",value);
    
    if(false == m_IsCanMix)
    {
        m_IsCanMix = true;
        //--初始化一个混合类道具
        m_MixItem = Game_MixItem::CreateWithId(_Interface_Game_Scene_1_MixPorpId);
        m_MixItem->setPosition(m_RectEndItem.origin);
        m_MixItem->RegisterCallback(CallFunc_IntAndNode_selector(Interface_Game_Scene_1::MixEndAction), this);
        m_LayoutProp->addChild(m_MixItem);
        //
        m_ButtonShowDosing->setVisible(false);
        //--
        for (auto & item : m_Props )
        {
            item.second->ClearSomeView();
            item.second->removeFromParent();
        }
        m_Props.clear();
        if(nullptr != m_ImageChoiceDosing)
        {
            m_ImageChoiceDosing->removeFromParent();
            m_ImageChoiceDosing = nullptr;
        }
        
    }else
    {
        m_MixItem->DoMixWithValue(value / 3.0);
    }
}
/*
 @
 */
void Interface_Game_Scene_1::MixEndAction(int value,Node * game_move_item)
{
    //--混合搅拌结束
    CCLOG("[搅拌结束,可以进入下一个场景]");
    //--等几秒进入下一个场景
    NoticeManager::getInstance()->PopPassGameParticle();
    NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10000003,m_Root);
    auto seq = Sequence::create(DelayTime::create(3),CallFunc::create([=](){
        GameManager::getInstance()->StartScene_2();
    }) ,NULL);
    this->runAction(seq);
}
/*
 @展示或隐藏配料选择
 */
void Interface_Game_Scene_1::ShowOrHideDosingEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if (ui::Widget::TouchEventType::BEGAN == eventType)
    {
        ShowDosingLayout();
    }
}
/*
 @隐藏配料选择
 */
void Interface_Game_Scene_1::HideDosingEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if (ui::Widget::TouchEventType::BEGAN == eventType)
    {
        HideDosingLayout();
    }
}
/*
 @配料按钮事件
 */
void Interface_Game_Scene_1::DosingEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if (ui::Widget::TouchEventType::BEGAN == eventType)
    {
        
        HideDosingLayout();
        auto btn = (Button *)pSender;
        m_DosingTag = btn->getTag();
        if (nullptr == m_ImageChoiceDosing)
        {
            m_ImageChoiceDosing = ImageView::create(m_DosingSrcs.at(m_DosingTag));
            m_ImageChoiceDosing->setPosition(_MixDosingVec2);
            m_LayoutProp->addChild(m_ImageChoiceDosing,99);
        }else
        {
            m_ImageChoiceDosing->loadTexture(m_DosingSrcs.at(m_DosingTag));
            m_ImageChoiceDosing->setPosition(_MixDosingVec2);
        }
        GameManager::getInstance()->SetDosingTag(m_DosingTag);
    }
}
/*
 @
 */
void Interface_Game_Scene_1::ShowDosingLayout()
{
    m_LayoutChoiceDosing->setScale(0.1);
    auto s_to = ScaleTo::create(0.5, 1);
    m_LayoutChoiceDosing->setVisible(true);
    m_LayoutChoiceDosing->runAction(s_to);
}
void Interface_Game_Scene_1::HideDosingLayout()
{
    m_LayoutChoiceDosing->setVisible(false);
}

void Interface_Game_Scene_1::onEnter()
{
    Interface_Entity::onEnter();
    NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10000001,m_Root);
}
