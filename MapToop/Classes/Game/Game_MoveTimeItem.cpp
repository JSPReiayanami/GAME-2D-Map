//
//  Game_MoveTimeItem.cpp
//  MakePizza
//
//  Created by xcode5 on 14/3/16.
//
//

#include "Game_MoveTimeItem.h"
//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/DataManager.h"
#include "Controller/Controller_Point.h"
#else
#include "DataManager.h"
#include "Controller_Point.h"
#endif//判断路径的引用


Game_MoveTimeItem::Game_MoveTimeItem():
m_MyId(0),
m_Data(nullptr),
m_IsChoice(false),
m_IsRunEnd(false),
m_OutFunctioner(nullptr),
m_OutFunction(nullptr),
m_EndRect(0,0,0,0),
m_ParticleGalaxy(nullptr)
{
    
}
Game_MoveTimeItem::~Game_MoveTimeItem()
{
    
}
/*
 @固定创建方法
 */
Game_MoveTimeItem * Game_MoveTimeItem::CreateWithId(const int &_id)
{
    auto *game_move_item = new(std::nothrow) Game_MoveTimeItem();
    if (game_move_item)
    {
        game_move_item->setMyId(_id);
        
    }
    if (game_move_item->init())
    {
        game_move_item->autorelease();
        return game_move_item;
    }
    else
    {
        delete game_move_item;
        game_move_item = nullptr;
        return nullptr;
    }
}
bool Game_MoveTimeItem::init()
{
    if (!Layout::init())
    {
        return false;
    }
    m_ControllerP = Controller_Point::create();
    m_ControllerP->RegisterCallback(CallFunc_ButtonTypeAndPoint_selector(Game_MoveTimeItem::MovePointEvent), this);
    addChild(m_ControllerP);
    //---
    this->setAnchorPoint(Vec2(0.5,0.5));
    //---获取完毕数据
    auto g_move_data = DataManager::getInstance()->GetMoveItemTimeDataWithId(m_MyId);
    if (nullptr != g_move_data)
    {
        setMyData(g_move_data);
    }
    InitView();
    return true;
}

/*
 @初始化显示数据
 */
void Game_MoveTimeItem::InitView()
{
    if (nullptr != m_Data)
    {
        m_ViewItem = ImageView::create(m_Data->getMySrc());
        this->setContentSize(m_ViewItem->getContentSize());
        m_ViewItem->setPosition(m_ViewItem->getContentSize() / 2);
        m_ViewItem->setTouchEnabled(false);
        this->addChild(m_ViewItem);
        //--
        this->setPosition(m_Data->getMySPos());
        this->scheduleUpdate();
    }
}
/*
 @移动
 */
void Game_MoveTimeItem::MoveToPos(const Vec2 & pos)
{
    this->setPosition(pos);
}

/*
 @坐标移动事件
 */
void Game_MoveTimeItem::MovePointEvent(ui::Widget::TouchEventType t_type,Vec2 t_vec)
{
    if( ui::Widget::TouchEventType::BEGAN == t_type)
    {
        
        CCLOG("[%s] 移动开始 [x:%0.2f y:%0.2f]",m_Data->getMyName().c_str(),t_vec.x,t_vec.y);
        //--判断是否点击到自己
        auto this_pos = this->getPosition();
        auto this_content = this->getContentSize();
        CCLOG("[posx:%0.2f posy:%0.2f]",this_pos.x,this_pos.y);
        CCLOG("[content:%0.2f posy:%0.2f]",this_content.width,this_content.height);
        if(
           t_vec.x >= (this_pos.x - this_content.width/2.0) &&
           t_vec.x <= (this_pos.x + this_content.width/2.0) &&
           (t_vec.y - __ResolutionPolicy_FIXED_WIDTH ) >= (this_pos.y - this_content.height/2.0)&&
           (t_vec.y + __ResolutionPolicy_FIXED_WIDTH )<= (this_pos.y + this_content.height/2.0)
           )
        {
            m_IsChoice = true;
        }
    }else if (ui::Widget::TouchEventType::MOVED == t_type)
    {
        if (true == m_IsChoice) {
            this->MoveToPos(Vec2(t_vec.x,t_vec.y - __ResolutionPolicy_FIXED_WIDTH));
            IsMoveToEndRect();
        }
        
    }else if (ui::Widget::TouchEventType::ENDED == t_type)
    {
        
        m_IsChoice = false;
        m_IsRunEnd = false;
        //CCLOG("[%s] 移动结束 [x:%0.2f y:%0.2f]",m_Data->getMyName().c_str(),t_vec.x,t_vec.y);
        if ( false == m_IsRunEnd)
        {
            //--回到原位
            this->setPosition(m_Data->getMySPos());
        }
    }
}

/*
 @判断是否移动到终点区域
 */
void Game_MoveTimeItem::IsMoveToEndRect()
{
    
    auto end_rect = m_EndRect;
    auto pos = this->getPosition();
    auto this_content = this->getContentSize();
    pos.x += ((m_Data->getMyPointRect().origin.x / 100.0) * this_content.width) - this_content.width / 2;
    pos.y += ((m_Data->getMyPointRect().origin.y / 100.0) * this_content.height) - this_content.height / 2;
    if(
       pos.x >= (end_rect.origin.x - end_rect.size.width) &&
       pos.x <= (end_rect.origin.x + end_rect.size.width) &&
       pos.y >= (end_rect.origin.y - end_rect.size.height) &&
       pos.y <= (end_rect.origin.y + end_rect.size.height)
       )
    {
        if (false == m_IsRunEnd) {
            
        }
        m_IsRunEnd = true;
        //CCLOG("[Game_MoveItem::IsMoveToEndRect]");
        
    }else{
        m_IsRunEnd = false;
    }
}
/*
 @开启或关闭移动
 */
void Game_MoveTimeItem::OpenOrCloseMove(bool _bool)
{
    if(true == _bool)
    {
        m_ControllerP->OpenTouch();
    }else if (false == _bool)
    {
        //--回到原位
        this->setPosition(m_Data->getMySPos());
        m_ControllerP->CloseTouch();
    }
}

/*
 @注册控制器的回调
 @SEL_CallFunc_IntAndNode Id和自身的函数
 @
 */
void Game_MoveTimeItem::RegisterCallback(SEL_CallFunc_IntAndNode callback, Ref * register_user)
{
    m_OutFunction = callback;
    m_OutFunctioner = register_user;
}

/*
 @
 */
void Game_MoveTimeItem::update(float dt)
{
    if(true == m_IsRunEnd)
    {
        if(nullptr != m_OutFunctioner && nullptr != m_OutFunction)
        {
            (m_OutFunctioner->*m_OutFunction)(atoi(m_Data->getMyValue().c_str()),this);
        }
    }
}
/*
 @绑定终点位置的坐标属性
 */
void Game_MoveTimeItem::BandEndRect(const Vec2 & end_point,const Size & end_size)
{
    m_EndRect.origin = end_point;
    m_EndRect.size = end_size;
}
/*
 @根据Type在指定区域中或离开的时候做的一些事
 */
void Game_MoveTimeItem::DoInEndWithType()
{
    switch ((int)m_Data->getMyType())
    {
        case 1:
        {
            if (nullptr == m_ParticleGalaxy) {
                m_ParticleGalaxy = ParticleGalaxy::create();
                m_ParticleGalaxy->setTexture(Director::getInstance()->getTextureCache()->addImage(m_Data->getMySrc()));
                m_ParticleGalaxy->setPosition(Vec2::ZERO);
                this->addChild(m_ParticleGalaxy);
            }else{
                m_ParticleGalaxy->setVisible(true);
            }
            break;
        }
        case 2:
        {
            m_ViewItem->setRotation(270);
            break;
        }
        default:
            break;
    }
}
void Game_MoveTimeItem::DoOutEndWithType()
{
    switch (m_Data->getMyType())
    {
        case 1:
        {
            m_ParticleGalaxy->setVisible(false);
            break;
        }
        case 2:
        {
            m_ViewItem->setRotation(0);
            break;
        }
        default:
            break;
    }
}

