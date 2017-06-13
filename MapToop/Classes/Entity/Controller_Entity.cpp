#include "Controller_Entity.h"
Controller_Entity::Controller_Entity()
{
	m_listner = nullptr;
}

Controller_Entity::~Controller_Entity()
{

}
bool Controller_Entity::init(){
	if (!Node::init())
	{
		return false;
	}
	
	return true;
}

void Controller_Entity::OpenTouch(){
	if (m_listner != nullptr)
	{
		this->CloseTouch();
	}
	m_listner = EventListenerTouchOneByOne::create();
	m_listner->onTouchBegan = CC_CALLBACK_2(Controller_Entity::onTouchBegan, this);
	m_listner->onTouchMoved = CC_CALLBACK_2(Controller_Entity::onTouchMoved, this);
	m_listner->onTouchEnded = CC_CALLBACK_2(Controller_Entity::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listner, this);
}
void Controller_Entity::CloseTouch(){
	if (m_listner != nullptr)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_listner);
	}
}

bool Controller_Entity::onTouchBegan(Touch *touch, Event *unusedEvent)
{	
	CCLOG("Controller_Entity::onTouchBegan");
	return true;
}
void Controller_Entity::onTouchMoved(Touch *touch, Event *unusedEvent)
{
	CCLOG("Controller_Entity::onTouchMoved");
}
void Controller_Entity::onTouchEnded(Touch *touch, Event *unusedEvent)
{
	CCLOG("Controller_Entity::onTouchEnded");
}