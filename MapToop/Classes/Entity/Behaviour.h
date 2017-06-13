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
		TraceOne, //����һ��Ŀ��
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
	DoType m_DoType;//1.δִ��,2.����ִ��,3.ִ�н���
	int m_Loop;//��Ϊѭ����������������������޼���
	int m_Id;
protected:
	bool IsCanLoop();
	void DecLoop(int decNum = 1);//�����߼�ѭ������
	void RunTo(PosInt pos, int playId);
};
/*
[--------------------------------------------------]
//--MonitorNodes ����Ŀ����Ϊ�����߶����Ի������ȵ�
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
//--MonitorNodes ����Ŀ����Ϊ�����߶����Ի������ȵ� END
[--------------------------------------------------]
*/

/*
[--------------------------------------------------]
//--BeRandomRun ��������߶�
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
	int m_RunLenth;//����ƶ��ķ�Χ
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
//--BeRandomRun ��������߶� END
[--------------------------------------------------]
*/


/*
[--------------------------------------------------]
//--TraceOne ����һ��Ŀ��
[--------------------------------------------------]
*/
class BeTraceOne :public Behaviour
{
public:
	BeTraceOne();
	~BeTraceOne();
	CreatBehaviour(BeTraceOne)
public:
	int m_TraceId; //�����id����� Ϊ 0 �͸�������
	int m_TraceLenth;//����������
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
//--TraceOne ����һ��Ŀ�� END
[--------------------------------------------------]
*/

#endif // ! __Behaviour_H_