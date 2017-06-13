//
//  Interface_Game_Scene_2.cpp
//  MakePizza
//
//  Created by xcode5 on 15/3/16.
//
//

#include "Interface_Game_Scene_2.h"

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

#define  _Interface_Game_Scene_2_Src "Interface/Interface_GameScene_2.csb"
#define  _Interface_Game_Scene_2_TimePorpId 20002
/*
 @构造,析构
 */
Interface_Game_Scene_2::Interface_Game_Scene_2()
{
    m_LayerSrc = _Interface_Game_Scene_2_Src;
    m_PizzaSrcs.insert(make_pair(0,"Game/Scene_2/mianpi.png"));
    m_PizzaSrcs.insert(make_pair(1,"Game/Scene_2/yumimianpi.png"));
    m_PizzaSrcs.insert(make_pair(2,"Game/Scene_2/yangcongmianpi.png"));
    m_PizzaSrcs.insert(make_pair(3,"Game/Scene_2/heiganlanmianpi.png"));
    m_PizzaSrcs.insert(make_pair(4,"Game/Scene_2/ganlanmianpi.png"));
    m_PizzaSrcs.insert(make_pair(5,"Game/Scene_2/fanqiemianpi.png"));
}
Interface_Game_Scene_2::~Interface_Game_Scene_2()
{
    
}

Scene * Interface_Game_Scene_2::createScene()
{
    Scene * scene = Scene::create();
    auto mainScene = Interface_Game_Scene_2::create();
    mainScene->setPositionY(__ResolutionPolicy_FIXED_WIDTH);
    scene->addChild(mainScene);
    return scene;
}
/*
 @固定创建方法
 */
bool Interface_Game_Scene_2::init()
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
void Interface_Game_Scene_2::SetInterfaceName()
{
    this->setName("Interface_Game_Scene_2");
    CCLOG("Interface_Game_Scene_2::SetInterfaceName()");
}
/*
	@初始化资源
 */
void Interface_Game_Scene_2::InitSrcLayout()
{
    m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
    this->addChild(m_Root);
    //--
    __GetChildAndAssignment(m_LayoutProp, m_Root, "Panel_Prop");
    __GetChildAndAssignment(m_LayoutButton, m_Root, "Panel_Button");
    __SetAllChildPercentPosition(m_LayoutButton);
    __GetChildAndAssignmentWithOB(m_PizzaModel, m_LayoutProp,"Image_Cake",ImageView)
    __GetChildAndAssignment(m_LayoutMoveItemEnd,m_Root,"Panel_EndItem");
    m_RectEndItem.origin = m_LayoutMoveItemEnd->getPosition();
    m_RectEndItem.size = m_LayoutMoveItemEnd->getContentSize();
    
}
/*
 @初始化按钮的回调事件
 */
void Interface_Game_Scene_2::InitButtonEvent()
{
    __AddTouchEventWithNameAndPNode("Button_LastLayer", m_LayoutButton, Interface_Game_Scene_2::BackLastLayer);
}
/*
 @初始化道具布置
 */
void Interface_Game_Scene_2::InitProp()
{
    auto g_t_item = Game_MoveTimeItem::CreateWithId(_Interface_Game_Scene_2_TimePorpId);
    g_t_item->OpenOrCloseMove(true);
    g_t_item->RegisterCallback(CallFunc_IntAndNode_selector(Interface_Game_Scene_2::PropMixAction), this);
    g_t_item->BandEndRect(m_RectEndItem.origin, m_RectEndItem.size);
    this->addChild(g_t_item,10);
    //--模型的初始化
    m_PizzaModel->loadTexture(m_PizzaSrcs.at(GameManager::getInstance()->GetDosingTag()));
}
/*
 @棒子的回调
 */
void Interface_Game_Scene_2::PropMixAction(int value,Node * game_move_item)
{
    if (m_LayoutProp->getScaleY() < 1)
    {
        auto s_y = m_LayoutProp->getScaleY()+value/200.0;
        if (s_y > 1)
        {
            s_y = 1;
        }
        m_LayoutProp->setScaleY(s_y);
    }else if(m_LayoutProp->getScaleX() < 1)
    {
        auto s_x = m_LayoutProp->getScaleX()+value/200.0;
        if (s_x > 1)
        {
            s_x = 1;
            //--缩放完毕--进入下个场景
            NoticeManager::getInstance()->PopPassGameParticle();
            NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10200002,m_Root);
            //--等几秒进入下一个场景
            auto seq = Sequence::create(DelayTime::create(3),CallFunc::create([=](){
                GameManager::getInstance()->StartScene_3();
            }) ,NULL);
            this->runAction(seq);
        }
        m_LayoutProp->setScaleX(s_x);
    }
    
    
}

/*
 @返回上一个场景
 */
void Interface_Game_Scene_2::BackLastLayer(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if(ui::Widget::TouchEventType::ENDED == eventType)
    {
        GameManager::getInstance()->StartScene_1();
    }
}
void Interface_Game_Scene_2::onEnter()
{
    Interface_Entity::onEnter();
    NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10200001,m_Root);
}

