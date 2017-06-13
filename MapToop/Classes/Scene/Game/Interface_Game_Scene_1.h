#ifndef __Interface_Game_Scene_1_H__
#define __Interface_Game_Scene_1_H__
#include "Head.h"
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Entity/Interface_Entity.h"
#else
#include "Interface_Entity.h"
#endif//判断路径的引用

class Game_MoveItem;
class Game_MixItem;
class Interface_Game_Scene_1 : public Interface_Entity
{
public:
    /*
     @构造,析构
     */
    Interface_Game_Scene_1();
    ~Interface_Game_Scene_1();
    /*
     @场景建立
     */
    static Scene * createScene();
    virtual bool init();
    CREATE_FUNC(Interface_Game_Scene_1);
    /*
        @控件
     */
    Layout * m_LayoutProp;//道具层
    Layout * m_LayoutPropUnder;//道具层
    Layout * m_LayoutChoiceDosing;//配料选择层
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
        @道具回调通知进入最终区域
     */
    void PropStartRunAction(int _id,Node * game_move_item);
    /*
        @开启下一个道具的移动
     */
    void OpenNextPropMove();
    /*
        @开启搅拌步骤
     */
    void OpenMixModel();
    /*
        @勺子回调
     */
    void PropMixAction(int value,Node * game_move_item);
    /*
        @
    */
    void MixEndAction(int value,Node * game_move_item);
    /*
        @展示或隐藏配料选择
    */
    void ShowOrHideDosingEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
    /*
        @隐藏配料选择
     */
    void HideDosingEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
    /*
        @配料按钮事件
     */
    void DosingEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
    /*
        @
    */
    void ShowDosingLayout();
    void HideDosingLayout();
    
    /*
        @
     */
    virtual void onEnter();
};
#endif // !__MAINSCENE__