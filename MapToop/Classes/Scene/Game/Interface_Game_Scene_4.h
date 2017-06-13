//
//  Interface_Game_Scene_4.hpp
//  MakePizza
//
//  Created by xcode5 on 16/3/16.
//
//

#ifndef Interface_Game_Scene_4_hpp
#define Interface_Game_Scene_4_hpp

#include "Head.h"
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Entity/Interface_Entity.h"
#else
#include "Interface_Entity.h"
#endif//判断路径的引用

class Game_MoveItem;
class Game_MixItem;
class Game_MoveTimeItem;
class Interface_Game_Scene_4 : public Interface_Entity
{
public:
    /*
     @构造,析构
     */
    Interface_Game_Scene_4();
    ~Interface_Game_Scene_4();
    /*
     @场景建立
     */
    static Scene * createScene();
    virtual bool init();
    CREATE_FUNC(Interface_Game_Scene_4);
    /*
     @控件
     */
    Layout * m_LayoutProp;//道具层
    Layout * m_LayoutButton;
    Game_MixItem * m_MixItem;//融合的道具
    Button * m_ButtonShowDosing;//展示配料的按钮
    ImageView * m_ImageChoiceDosing;//配料的选择
    Layout * m_LayoutMoveItemEnd;//终点位置
    vector<int> m_PropIds;
    map<int,bool> m_PropState;//道具的状态
    map<int, Game_MoveItem *> m_Props;//道具
    map<int, string> m_DosingSrcs;//配料的图片路径
    bool m_IsCanMix;//是否可以开始搅拌
    int m_DosingTag;//配料的标记
    Rect m_RectEndItem;
    ImageView * m_ImageCake;//面饼
    ImageView * m_ImageSoup;//汤汁
    ImageView * m_ImageCheese;//起司
    Game_MoveTimeItem * m_ItemSoup;//汤汁移动道具
    Game_MoveTimeItem * m_ItemCheese;//起司移动道具
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
        @汤汁回调
     */
    void ItemSoupAction(int value,Node * game_move_item);
    /*
        @起司回调
     */
    void ItemCheeseAction(int value,Node * game_move_item);
     /*
    @返回上一个场景
     */
    void BackLastLayer(Ref * pSender, ui::Widget::TouchEventType eventType);
    /*
        @
     */
    void DoSoupAction(const int & value);
    /*
     @
     */
    void DoCheeseAction(const int & value);
    /*
     @
     */
    virtual void onEnter();
    
};

#endif /* Interface_Game_Scene_4_hpp */
