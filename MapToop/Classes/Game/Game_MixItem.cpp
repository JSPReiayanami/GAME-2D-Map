//
//  Game_MixItem.cpp
//  MakePizza
//
//  Created by xcode5 on 15/3/16.
//
//

#include "Game_MixItem.h"
//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/DataManager.h"
#include "Tool/StringHelper.h"
#else
#include "DataManager.h"
#include "StringHelper.h"
#endif//判断路径的引用


Game_MixItem::Game_MixItem():
m_MyId(0),
m_Data(nullptr),
m_CurShapeNum(0),
m_IsCanMix(true),
m_OutFunction(nullptr),
m_OutFunctioner(nullptr)
{
    m_ViewSrc.clear();
}
Game_MixItem::~Game_MixItem()
{
    
}
/*
 @固定创建方法
 */
Game_MixItem * Game_MixItem::CreateWithId(const int &_id)
{
    auto *game_move_item = new(std::nothrow) Game_MixItem();
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
bool Game_MixItem::init()
{
    if (!Layout::init())
    {
        return false;
    }
    //---
    this->setAnchorPoint(Vec2(0.5,0.5));
    //---获取完毕数据
    auto g_data = DataManager::getInstance()->GetGameMixItemDataWithId(m_MyId);
    if (nullptr != g_data)
    {
        setMyData(g_data);
        //---解析图片数据
        auto v_data = StringHelper::splitString(m_Data->getMyValue(), ",");
        for (int i = 1;  i <= v_data.size(); i++)
        {
            m_ViewSrc.insert(make_pair(i, v_data.at(i-1)));
        }
    }
    InitView();
    return true;
}

/*
 @初始化显示数据
 */
void Game_MixItem::InitView()
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
        //--
        NextShape();
    }
}
/*
 @切换到下一个形态
 */
void Game_MixItem::NextShape()
{
    m_CurShapeNum++;
    bool is_change = false;
    if (m_ViewSrc.find(m_CurShapeNum) != m_ViewSrc.end())
    {
        is_change = true;
        CCLOG("[Game_MixItem::NextShape][%s]",m_ViewSrc.at(m_CurShapeNum).c_str());
        m_ViewItem->loadTexture(m_ViewSrc.at(m_CurShapeNum));
        this->setContentSize(m_ViewItem->getContentSize());
        m_ViewItem->setPosition(m_ViewItem->getContentSize() / 2);
        m_ViewItem->setTouchEnabled(false);
        //--
        this->setPosition(m_Data->getMySPos());
    }
    //--判断是否有改变
    if(false == is_change)
    {
        OnEndShape();
    }
}
/*
 @变成最终心态时
 */
void Game_MixItem::OnEndShape()
{
    m_IsCanMix = false;
    //--告诉外部
    if(nullptr != m_OutFunctioner && nullptr != m_OutFunction)
    {
        (m_OutFunctioner->*m_OutFunction)(1,this);
    }
}

/*
 @旋转
 */
void Game_MixItem::DoMixWithValue(const float value)
{
    if (m_IsCanMix) {
        auto r_num = m_ViewItem->getRotation() + value;
        if (r_num >=360)
        {
            NextShape();
            m_ViewItem->setRotation(0);
            CCLOG("[Game_MixItem::DoMixWithValue][切换形态]");
        }else
        {
            m_ViewItem->setRotation(r_num);
        }
        
    }
    
}
/*
 @注册控制器的回调
 @SEL_CallFunc_IntAndNode Id和自身的函数
 @
 */
void Game_MixItem::RegisterCallback(SEL_CallFunc_IntAndNode callback, Ref * register_user)
{
    m_OutFunction = callback;
    m_OutFunctioner = register_user;
}



