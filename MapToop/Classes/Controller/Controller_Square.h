#ifndef __Controller_Square_H__
#define __Controller_Square_H__
#include "Head.h"
//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Entity/Controller_Entity.h"
#else
#include "Controller_Entity.h"
#endif//判断路径的引用


class Controller_Square : public Controller_Entity
{
public:
	Controller_Square();
	~Controller_Square();
	/*
	@固定创建方法
	*/
	virtual bool init();
	CREATE_FUNC(Controller_Square);
	/*
		@注册控制器的回调
		@SEL_CallFunc_SquareEnum 带方向枚举参数的函数
		@
	*/
	void RegisterCallback(SEL_CallFunc_SquareEnum callback, Ref * register_user);
	//void RegisterCallback_2(CallFunc_Function_SquareEnum callback);
	Point m_PointCur;
	Point m_PointLast;
protected:

private:
	virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);
	virtual void onTouchMoved(Touch *touch, Event *unusedEvent);
	virtual void onTouchEnded(Touch *touch, Event *unusedEvent);
	SEL_CallFunc_SquareEnum m_OutFunction;
	Ref * m_OutFunctioner;
	
	//CallFunc_Function_SquareEnum m_OutFunction2;
	bool m_IsCanTouch;
	/*
		@根据2个点计算方向然后返回
	*/
	void CalculateDirection();
	/*
		@调用外部函数
	*/
	void CallOutFunction(Square_DirType s_type);
};
#endif // !__MAINSCENE__