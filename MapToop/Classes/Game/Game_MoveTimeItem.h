#ifndef __Game_MoveTimeItem_H__
#define __Game_MoveTimeItem_H__
#include "Head.h"
class Controller_Point;

class Game_MoveTimeItem : public Layout
{
public:
    Game_MoveTimeItem();
    ~Game_MoveTimeItem();
    /*
     @固定创建方法
     */
    static Game_MoveTimeItem * CreateWithId(const int &_id);
    virtual bool init();
    CREATE_FUNC(Game_MoveTimeItem);
    /*
     @属性
     */
    CC_SYNTHESIZE(int,m_MyId,MyId); //--Id
    CC_SYNTHESIZE(GameMoveTimeItemData *,m_Data,MyData);
    
    ImageView * m_ViewItem;//主要的显示
    bool m_IsChoice;//是否被选中
    bool m_IsRunEnd;//是否在进行最终动画
    Controller_Point * m_ControllerP;//坐标控制器
    EventListenerTouchOneByOne * m_TouchListner;
    SEL_CallFunc_IntAndNode m_OutFunction;
    Ref * m_OutFunctioner;
    Layout * m_EndLayout;//最终
    Rect m_EndRect;
    ParticleGalaxy * m_ParticleGalaxy;//粒子
    /*
     @初始化显示数据
     */
    void InitView();
    /*
     @移动
     */
    void MoveToPos(const Vec2 & pos);
    
    /*
     @坐标移动事件
     */
    void MovePointEvent(ui::Widget::TouchEventType t_type,Vec2 t_vec);
    
    /*
     @判断是否移动到终点区域
     */
    void IsMoveToEndRect();
    /*
     @开启或关闭移动
     */
    void OpenOrCloseMove(bool _bool);
    /*
     @注册控制器的回调
     @SEL_CallFunc_IntAndNode Id和自身的函数
     @
     */
    void RegisterCallback(SEL_CallFunc_IntAndNode callback, Ref * register_user);
    /*
        @
     */
    virtual void update(float dt);
    /*
     @绑定终点位置的坐标属性
     */
    void BandEndRect(const Vec2 & end_point,const Size & end_size);
    /*
        @根据Type在指定区域中或离开的时候做的一些事
     */
    void DoInEndWithType();
    void DoOutEndWithType();
};

#endif