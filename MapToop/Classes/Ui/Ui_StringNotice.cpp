//
//  Ui_StringNotice.cpp
//  MakePizza
//
//  Created by xcode5 on 17/3/16.
//
//

#include "Ui_StringNotice.h"

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

#define  _Ui_StringNotice_Src "Ui/Ui_StringNotice.csb"
/*
 @构造,析构
 */
Ui_StringNotice::Ui_StringNotice():
m_Content("")
{
    m_LayerSrc = _Ui_StringNotice_Src;
}
Ui_StringNotice::~Ui_StringNotice()
{
    
}


Ui_StringNotice * Ui_StringNotice::CreateWithContentWithTime(const string & content)
{
    auto * ui_s_n = new(std::nothrow) Ui_StringNotice();
    if (ui_s_n)
    {
        ui_s_n->setMyContent(content);
    }
    if (ui_s_n->init())
    {
        ui_s_n->autorelease();
        
        return ui_s_n;
    }
    else
    {
        delete ui_s_n;
        ui_s_n = nullptr;
        return nullptr;
    }
}
/*
 @固定创建方法
 */
bool Ui_StringNotice::init()
{
    if (!Interface_Entity::init())
    {
        return false;
    }
    this->setAnchorPoint(Vec2(0.5,0.5));
    RefreshView();
    return true;
}
/*
 @设置自己的名字
 */
void Ui_StringNotice::SetInterfaceName()
{
    this->setName("Ui_StringNotice");
    CCLOG("Ui_StringNotice::SetInterfaceName()");
}
/*
	@初始化资源
 */
void Ui_StringNotice::InitSrcLayout()
{
    m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
    this->addChild(m_Root);
    this->setContentSize(m_Root->getContentSize());
    //---
    __GetChildAndAssignmentWithOB(m_ImageBackground, m_Root, "Image_Background", ImageView);
    __GetChildAndAssignmentWithOB(m_TextContent, m_Root, "Text_Notice", Text);
}
/*
 @初始化按钮的回调事件
 */
void Ui_StringNotice::InitButtonEvent()
{
    __AddTouchEventWithNameAndPNode("Button_Close", m_Root, Ui_StringNotice::ButtonAboutBaseEvent)
}
/*
 @按钮的回调事件
 */
void Ui_StringNotice::ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if (eventType == ui::Widget::TouchEventType::ENDED)
    {
        this->stopAllActions();
        this->removeFromParentAndCleanup(true);
    }
}

/*
 @刷新
 */
void Ui_StringNotice::RefreshView()
{
    m_TextContent->setString(m_Content);
}
