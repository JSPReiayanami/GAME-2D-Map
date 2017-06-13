#ifndef __MBUGBODY_H__
#define __MBUGBODY_H__
#include "Config/Head.h"
#include "Dll/MathFunction.h"
struct BugPath
{
	Vec2  pos;
	float roatation;
	int type;//0，空路径,1.有效路径
};

class MBugBody : public Node
{
public:
	MBugBody();
	~MBugBody();
	/*
	@固定创建方法
	*/
	static MBugBody * CreateBodyWithId(int Body_Id);
	virtual bool init();
	CREATE_FUNC(MBugBody);
	MBugBody * m_LastBody;
	MBugBody * m_NextBody;
	int m_Id;
	ImageView * m_Body;
	void MoveToPoint(Point pos);
	void BindNextBody(MBugBody * next_body);
	void SetMyBody(int Body_Id);
	virtual void update(float delta);
	void SetMyPos(Vec2 pos);
	float m_MoveSpeed;
	float m_NodeGap;
	float m_Time;
	float m_TimeGap;
	list<BugPath> m_path;
	void ClearPath();
	void SetPath(const list<BugPath> & path);
	void MoveWithPath();
};
#endif // !__MBUG_H__