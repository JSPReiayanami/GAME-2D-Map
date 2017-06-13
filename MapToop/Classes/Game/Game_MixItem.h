#ifndef __Game_MixItem_H__
#define __Game_MixItem_H__
#include "Head.h"
class Controller_Point;

class Game_MixItem : public Layout
{
public:
    Game_MixItem();
    ~Game_MixItem();
    /*
     @固定创建方法
     */
    static Game_MixItem * CreateWithId(const int &_id);
    virtual bool init();
    CREATE_FUNC(Game_MixItem);
    /*
     @属性
     */
    CC_SYNTHESIZE(int,m_MyId,MyId); //--Id
    CC_SYNTHESIZE(GameMixItemData *,m_Data,MyData);
    ImageView * m_ViewItem;//主要的显示
    map<int,string> m_ViewSrc;//所有状态的图片资源
    int m_CurShapeNum;//当前的形态数值
    bool m_IsCanMix;//是否可以旋转
    SEL_CallFunc_IntAndNode m_OutFunction;
    Ref * m_OutFunctioner;
    /*
        @初始化显示数据
    */
    void InitView();
    /*
        @切换到下一个形态
    */
    void NextShape();
    /*
        @变成最终心态时
    */
    void OnEndShape();
    /*
        @旋转
    */
    void DoMixWithValue(const float value);
    /*
     @注册控制器的回调
     @SEL_CallFunc_IntAndNode Id和自身的函数
     @
     */
    void RegisterCallback(SEL_CallFunc_IntAndNode callback, Ref * register_user);
    
    
};

#endif