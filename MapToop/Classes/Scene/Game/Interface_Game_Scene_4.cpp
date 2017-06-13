//
//  Interface_Game_Scene_4.cpp
//  MakePizza
//
//  Created by xcode5 on 16/3/16.
//
//

#include "Interface_Game_Scene_4.h"
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

#define  _Interface_Game_Scene_4_Src "Interface/Interface_GameScene_4.csb"
#define  _Interface_Game_Scene_4_TimePorpId_1 20004
#define  _Interface_Game_Scene_4_TimePorpId_2 20005
/*
 @构造,析构
 */
Interface_Game_Scene_4::Interface_Game_Scene_4():
m_MixItem(nullptr),
m_IsCanMix(false),
m_ImageChoiceDosing(nullptr),
m_DosingTag(1),
m_RectEndItem(0,0,0,0),
m_ItemSoup(nullptr),
m_ItemCheese(nullptr)
{
    m_LayerSrc = _Interface_Game_Scene_4_Src;
    m_PropIds.clear();
    m_PropState.clear();
    m_Props.clear();
    m_DosingSrcs.clear();
    
}
Interface_Game_Scene_4::~Interface_Game_Scene_4()
{
    
}

Scene * Interface_Game_Scene_4::createScene()
{
    Scene * scene = Scene::create();
    auto mainScene = Interface_Game_Scene_4::create();
    mainScene->setPositionY(__ResolutionPolicy_FIXED_WIDTH);
    scene->addChild(mainScene);
    return scene;
}
/*
 @固定创建方法
 */
bool Interface_Game_Scene_4::init()
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
void Interface_Game_Scene_4::SetInterfaceName()
{
    this->setName("Interface_Game_Scene_4");
    CCLOG("Interface_Game_Scene_4::SetInterfaceName()");
}
/*
	@初始化资源
 */
void Interface_Game_Scene_4::InitSrcLayout()
{
    m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
    this->addChild(m_Root);
    //--
    __GetChildAndAssignment(m_LayoutProp, m_Root, "Panel_Prop");
    __GetChildAndAssignment(m_LayoutMoveItemEnd,m_LayoutProp,"Panel_MoveItemEnd");
    __GetChildAndAssignmentWithOB(m_ImageCake,m_LayoutProp, "Image_Cake", ImageView);
    __GetChildAndAssignmentWithOB(m_ImageSoup,m_LayoutProp, "Image_Soup", ImageView);
    __GetChildAndAssignmentWithOB(m_ImageCheese,m_LayoutProp, "Image_Cheese", ImageView);
    m_RectEndItem.origin = m_LayoutMoveItemEnd->getPosition();
    m_RectEndItem.size = m_LayoutMoveItemEnd->getContentSize() / 2;
    __GetChildAndAssignment(m_LayoutButton, m_Root, "Panel_Button");
    __SetAllChildPercentPosition(m_LayoutButton);
}
/*
 @初始化按钮的回调事件
 */
void Interface_Game_Scene_4::InitButtonEvent()
{
    __AddTouchEventWithNameAndPNode("Button_LastLayer", m_LayoutButton, Interface_Game_Scene_4::BackLastLayer);
}
/*
 @初始化道具布置
 */
void Interface_Game_Scene_4::InitProp()
{
    //---布置一个汤汁
    m_ItemSoup = Game_MoveTimeItem::CreateWithId(_Interface_Game_Scene_4_TimePorpId_1);
    m_ItemSoup->OpenOrCloseMove(true);
    m_ItemSoup->RegisterCallback(CallFunc_IntAndNode_selector(Interface_Game_Scene_4::ItemSoupAction), this);
    m_ItemSoup->BandEndRect(m_RectEndItem.origin, m_RectEndItem.size);
    m_LayoutProp->addChild(m_ItemSoup);
    
}

/*
 @勺子回调
 */
void Interface_Game_Scene_4::ItemSoupAction(int value,Node * game_move_item)
{
    //CCLOG("[Interface_Game_Scene_1::PropMixAction] value:%d",value);
    auto s_scale = m_ImageSoup->getScale();
    if (s_scale <= 1)
    {
        m_ImageSoup->setVisible(true);
        m_ImageSoup->setScale(s_scale + value / 300.0);
    }else{
        m_ImageSoup->setScale(1);
        m_ItemSoup->removeFromParent();
        m_ItemSoup = nullptr;
        //--开始弄起司
        NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10400002,m_Root);
        //---布置一个起司
        m_ItemCheese = Game_MoveTimeItem::CreateWithId(_Interface_Game_Scene_4_TimePorpId_2);
        m_ItemCheese->OpenOrCloseMove(true);
        m_ItemCheese->RegisterCallback(CallFunc_IntAndNode_selector(Interface_Game_Scene_4::ItemCheeseAction), this);
        m_ItemCheese->BandEndRect(m_RectEndItem.origin, m_RectEndItem.size);
        m_LayoutProp->addChild(m_ItemCheese);
    }
}
/*
 @
 */
void Interface_Game_Scene_4::ItemCheeseAction(int value,Node * game_move_item)
{
    auto o_op = m_ImageCheese->getOpacity();
    if (o_op < 250) {
        o_op += value;
        if(o_op > 250){
            o_op = 250;
        }
        CCLOG("[Interface_Game_Scene_4][op:%d]",o_op);
        m_ImageCheese->setOpacity(o_op);
    }else
    {
        NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10400003,m_Root);
        //--进入下一关
        GameManager::getInstance()->StartScene_5();
        m_ItemCheese->removeFromParent();
        m_ItemCheese = nullptr;
    }
}

/*
 @返回上一个场景
 */
void Interface_Game_Scene_4::BackLastLayer(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if(ui::Widget::TouchEventType::ENDED == eventType)
    {
        GameManager::getInstance()->StartScene_3();
    }
}

void Interface_Game_Scene_4::onEnter()
{
    Interface_Entity::onEnter();
    NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10400001,m_Root);
}
/*
 @
 */
void Interface_Game_Scene_4::DoSoupAction(const int & value)
{
    
}
/*
 @
 */
void Interface_Game_Scene_4::DoCheeseAction(const int & value)
{
    
}
