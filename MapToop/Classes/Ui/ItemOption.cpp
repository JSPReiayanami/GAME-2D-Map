#include "ItemOption.h"
ItemOption::ItemOption()
{
	m_CurDir = 2;
}
ItemOption::~ItemOption()
{

}

bool ItemOption::init(){
	if (!Layout::init()){
		return false;
	}
	this->setColor(Color3B::RED);
	this->setBackGroundColor(Color3B::YELLOW);
	this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	this->setClippingEnabled(true);
	this->setContentSize(Size(400,100));
	m_Layout = Layout::create();
	this->addChild(m_Layout);

	m_ViewLayout = Layout::create();
	m_ViewLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	m_ViewLayout->setContentSize(Size(400, 100));
	m_ViewLayout->setBackGroundColor(Color3B::GRAY);
	
	m_Layout->addChild(m_ViewLayout);

	m_Button1 = Layout::create();
	m_Button1->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	m_Button1->setBackGroundColor(Color3B::GREEN);
	m_Button1->setContentSize(Size(50, 100));
	m_Button1->setPosition(Vec2(400,0));
	m_Layout->addChild(m_Button1);

	m_Button2 = Layout::create();
	m_Button2->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	m_Button2->setBackGroundColor(Color3B::RED);
	m_Button2->setContentSize(Size(50, 100));
	m_Button2->setPosition(Vec2(450, 0));
	m_Layout->addChild(m_Button2);


	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->setSwallowTouches(true);

	listener1->onTouchBegan = [=](Touch* touch, Event* event){
		auto target = static_cast<Layout*>(event->getCurrentTarget());

		Rect rect = target->getBoundingBox();
		rect.size = rect.size+(Size)this->getPosition();
		if (rect.containsPoint(touch->getLocation()))
		{
			log("sprite3d began... x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);
			//target->setOpacity(100);
			m_Vec2Last = touch->getLocation();
			return true;
		}
		return false;
	};
	listener1->onTouchMoved = [=](Touch* touch, Event* event){
		if (m_IsRun == false)
		{
			m_Layout->stopAllActions();
			if (m_Vec2Last.x > touch->getLocation().x && m_CurDir == 2){
				m_IsRun = true;
				CCLOG("move to left");
				MoveTo * moveT = MoveTo::create(1, Vec2(-350,0));
				m_CurDir = 1;
				m_Layout->runAction(moveT);
			}
			else if (m_CurDir == 1)
			{
				m_IsRun = true;
				CCLOG("move to right");
				MoveTo * moveT = MoveTo::create(1, Vec2(0, 0));
				m_CurDir = 2;
				m_Layout->runAction(moveT);
			}
		}
		
	};

	listener1->onTouchEnded = [=](Touch* touch, Event* event){
		m_IsRun = false;
		//auto target = static_cast<Sprite3D*>(event->getCurrentTarget());
		//log("sprite3d onTouchesEnded.. ");
		//target->setOpacity(255);
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, m_ViewLayout);
	return true;
}