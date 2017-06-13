#ifndef __Interface_Game_Scene_2_H__
#define __Interface_Game_Scene_2_H__
#include "Head.h"
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Entity/Interface_Entity.h"
#else
#include "Interface_Entity.h"
#endif//判断路径的引用

class Interface_Game_Scene_2 : public Interface_Entity
{
public:
    /*
     @构造,析构
     */
    Interface_Game_Scene_2();
    ~Interface_Game_Scene_2();
    /*
     @场景建立
     */
    static Scene * createScene();
    virtual bool init();
    CREATE_FUNC(Interface_Game_Scene_2);
    /*
     @控件
     */
    Layout * m_LayoutProp;//道具层
    Layout * m_LayoutButton;
    ImageView * m_PizzaModel;//匹萨模型
    Layout * m_LayoutMoveItemEnd;//终点位置
    Rect m_RectEndItem;
    map<int, string> m_PizzaSrcs;//配料的图片路径
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
        @棒子的回调
     */
    void PropMixAction(int value,Node * game_move_item);
    /*
        @返回上一个场景
    */
    void BackLastLayer(Ref * pSender, ui::Widget::TouchEventType eventType);
    /*
     @
     */
    virtual void onEnter();
};
#endif // !__MAINSCENE__