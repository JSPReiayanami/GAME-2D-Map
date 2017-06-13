//
//  Interface_Game_Scene_5.hpp
//  MakePizza
//
//  Created by xcode5 on 16/3/16.
//
//

#ifndef Interface_Game_Scene_5_hpp
#define Interface_Game_Scene_5_hpp
#include "Head.h"
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Entity/Interface_Entity.h"
#else
#include "Interface_Entity.h"
#endif//判断路径的引用

class Game_MoveItem;
class Game_MixItem;
class Interface_Game_Scene_5 : public Interface_Entity
{
public:
    /*
     @构造,析构
     */
    Interface_Game_Scene_5();
    ~Interface_Game_Scene_5();
    /*
     @场景建立
     */
    static Scene * createScene();
    virtual bool init();
    CREATE_FUNC(Interface_Game_Scene_5);
    /*
     @控件
     */
    Layout * m_LayoutProp;//道具层
    Layout * m_LayoutGroupListView;//
    Layout * m_LayoutButton;
    ImageView * m_ImagePizza;
    ImageView * m_ImageHair;//头发
    ImageView * m_ImageEyes;//眼睛
    ImageView * m_ImageMouth;//嘴巴
    ImageView * m_ImageNose;//鼻子
    vector<int> m_KindId;
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
    
};
#endif /* Interface_Game_Scene_5_hpp */
