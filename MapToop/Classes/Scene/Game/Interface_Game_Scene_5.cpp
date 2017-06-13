//
//  Interface_Game_Scene_5.cpp
//  MakePizza
//
//  Created by xcode5 on 16/3/16.
//
//

#include "Interface_Game_Scene_5.h"
//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataManager.h"
#include "Manager/SoundManager.h"
#include "Ui/Ui_GroupListView.h"
#else
#include "SceneManager.h"
#include "GameManager.h"
#include "DataManager.h"
#include "SoundManager.h"
#include "Ui_GroupListView.h"
#endif//判断路径的引用

#define  _Interface_Game_Scene_5_Src "Interface/Interface_GameScene_5.csb"
#define  _Interface_Game_Scene_3_TimePorpId 20003
#define  _Interface_Game_Scene_3_MixPorpId 30003
/*
 @构造,析构
 */
Interface_Game_Scene_5::Interface_Game_Scene_5()
{
    m_LayerSrc = _Interface_Game_Scene_5_Src;
    m_KindId.clear();
    for (int i = 40001; i<= 40004; i++) {
        m_KindId.push_back(i);
    }
}
Interface_Game_Scene_5::~Interface_Game_Scene_5()
{
    
}

Scene * Interface_Game_Scene_5::createScene()
{
    Scene * scene = Scene::create();
    auto mainScene = Interface_Game_Scene_5::create();
    mainScene->setPositionY(__ResolutionPolicy_FIXED_WIDTH);
    scene->addChild(mainScene);
    return scene;
}
/*
 @固定创建方法
 */
bool Interface_Game_Scene_5::init()
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
void Interface_Game_Scene_5::SetInterfaceName()
{
    this->setName("Interface_Game_Scene_5");
    CCLOG("Interface_Game_Scene_5::SetInterfaceName()");
}
/*
	@初始化资源
 */
void Interface_Game_Scene_5::InitSrcLayout()
{
    m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
    this->addChild(m_Root);
    //--
    __GetChildAndAssignment(m_LayoutProp, m_Root, "Panel_Prop");
    __GetChildAndAssignment(m_LayoutGroupListView, m_Root, "Panel_GroupListView");
    __GetChildAndAssignmentWithOB(m_ImagePizza, m_LayoutProp, "Image_Model", ImageView);
    __GetChildAndAssignmentWithOB(m_ImageHair, m_LayoutProp, "Image_Hair", ImageView);
    __GetChildAndAssignmentWithOB(m_ImageEyes, m_LayoutProp, "Image_Eyes", ImageView);
    __GetChildAndAssignmentWithOB(m_ImageNose, m_LayoutProp, "Image_Nose", ImageView);
    __GetChildAndAssignmentWithOB(m_ImageMouth, m_LayoutProp, "Image_Mouth", ImageView);
    __GetChildAndAssignment(m_LayoutButton,m_Root, "Panel_Button");
    //__SetAllChildPercentPosition(m_LayoutButton);

}
/*
 @初始化按钮的回调事件
 */
void Interface_Game_Scene_5::InitButtonEvent()
{
    __AddTouchEventWithNameAndPNode("Button_LastLayer", m_LayoutButton, Interface_Game_Scene_5::BackLastLayer);
}
/*
 @初始化道具布置
 */
void Interface_Game_Scene_5::InitProp()
{
    
    auto ui_g = Ui_GroupListView::CreateWithId(m_KindId);
    //--注册一些点击回调
    m_LayoutGroupListView->addChild(ui_g);
}


/*
 @返回上一个场景
 */
void Interface_Game_Scene_5::BackLastLayer(Ref * pSender, ui::Widget::TouchEventType eventType)
{
    if(ui::Widget::TouchEventType::ENDED == eventType)
    {
        GameManager::getInstance()->StartScene_4();
    }
}

