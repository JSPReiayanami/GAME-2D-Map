#ifndef __Controller_Entity_H__
#define __Controller_Entity_H__
#include "Config/Head.h"
class Controller_Entity : public Layout
{
public:
	Controller_Entity();
	~Controller_Entity();
	/*
	@固定创建方法
	*/
	virtual bool init();
	CREATE_FUNC(Controller_Entity);
	void OpenTouch();
	void CloseTouch();
	virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);
	virtual void onTouchMoved(Touch *touch, Event *unusedEvent);
	virtual void onTouchEnded(Touch *touch, Event *unusedEvent);
	EventListenerTouchOneByOne * m_listner;
protected:
private:
	
	
};
#endif // !__MAINSCENE__