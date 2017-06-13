#ifndef _Behaviour_H_
#define _Behaviour_H_
#include "Config/Head.h"

#define  CreatBehaviour(_class)\
static _class * CreateBehaviour(RoleModel * rm,void * data = nullptr){\
	_class * b = new _class(); \
	if (b && b->Init(rm, data))\
	{\
		return b;\
	}\
	else\
	{\
		delete b;\
		return nullptr;\
	}\
}

class RoleModel;
class Behaviour
{
public:
	enum BehaviourType
	{
		Stand = 1,
		RandomMove,
		MonitorNodes,
		Talk,	 
		TraceOne, //跟随一个目标
		MAX
	};
	enum DoType{
		UnExecute = 1,
		Executeing,
		ExecutEnd
	};
public:
	Behaviour();
	~Behaviour();
	CreatBehaviour(Behaviour)
	virtual bool Init(RoleModel * rm, void * data);
	virtual bool IsDo(float dt);
	virtual void Do(float dt);
	virtual void DoEnd();
	virtual bool IsCanContinueExecuteing(float dt);
	virtual void SetData(void * data);
	bool WillDo(float dt);
	BehaviourType m_Type;
	RoleModel * m_Role;
	DoType m_DoType;//1.未执行,2.正在执行,3.执行结束
	int m_Loop;//行为循环次数，负数代表可以无限激活
	int m_Id;
protected:
	bool IsCanLoop();
	void DecLoop(int decNum = 1);//减少逻辑循环次数
	void RunTo(PosInt pos, int playId);
};
/*
[--------------------------------------------------]
//--MonitorNodes 监视目标行为触发走动，对话动作等等
[--------------------------------------------------]
*/
class BeMonitorNodes :public Behaviour
{
public:
	BeMonitorNodes();
	~BeMonitorNodes();
	CreatBehaviour(BeMonitorNodes)
public:
	int m_MonitorId;
	int m_MonitorLenth;
	float m_EndTime;
	bool m_IsLockPlayer;
	bool m_IsTouchTarget;
	PosInt m_Pos;
	DirType m_MonitorDir;
	virtual bool Init(RoleModel * rm, void * data);
	virtual bool IsDo(float dt);
	virtual void Do(float dt);
	virtual bool IsCanContinueExecuteing(float dt);
	virtual void SetData(void * data);
private:
	bool IsInMyLenth();
	bool IsCloseMonitor();
	void RunToMonitorTarget();
};
/*
[--------------------------------------------------]
//--MonitorNodes 监视目标行为触发走动，对话动作等等 END
[--------------------------------------------------]
*/

/*
[--------------------------------------------------]
//--BeRandomRun 随机触发走动
[--------------------------------------------------]
*/
class BeRandomRun :public Behaviour
{
public:
	BeRandomRun();
	~BeRandomRun();
	CreatBehaviour(BeRandomRun)
public:
	int m_RunTime;
	int m_RunLenth;//随机移动的范围
	int m_RunOdds;
	int m_OneRunLenth;
	int m_RunCount;
	PosInt m_Pos;
	virtual bool Init(RoleModel * rm, void * data);
	virtual bool IsDo(float dt);
	virtual void Do(float dt);
	virtual bool IsCanContinueExecuteing(float dt);
	virtual void SetData(void * data);
private:
	void RandomRun();
};
/*
[--------------------------------------------------]
//--BeRandomRun 随机触发走动 END
[--------------------------------------------------]
*/


/*
[--------------------------------------------------]
//--TraceOne 跟随一个目标
[--------------------------------------------------]
*/
class BeTraceOne :public Behaviour
{
public:
	BeTraceOne();
	~BeTraceOne();
	CreatBehaviour(BeTraceOne)
public:
	int m_TraceId; //跟随的id，如果 为 0 就跟随主角
	int m_TraceLenth;//跟随间隔距离
	virtual bool Init(RoleModel * rm, void * data);
	virtual bool IsDo(float dt);
	virtual void Do(float dt);
	virtual bool IsCanContinueExecuteing(float dt);
	virtual void SetData(void * data);
private:
	void TraceRun();
};
/*
[--------------------------------------------------]
//--TraceOne 跟随一个目标 END
[--------------------------------------------------]
*/

#endif // ! __Behaviour_H_