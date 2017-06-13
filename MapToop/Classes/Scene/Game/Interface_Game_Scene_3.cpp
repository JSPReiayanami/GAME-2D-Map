//
//  Interface_Game_Scene_3.cpp
//  MakePizza
//
//  Created by xcode5 on 16/3/16.
//
//

#include "Interface_Game_Scene_3.h"
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
#endif//判断路径的引用

#define  _Interface_Game_Scene_3_Src "Interface/Interface_GameScene_3.csb"
#define  _Interface_Game_Scene_3_TimePorpId 20003
#define  _Interface_Game_Scene_3_MixPorpId 30003
/*
 @构造,析构
 */
Interface_Game_Scene_3::Interface_Game_Scene_3():
m_MixItem(nullptr),
m_IsCanMix(false),
m_ImageChoiceDosing(nullptr),
m_DosingTag(1),
m_RectEndItem(0,0,0,0)
{
    m_LayerSrc = _Interface_Game_Scene_3_Src;
    m_PropIds.clear();
    m_PropState.clear();
    m_Props.clear();
    m_DosingSrcs.clear();
    for(int i = 10006 ; i <= 10010 ; i++)
    {
        m_PropIds.push_back(i);
        m_PropState.insert(make_pair(i, false));
    }
    
}
Interface_Game_Scene_3::~Interface_Game_Scene_3()
{
    
}

Scene * Interface_Game_Scene_3::createScene()
{
    Scene * scene = Scene::create();
    auto mainScene = Interface_Game_Scene_3::create();
    mainScene->setPositionY(__ResolutionPolicy_FIXED_WIDTH);
    scene->addChild(mainScene);
    return scene;
}
/*
 @固定创建方法
 */
bool Interface_Game_Scene_3::init()
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
void Interface_Game_Scene_3::SetInterfaceName()
{
    this->setName("Interface_Game_Scene_3");
    CCLOG("Interface_Game_Scene_3::SetInterfaceName()");
}
/*
	@初始化资源
 */
void Interface_Game_Scene_3::InitSrcLayout()
{
    m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
    this->addChild(m_Root);
    //--
    __GetChildAndAssignment(m_LayoutProp, m_Root, "Panel_Prop");
    __GetChildAndAssignment(m_LayoutMoveItemEnd,m_LayoutProp,"Panel_MoveItemEnd");
    __GetChildAndAssignment(m_LayoutButton, m_Root, "Panel_Button");
    __SetAllChildPercentPosition(m_LayoutButton);
    m_RectEndItem.origin = m_LayoutMoveItemEnd->getPosition();
    m_RectEndItem.size = m_LayoutMoveItemEnd->getContentSize() / 2;
}
/*
 @初始化按钮的回调事件
 */
void Interface_Game_Scene_3::InitButtonEvent()
{
    __AddTouchEventWithNameAndPNode("Button_LastLayer", m_LayoutButton, Interface_Game_Scene_3::BackLastLayer);
}
/*
 @初始化道具布置
 */
void Interface_Game_Scene_3::InitProp()
{
    //---
    for(auto & prop_id : m_PropIds)
    {
        auto g_move = Game_MoveItem::CreateWithId(prop_id);
        g_move->RegisterCallback(CallFunc_IntAndNode_selector(Interface_Game_Scene_3::PropStartRunAction), this);
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
void Interface_Game_Scene_3::PropStartRunAction(int _id,Node * game_move_item)
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
void Interface_Game_Scene_3::OpenNextPropMove()
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
void Interface_Game_Scene_3::OpenMixModel()
{
    //m_LayoutProp->removeAllChildren();
    //--初始化一个时间范围类道具
    auto move_time = Game_MoveTimeItem::CreateWithId(_Interface_Game_Scene_3_TimePorpId);
    move_time->OpenOrCloseMove(true);
    move_time->RegisterCallback(CallFunc_IntAndNode_selector(Interface_Game_Scene_3::PropMixAction), this);
    move_time->BandEndRect(m_RectEndItem.origin, m_RectEndItem.size);
    m_LayoutProp->addChild(move_time,100);
}

/*
 @勺子回调
 */
void Interface_Game_Scene_3::PropMixAction(int value,Node * game_move_item)
{
    //CCLOG("[Interface_Game_Scene_1::PropMixAction] value:%d",value);
    
    if(false == m_IsCanMix)
    {
        m_IsCanMix = true;
        //--初始化一个混合类道具
        m_MixItem = Game_MixItem::CreateWithId(_Interface_Game_Scene_3_MixPorpId);
        m_MixItem->setPosition(m_RectEndItem.origin);
        m_MixItem->RegisterCallback(CallFunc_IntAndNode_selector(Interface_Game_Scene_3::MixEndAction), this);
        m_LayoutProp->addChild(m_MixItem);
        //--
        for (auto & item : m_Props )
        {
            item.second->ClearSomeView();
            item.second->removeFromParent();
        }
        m_Props.clear();
    }else
    {
        m_MixItem->DoMixWithValue(value/ 3.0);
    }
}
/*
 @
 */
void Interface_Game_Scene_3::MixEndAction(int value,Node * game_move_item)
{
    //--混合搅拌结束
    CCLOG("[搅拌结束,可以进入下一个场景]");
    //--等几秒进入下一个场景
    NoticeManager::getInstance()->PopPassGameParticle();
    NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10300002,m_Root);
    auto seq = Sequence::create(DelayTime::create(3),CallFunc::create([=](){
        //GameManager::getInstance()->StartScene_4();
    }) ,NULL);
    this->runAction(seq);
}

/*
 @返回上一个场景
 */
void Interface_Game_Scene_3::BackLastLayer(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if(ui::Widget::TouchEventType::ENDED == eventType)
    {
        GameManager::getInstance()->StartScene_2();
    }
}

void Interface_Game_Scene_3::onEnter()
{
    Interface_Entity::onEnter();
    NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10300001,m_Root);
}

