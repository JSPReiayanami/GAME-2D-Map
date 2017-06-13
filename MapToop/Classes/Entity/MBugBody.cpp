#include "MBugBody.h"

using namespace MathHelper;

Vec2 FloatPosToVec2(const FloatPos & f_pos)
{
	return Vec2(f_pos.x, f_pos.y);
}
FloatPos Vec2ToFloatPos(const Vec2 & vec2)
{
	return FloatPos(vec2.x,vec2.y);
}

MBugBody::MBugBody(){
	m_LastBody = NULL;//上一节身体
	m_NextBody = NULL;//下一节身体
	m_Body = NULL;//自己的身体图片
	m_Time = 0;
	m_TimeGap = 0.02;//每次移动的间隙,改大间隙可以减小移动速度
	m_MoveSpeed = 40;//控制移动速度每一帧移动像素
	m_NodeGap = 0;//身体和身体间的间隙
}

MBugBody::~MBugBody(){

}

MBugBody * MBugBody::CreateBodyWithId(int Body_Id)
{
	MBugBody *pRet = new(std::nothrow) MBugBody();
	if (pRet && pRet->init())
	{
		pRet->SetMyBody(Body_Id);
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void MBugBody::SetMyBody(int Body_Id)
{
	if (m_Body == NULL)
	{
		m_Body = ImageView::create(StringUtils::format("%d.png", Body_Id));
		this->setContentSize(m_Body->getContentSize());
		m_Body->setAnchorPoint(Vec2::ZERO);
		this->addChild(m_Body, 5);
	}
	else
	{
		m_Body->loadTexture(StringUtils::format("%d.png", Body_Id));
	}
	m_Id = Body_Id;
}

bool MBugBody::init(){
	if (!Node::init())
	{
		return false;
	}
	this->setAnchorPoint(Vec2(0.5, 0.5));
	return true;
}
void MBugBody::SetMyPos(Vec2 pos)
{
	if (m_LastBody)
	{
		auto vec = GetPostionWithVectorAndLength(Vec2ToFloatPos(pos - this->getPosition()), m_NodeGap, 180);
		this->setPosition(FloatPosToVec2(vec) + pos);
	}
	else
	{
		this->setPosition(pos);
	}
}

void MBugBody::MoveToPoint(Point pos)
{
	if (m_LastBody == NULL)
	{
		this->unscheduleUpdate();
		ClearPath();
		auto cur_vec = this->getPosition();
		Vec2 vec = pos - cur_vec;
		float dis = sqrt(pow(cur_vec.x - pos.x,2)+pow(cur_vec.y - pos.y,2));
		float angle = GetAngleToOneRotation(Vec2ToFloatPos(cur_vec), Vec2ToFloatPos(pos));
		FloatPos one_pos = GetPostionWithVectorAndLength(Vec2ToFloatPos(vec), m_MoveSpeed, angle);
		Vec2 one_move = Vec2(one_pos.x, one_pos.y);
		int path_count = ceil(dis / m_MoveSpeed);
		for (int i = 1; i <= path_count; i++)
		{
			BugPath path;
			path.pos = cur_vec + one_move*i;
			path.type = 1;
			if (i == 1)
			{
				path.roatation = 360 - angle;
			}
			else{
				path.roatation = -1;
			}
			m_path.push_back(path);
		}
		if (m_NextBody)
		{
			m_NextBody->SetPath(m_path);
		}
		//--开始
		this->scheduleUpdate();
	}
}

void MBugBody::BindNextBody(MBugBody * next_body)
{
	m_NextBody = next_body;
	next_body->m_LastBody = this;
	next_body->m_NodeGap = m_Body->getContentSize().width / 2;
	next_body->SetMyPos(Vec2(this->getPosition()));
}
void MBugBody::update(float delta)
{
	m_Time += delta;
	if (m_Time >= m_TimeGap)
	{
		m_Time = 0;
		if (m_LastBody == NULL)
		{
			MoveWithPath();
		}
	}
}

void MBugBody::ClearPath()
{
	m_path.clear();
	if (m_NextBody)
	{
		m_NextBody->ClearPath();
	}
}
void MBugBody::SetPath(const list<BugPath> & path)
{
	m_path.clear();
	Vec2 temp_pos = this->getPosition();
	for (auto & p : path)
	{
		BugPath new_path;
		Vec2 vec = (temp_pos - p.pos);
		float angle = GetAngleToOneRotation(Vec2ToFloatPos(temp_pos), Vec2ToFloatPos(p.pos));
		Vec2 pos_add = FloatPosToVec2(GetPostionWithVectorAndLength(Vec2ToFloatPos(vec), m_NodeGap, angle + 180));
		new_path.type = 1;
		new_path.roatation = 360 - angle;
		new_path.pos = pos_add + p.pos;
		temp_pos = new_path.pos;
		m_path.push_back(new_path);
	}
	m_path.pop_back();
	if (m_NextBody)
	{
		m_NextBody->SetPath(m_path);
	}
}
void MBugBody::MoveWithPath()
{
	if (m_path.size() > 0)
	{
		list<BugPath>::iterator end_ite = m_path.begin();
		BugPath path = (*end_ite);
		if (path.type == 1)
		{
			this->setPosition(path.pos);
			if (path.roatation > 0)
			{
				this->setRotation(path.roatation);
			}
		}
		m_path.pop_front();
	}
	if (m_LastBody == NULL && m_path.size() == 0)
	{
		this->unscheduleUpdate();
	}
	if (m_NextBody)
	{
		m_NextBody->MoveWithPath();
	}
}