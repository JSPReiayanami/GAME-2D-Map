#include "Controller_Square.h"

Controller_Square::Controller_Square() :
m_OutFunction(nullptr),
m_OutFunctioner(nullptr),
m_PointCur(0, 0),
m_PointLast(0,0),
m_IsCanTouch(true)
{
	m_listner = nullptr;
}

Controller_Square::~Controller_Square()
{

}
bool Controller_Square::init(){
	if (!Node::init())
	{
		return false;
	}

	return true;
}
bool Controller_Square::onTouchBegan(Touch *touch, Event *unusedEvent)
{
	m_IsCanTouch = true;
	m_PointCur = touch->getLocation();
	CCLOG("Controller_Square::onTouchBegan [x:%0.1f  y:%0.1f ]",m_PointCur.x,m_PointCur.y);
	return true;
}
void Controller_Square::onTouchMoved(Touch *touch, Event *unusedEvent)
{
	if (m_IsCanTouch)
	{
		m_PointLast = m_PointCur;
		m_PointCur = touch->getLocation();
		this->CalculateDirection();
	}
	//CCLOG("Controller_Square::onTouchMoved [x:%0.1f  y:%0.1f ]", m_PointCur.x, m_PointCur.y);
}
void Controller_Square::onTouchEnded(Touch *touch, Event *unusedEvent)
{
	if (m_IsCanTouch)
	{
		m_PointLast = Point(0, 0);
		m_PointCur = Point(0, 0);
		this->CalculateDirection();
	}
	//CCLOG("Controller_Square::onTouchEnded [x:%0.1f  y:%0.1f ]", m_PointCur.x, m_PointCur.y);
}

/*
@注册控制器的回调
@SEL_CallFunc_SquareEnum 带方向枚举参数的函数
@
*/
void Controller_Square::RegisterCallback(SEL_CallFunc_SquareEnum callback, Ref * register_user)
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
void Controller_Square::CalculateDirection()
{
	//(m_OutFunctioner->*m_OutFunction)(Square_DirType::Square_DirType_Left);
	if (m_PointCur.x == 0 && m_PointCur.y ==  0 && m_PointLast.x == 0&& m_PointLast.y ==0)
	{
		CallOutFunction(Square_DirType_Null);
	}
	else
	{
		float delete_x = m_PointCur.x - m_PointLast.x;
		float delete_y = m_PointCur.y - m_PointLast.y;
		if (fabs(delete_x) > __SquareTouchDel || fabs(delete_y) > __SquareTouchDel)
		{
			if (fabs(delete_x) > fabs(delete_y))
			{
				if (delete_x < 0)
				{
					CallOutFunction(Square_DirType::Square_DirType_Left);
				}
				else
				{
					CallOutFunction(Square_DirType::Square_DirType_Right);
				}
			}
			else if (fabs(delete_x) < fabs(delete_y))
			{
				if (delete_y < 0)
				{
					CallOutFunction(Square_DirType::Square_DirType_Donw);
				}
				else
				{
					CallOutFunction(Square_DirType::Square_DirType_Up);
				}
			}
			else
			{
				CallOutFunction(Square_DirType_Null);
			}
		}
	}
}
/*
@调用外部函数
*/
void Controller_Square::CallOutFunction(Square_DirType s_type)
{
	m_IsCanTouch = false;
	if (m_OutFunction != nullptr)
	{
		(m_OutFunctioner->*m_OutFunction)(s_type);
	}
}

