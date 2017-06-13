//
//  Interface_Game_Scene_7.hpp
//  MakePizza
//
//  Created by xcode5 on 17/3/16.
//
//

#ifndef Interface_Game_Scene_7_hpp
#define Interface_Game_Scene_7_hpp
#include "Head.h"
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Entity/Interface_Entity.h"
#else
#include "Interface_Entity.h"
#endif//判断路径的引用

class Game_MoveItem;
class Game_MixItem;
class Interface_Game_Scene_7 : public Interface_Entity
{
public:
    /*
     @构造,析构
     */
    Interface_Game_Scene_7();
    ~Interface_Game_Scene_7();
    /*
     @场景建立
     */
    static Scene * createScene();
    virtual bool init();
    CREATE_FUNC(Interface_Game_Scene_7);
    /*
     @控件
     */
    Slider * m_SliderBurnTime;//时间条
    Button * m_ButtonShovel;//铁铲子按钮
    ImageView * m_ImageCake;//那张饼
    Vec2  m_Vec2CakePoint;//铁铲子中披萨的位置
    Vec2  m_Vec2ShovelPoint;//铲子的位置
    Vec2  m_Vec2ShoveBurnPoint;//铲子被烤的位置
    float m_BurnTime;
    bool  m_IsBurnModel;//是否是烧烤模式
    /*
     @设置自己的名字
     */
    void SetInterfaceName();
    /*
     @初始化资源
     */
    void InitSrcLayout();
    /*
     @初始化按钮的回调事件
     */
    void InitButtonEvent();
    /*
     @初始化道具布置
     */
    void InitProp();
    /*
     @返回上一个场景
     */
    void BackLastLayer(Ref * pSender, ui::Widget::TouchEventType eventType);
    /*
        @铁铲子点击事件
     */
    void TouchShoveEvetn(Ref * pSender, ui::Widget::TouchEventType eventType);
    /*
        @开启烧烤模式
     */
    void OpenBurnModel();
    /*
        @
     */
    virtual void update(float dt);
    /*
        @烤制失败
     */
    void BurnIsFailure();
    /*
     @
     */
    virtual void onEnter();
    
};
#endif /* Interface_Game_Scene_7_hpp */
