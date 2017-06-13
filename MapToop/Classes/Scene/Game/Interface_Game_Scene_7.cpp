//
//  Interface_Game_Scene_7.cpp
//  MakePizza
//
//  Created by xcode5 on 17/3/16.
//
//

#include "Interface_Game_Scene_7.h"

//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataManager.h"
#include "Manager/SoundManager.h"
#include "Manager/NoticeManager.h"
#else
#include "SceneManager.h"
#include "GameManager.h"
#include "DataManager.h"
#include "SoundManager.h"
#include "NoticeManager.h"
#endif//判断路径的引用

#define  _Interface_Game_Scene_7_Src "Interface/Interface_GameScene_7.csb"
#define  _Burn_Good_1 30
#define  _Burn_Good_2 60
#define  _BurnTime_Scale 10
/*
 @构造,析构
 */
Interface_Game_Scene_7::Interface_Game_Scene_7():
m_Vec2CakePoint(Vec2::ZERO),
m_Vec2ShovelPoint(Vec2::ZERO),
m_Vec2ShoveBurnPoint(Vec2(__TransformPercentWidthToViewSize(59),__TransformPercentHeightToViewSize(26.6)))
{
    m_LayerSrc = _Interface_Game_Scene_7_Src;
}
Interface_Game_Scene_7::~Interface_Game_Scene_7()
{
    
}

Scene * Interface_Game_Scene_7::createScene()
{
    Scene * scene = Scene::create();
    auto mainScene = Interface_Game_Scene_7::create();
    mainScene->setPositionY(__ResolutionPolicy_FIXED_WIDTH);
    scene->addChild(mainScene);
    return scene;
}
/*
 @固定创建方法
 */
bool Interface_Game_Scene_7::init()
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
void Interface_Game_Scene_7::SetInterfaceName()
{
    this->setName("Interface_Game_Scene_7");
    CCLOG("Interface_Game_Scene_7::SetInterfaceName()");
}
/*
	@初始化资源
 */
void Interface_Game_Scene_7::InitSrcLayout()
{
    m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
    this->addChild(m_Root);
    //--
    __GetChildAndAssignmentWithOB(m_SliderBurnTime, m_Root, "Slider_BurnTime", Slider);
    __GetChildAndAssignmentWithOB(m_ButtonShovel,m_Root, "Button_Shovel", Button);
    __GetChildAndAssignmentWithOB(m_ImageCake,m_Root, "Image_Cake", ImageView);
    m_Vec2CakePoint = m_ImageCake->getPosition();
    m_Vec2ShovelPoint = m_ButtonShovel->getPosition();
    
}
/*
 @初始化按钮的回调事件
 */
void Interface_Game_Scene_7::InitButtonEvent()
{
    __AddTouchEventWithNameAndPNode("Button_LastLayer", m_Root, Interface_Game_Scene_7::BackLastLayer);
    __AddTouchEventWithNameAndPNode("Button_Shovel", m_Root, Interface_Game_Scene_7::TouchShoveEvetn);
}
/*
 @初始化道具布置
 */
void Interface_Game_Scene_7::InitProp()
{
    m_BurnTime = 0;
    m_IsBurnModel = false;
    m_SliderBurnTime->setPercent(0);
    m_ButtonShovel->setPosition(m_Vec2ShovelPoint);
    m_ButtonShovel->setTouchEnabled(true);
}


/*
 @返回上一个场景
 */
void Interface_Game_Scene_7::BackLastLayer(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if(ui::Widget::TouchEventType::ENDED == eventType)
    {
        GameManager::getInstance()->StartScene_6();
    }
}
/*
 @铁铲子点击事件
 */
void Interface_Game_Scene_7::TouchShoveEvetn(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if(ui::Widget::TouchEventType::BEGAN == eventType)
    {
        if (false == m_IsBurnModel)
        {
            auto move_to = MoveTo::create(1, m_Vec2ShoveBurnPoint);
            auto seq = Sequence::create(move_to,CallFunc::create([=](){
                //--移动完毕开启烧烤模式
                OpenBurnModel();
                m_ButtonShovel->setTouchEnabled(true);
            }), NULL);
            m_ButtonShovel->runAction(seq);
            m_ButtonShovel->setTouchEnabled(false);
            m_IsBurnModel = true;
        }else
        {
            //--烧烤完毕
            auto cur_p = m_SliderBurnTime->getPercent();
            if (cur_p >= _Burn_Good_1 && cur_p <= _Burn_Good_2)
            {
                //--烧烤成功
                NoticeManager::getInstance()->PopPassGameParticle();
                NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10700002,m_Root);
                m_ButtonShovel->setPosition(m_Vec2ShovelPoint);
                this->unscheduleUpdate();
                //--弹出接下来3个场景的按钮
            }else
            {
                //--烤制失败
                BurnIsFailure();
                NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10700003,m_Root);
            }
        }
        
    }
}
/*
 @开启烧烤模式
 */
void Interface_Game_Scene_7::OpenBurnModel()
{
    this->scheduleUpdate();
}
/*
 @
 */
void Interface_Game_Scene_7::update(float dt)
{
    m_BurnTime += dt*_BurnTime_Scale;
    if (m_BurnTime >= 100)
    {
        //--失败
        BurnIsFailure();
    }
    m_SliderBurnTime->setPercent(m_BurnTime);
}
/*
 @烤制失败
 */
void Interface_Game_Scene_7::BurnIsFailure()
{
    this->unscheduleUpdate();
    InitProp();
}
void Interface_Game_Scene_7::onEnter()
{
    Interface_Entity::onEnter();
    NoticeManager::getInstance()->PopStringNoticeAndWillRemoveWithId(10700001,m_Root);
}
