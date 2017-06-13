#ifndef __Controller_Point_H__
#define __Controller_Point_H__


#include "Config/Head.h"
#include "Entity/Controller_Entity.h"


class Controller_Point : public Controller_Entity
{
public:
    Controller_Point();
    ~Controller_Point();
    /*
		@固定创建方法
     */
    virtual bool init();
    CREATE_FUNC(Controller_Point);
    /*
		 @注册控制器的回调
		 @SEL_CallFunc_SquareEnum 带方向枚举参数的函数
		 @
     */
    void RegisterCallback(SEL_CallFunc_ButtonTypeAndPoint callback, Ref * register_user);
    Point m_PointCur;
    Point m_PointLast;
    ui::Widget::TouchEventType m_CurTouchType;
protected:
    
private:
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent);
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent);
    SEL_CallFunc_ButtonTypeAndPoint m_OutFunction;
    Ref * m_OutFunctioner;
    bool m_IsCanTouch;
    /*
		@根据2个点计算方向然后返回
     */
    void CalculateDirection();
    /*
		@调用外部函数
     */
    void CallOutFunction(ui::Widget::TouchEventType s_type,Vec2 cur_vec2);
};
#endif // !__Controller_Point_H__