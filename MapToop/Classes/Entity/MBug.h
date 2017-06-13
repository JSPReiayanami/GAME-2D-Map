#ifndef __MBUG_H__
#define __MBUG_H__
#include "Config/Head.h"
class MBugBody;
class MBug : public Node
{
public:
	MBug();
	~MBug();
	/*
	@固定创建方法
	*/
	virtual bool init();
	CREATE_FUNC(MBug);
	void MoveToPoint(Point pos);
	virtual void update(float delta);
	void SetMyPos(Vec2 pos);
protected:

private:
	std::map<int, MBugBody *> m_Bodys;
	MBugBody * m_Head;
	void UpdateBodysPos();
};
#endif // !__MBUG_H__