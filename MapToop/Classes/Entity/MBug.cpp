#include "MBug.h"
#include "MBugBody.h"

//---

MBug::MBug(){
	m_Head = NULL;
	m_Bodys.clear();
}

MBug::~MBug(){

}

bool MBug::init(){
	if (!Node::init())
	{
		return false;
	}
	for (int i = 1; i <= 5;i++)
	{
		auto body = MBugBody::CreateBodyWithId(i);
		m_Bodys.insert(make_pair(i, body));
		if (i>=2)
		{
			m_Bodys.at(i - 1)->BindNextBody(body);
		}else if (i == 1)
		{
			m_Head = body;
		}
		addChild(body, 5 - i);
	}
	
	m_Head->setPosition(Vec2::ZERO);
	scheduleUpdate();
	return true;
}

void MBug::MoveToPoint(Point pos){
	m_Head->MoveToPoint(pos);
}

void MBug::SetMyPos(Vec2 pos)
{

}

void MBug::update(float delta)
{
	if (m_Head != NULL)
	{
		UpdateBodysPos();
	}
}

void MBug::UpdateBodysPos()
{
	
}
