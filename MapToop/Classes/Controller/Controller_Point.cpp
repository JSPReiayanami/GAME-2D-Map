
#include "Controller_Point.h"

Controller_Point::Controller_Point() :
m_OutFunction(nullptr),
m_OutFunctioner(nullptr),
m_PointCur(0, 0),
m_PointLast(0,0),
m_IsCanTouch(true),
m_CurTouchType(ui::Widget::TouchEventType::BEGAN)
{
    m_listner = nullptr;
}

Controller_Point::~Controller_Point()
{
    
}
bool Controller_Point::init(){
    if (!Node::init())
    {
        return false;
    }
    
    return true;
}
bool Controller_Point::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    m_IsCanTouch = true;
    m_CurTouchType = ui::Widget::TouchEventType::BEGAN;
    m_PointCur = touch->getLocation();
    this->CalculateDirection();
    //CCLOG("Controller_Point::onTouchBegan [x:%0.1f  y:%0.1f ]",m_PointCur.x,m_PointCur.y);
    return true;
}
void Controller_Point::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    m_CurTouchType = ui::Widget::TouchEventType::MOVED;
    if (m_IsCanTouch)
    {
        m_PointLast = m_PointCur;
        m_PointCur = touch->getLocation();
        this->CalculateDirection();
    }
    //CCLOG("Controller_Point::onTouchMoved [x:%0.1f  y:%0.1f ]", m_PointCur.x, m_PointCur.y);
}
void Controller_Point::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    m_CurTouchType = ui::Widget::TouchEventType::ENDED;
    if (m_IsCanTouch)
    {
		this->CalculateDirection();
        m_PointLast = Point(0, 0);
        m_PointCur = Point(0, 0);
        
    }
    //CCLOG("Controller_Point::onTouchEnded [x:%0.1f  y:%0.1f ]", m_PointCur.x, m_PointCur.y);
}

/*
 @注册控制器的回调
 @SEL_CallFunc_SquareEnum 带方向枚举参数的函数
 @
 */
void Controller_Point::RegisterCallback(SEL_CallFunc_ButtonTypeAndPoint callback, Ref * register_user)
{
    m_OutFunction = callback;
    m_OutFunctioner = register_user;
    //(register_user->*m_OutFunction)(Square_DirType::Square_DirType_Left);
}
//void Controller_Square::RegisterCallback_2(CallFunc_Function_SquareEnum callback)
//{
//	m_OutFunction2 = callback;
//}
/*
 @根据2个点计算方向然后返回
 */
void Controller_Point::CalculateDirection()
{
    CallOutFunction(m_CurTouchType,m_PointCur);

}
/*
 @调用外部函数
 */
void Controller_Point::CallOutFunction(ui::Widget::TouchEventType s_type,Vec2 cur_vec2)
{
    
    if (m_OutFunction != nullptr)
    {
        
        (m_OutFunctioner->*m_OutFunction)(s_type,cur_vec2);
    }
}

