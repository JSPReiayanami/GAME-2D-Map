#include "Behaviour.h"
#include "RoleModel.h"
#include "MapRoot.h"
#include "MapBase.h"
#include "Tool/MapHelper.h"
#include "FileData/FileData_Behaviour.h"
#include "Scene/GameScene.h"
#include "Scene/GameLogicDelegate.h"
Behaviour::Behaviour()
{
	m_Type = MAX;
	m_Role = nullptr;
	m_DoType = DoType::UnExecute;
	m_Loop = -1;
}

Behaviour::~Behaviour()
{
}

bool Behaviour::Init(RoleModel * rm,void * data)
{
	m_Role = rm;
	SetData(data);
	rm->AddBehaviour(this);
	return true;
}

bool Behaviour::IsDo(float dt)
{
	return false;
}
void Behaviour::Do(float dt)
{

}
void Behaviour::DoEnd()
{
	if (m_Role != nullptr)
	{
		m_Role->m_BehaviourCanDo = true;
		m_DoType = DoType::ExecutEnd;
	}
}
bool Behaviour::IsCanContinueExecuteing(float dt)
{
	return false;
}
void Behaviour::SetData(void * data)
{
	
}
bool Behaviour::WillDo(float dt)
{
	if (m_DoType == Behaviour::UnExecute)
	{
		return IsDo(dt);
	}
	else if (m_DoType == Behaviour::ExecutEnd)
	{
		if (IsCanContinueExecuteing(dt) == true && IsCanLoop())
		{
			m_DoType = Behaviour::UnExecute;
			DecLoop();
		}
	}
	return false;
}
bool Behaviour::IsCanLoop()
{
	if (m_Loop <0 || m_Loop >0)
	{
		return true;
	}
	return false;
}

void Behaviour::DecLoop(int decNum)
{
	if (m_Loop >0)
	{
		m_Loop -= decNum;
	}
}
void Behaviour::RunTo(PosInt pos, int playId)
{
	if (m_Role && m_Role->GetMyMap() != nullptr)
	{
		MapRoot * mapR = (MapRoot *)m_Role->GetMyMap();
		mapR->PlayerRunToPos(playId, pos);
	}
}
/*
[--------------------------------------------------]
//--MonitorNodes 监视目标行为触发走动，对话动作等等
[--------------------------------------------------]
*/
BeMonitorNodes::BeMonitorNodes()
{
	m_Type = Behaviour::MonitorNodes;
	m_MonitorId = 0;
	m_MonitorLenth = 1;
	m_IsLockPlayer = true;
	m_EndTime = 0;
	m_MonitorDir = DirType::DirT_Down;
}

BeMonitorNodes::~BeMonitorNodes()
{
}
bool BeMonitorNodes::Init(RoleModel * rm, void * data)
{
	if (!Behaviour::Init(rm,data))
	{
		return false;
	}
	m_Pos = rm->GetPos();
	return true;
}
bool BeMonitorNodes::IsDo(float dt)
{
	if (m_Role->IsRuning() == false && m_Role->GetDir() != m_MonitorDir)
	{
		m_Role->SetDir(m_MonitorDir);
	}

	if (IsInMyLenth() && m_DoType == DoType::UnExecute && m_MonitorDir == m_Role->GetDir())
	{

		m_DoType = DoType::Executeing;
		m_Role->m_BehaviourCanDo = false;
		m_EndTime = 0;
		m_IsTouchTarget = false;
		//--开始往监测的目标走过去
		RunToMonitorTarget();
		return true;
	}
	//====
	return false;
}
void BeMonitorNodes::Do(float dt)
{
	
	if (m_DoType == DoType::Executeing)
	{
		
		if (IsCloseMonitor() && m_IsTouchTarget == false && m_Role->IsRuning() == false)
		{
			if (m_Role && m_Role->GetMyMap() != nullptr)
			{
				MapRoot * mapR = (MapRoot *)m_Role->GetMyMap();
				RoleModel * monitorRole = mapR->GetRoleById(m_MonitorId);
				if (m_IsLockPlayer)
				{
					monitorRole->SetIsCanMove(true);
					DirType dir = MapHelper::GetOppositeDir(m_Role->GetDir());
					monitorRole->SetDir(dir);
					GameScene::GetInstance()->GetGameLogic()->DoTalk(m_Role->GetId());
				}
			}
			m_IsTouchTarget = true;
			
		}
		if (m_IsTouchTarget == true)
		{
			if (m_Role && m_Role->GetMyMap() != nullptr)
			{
				MapRoot * mapR = (MapRoot *)m_Role->GetMyMap();
				RoleModel * monitorRole = mapR->GetRoleById(m_MonitorId);
				if (MapHelper::GetTwoPosLenth(m_Role->GetPos(), monitorRole->GetPos()) > 2 && m_Role->IsRuning() == false)
				{
					RunTo(m_Pos, m_Role->GetId());
				}
			}
			//==
			if (PosInt::IsSamePosInt(m_Pos, m_Role->GetPos()) && m_Role->IsRuning() == false)
			{
				//--
				DoEnd();
			}
		}
	}
}

bool BeMonitorNodes::IsInMyLenth()
{
	if (m_Role && m_Role->GetMyMap() != nullptr)
	{
		MapRoot * mapR = (MapRoot *)m_Role->GetMyMap();
		RoleModel * monitorRole = mapR->GetRoleById(m_MonitorId);
		if (monitorRole != nullptr)
		{
			PosInt pos_mon = monitorRole->GetPos();
			PosInt pos_rol = m_Role->GetPos();
			/*
			@dir 1.上,2.下，3.左，4.右
			*/
			bool isDo = false;
			if (m_Role->GetDir() == DirType::DirT_Up && (pos_rol.getX() - pos_mon.getX()) == 0 && abs(pos_rol.getY() - pos_mon.getY()) <= m_MonitorLenth && (pos_rol.getY() - pos_mon.getY()) <0)
			{
				isDo = true;
			}
			else if (m_Role->GetDir() == DirType::DirT_Down && (pos_rol.getX() - pos_mon.getX()) == 0 && abs(pos_rol.getY() - pos_mon.getY()) <= m_MonitorLenth && (pos_rol.getY() - pos_mon.getY()) > 0)
			{
				isDo = true;
			}
			else if (m_Role->GetDir() == DirType::DirT_Left && (pos_rol.getY() - pos_mon.getY()) == 0 && abs(pos_rol.getX() - pos_mon.getX()) <= m_MonitorLenth && pos_rol.getX() - pos_mon.getX() > 0)
			{
				isDo = true;
			}
			else if (m_Role->GetDir() == DirType::DirT_Right && (pos_rol.getY() - pos_mon.getY()) == 0 && abs(pos_rol.getX() - pos_mon.getX()) <= m_MonitorLenth&& pos_rol.getX() - pos_mon.getX() < 0)
			{
				isDo = true;
			}
			//---如果满足条件，开始执行逻辑
			if (isDo)
			{
				return true;
			}
		}
	}
	return false;
}
bool BeMonitorNodes::IsCloseMonitor()
{
	if (m_Role && m_Role->GetMyMap() != nullptr)
	{
		MapRoot * mapR = (MapRoot *)m_Role->GetMyMap();
		RoleModel * monitorRole = mapR->GetRoleById(m_MonitorId);
		if (monitorRole != nullptr)
		{
			PosInt pos_mon = monitorRole->GetPos();
			PosInt pos_rol = m_Role->GetPos();
			int dis_x = abs(pos_mon.getX() - pos_rol.getX());
			int dis_y = abs(pos_mon.getY() - pos_rol.getY());
			if ((dis_y == 0 && dis_x == 1) || (dis_x == 0 && dis_y == 1))
			{
				return true;
			}
		}
	}
	return false;
}
bool BeMonitorNodes::IsCanContinueExecuteing(float dt)
{
	return true;
}
void BeMonitorNodes::SetData(void * data)
{
	if (data != nullptr)
	{
		CfgBeMonitor * cfg = (CfgBeMonitor *)data;
		m_Id = cfg->Id;
		m_Loop = cfg->Id;
		m_MonitorDir = (DirType)cfg->MonitorDir;
		m_MonitorId = cfg->MonitorId;
		m_MonitorLenth = cfg->MonitorLenth;
	}
}
void BeMonitorNodes::RunToMonitorTarget()
{
	if (m_Role && m_Role->GetMyMap() != nullptr)
	{
		MapRoot * mapR = (MapRoot *)m_Role->GetMyMap();
		RoleModel * monitorRole = mapR->GetRoleById(m_MonitorId);
		PosInt targetPos = MapHelper::GetPosIntWithDir(m_Role->GetDir(), monitorRole->GetPos(), 1);
		mapR->PlayerRunToPos(m_Role->GetId(),targetPos);
		if (m_IsLockPlayer)
		{
			monitorRole->SetIsCanMove(false);
		}
	}
}
/*
[--------------------------------------------------]
//--MonitorNodes 监视目标行为触发走动，对话动作等等 END
[--------------------------------------------------]
*/

/*
[--------------------------------------------------]
//--BeRandomRun 监视目标行为触发走动，对话动作等等
[--------------------------------------------------]
*/

BeRandomRun::BeRandomRun()
{
	m_Loop = -1;
	m_RunOdds = 50;
}
BeRandomRun::~BeRandomRun()
{

}
bool BeRandomRun::Init(RoleModel * rm, void * data)
{
	if (!Behaviour::Init(rm, data))
	{
		return false;
	}
	m_Pos = rm->GetPos();
	return true;
}
bool BeRandomRun::IsDo(float dt)
{
	if (true)
	{
		m_DoType = DoType::Executeing;
		m_Role->m_BehaviourCanDo = false;
		m_RunCount = 0;
		return true;
	}
	return false;
}
void BeRandomRun::Do(float dt)
{
	if (m_RunCount == 0)
	{
		m_RunCount++;
		RandomRun();
	}
	else if (m_Role->IsRuning() == false)
	{
		DoEnd();
	}
}
bool BeRandomRun::IsCanContinueExecuteing(float dt)
{
	if (CCRANDOM_0_1() * 100 < m_RunOdds)
	{
		return true;
	}
	return false;
}
void BeRandomRun::SetData(void * data)
{
	if (data != nullptr)
	{
		CfgBeRandomRun * cfg = (CfgBeRandomRun *)data;
		m_Id = cfg->Id;
		m_Loop = cfg->Loop;
		m_RunLenth = cfg->RunLenth;
		m_RunOdds = cfg->RunOdds;
		m_OneRunLenth = cfg->OneRunLenth;
	}
}
void BeRandomRun::RandomRun()
{
	PosInt pos = m_Role->GetPos();
	int pos_x_r = 0;
	int pos_y_r = 0;
	if (m_OneRunLenth == 0)
	{
		pos_x_r = CCRANDOM_0_1()*(m_RunLenth / 2 + 1);
		pos_y_r = CCRANDOM_0_1()*(m_RunLenth / 2 + 1);
	}
	else
	{
		pos_x_r = CCRANDOM_0_1()*(m_OneRunLenth + 1);
		pos_y_r = m_OneRunLenth - pos_x_r;
	}
	if ((int)(CCRANDOM_0_1() * 2) == 0)
	{
		pos_x_r = -pos_x_r;
	}
	if ((int)(CCRANDOM_0_1() * 2) == 0)
	{
		pos_y_r = -pos_y_r;
	}
	//==
	if (m_OneRunLenth == 0)
	{
		pos = m_Pos + PosInt(pos_x_r, pos_y_r);
	}
	else
	{
		pos = m_Role->GetPos() + PosInt(pos_x_r, pos_y_r);
		if (abs(pos.getX() - m_Pos.getX()) > (m_RunLenth / 2))
		{
			pos.setX(m_Pos.getX() + (pos_x_r > 0 ? (m_RunLenth / 2):- ((m_RunLenth / 2))));
		}
		if (abs(pos.getY() - m_Pos.getY()) > (m_RunLenth / 2))
		{
			pos.setY(m_Pos.getY() + (pos_y_r > 0 ? (m_RunLenth / 2) : -((m_RunLenth / 2))));
		}
	}
	
	if (PosInt::IsSamePosInt(pos,m_Role->GetPos()) == false)
	{
		RunTo(pos, m_Role->GetId());
	}
}
/*
[--------------------------------------------------]
//--BeRandomRun 监视目标行为触发走动，对话动作等等 END
[--------------------------------------------------]
*/

/*
[--------------------------------------------------]
//--TraceOne 跟随一个目标
[--------------------------------------------------]
*/
BeTraceOne::BeTraceOne()
{
	m_Loop = -1;
	m_TraceLenth = 1;
	m_TraceId = 0;
}
BeTraceOne::~BeTraceOne()
{

}
bool BeTraceOne::Init(RoleModel * rm, void * data)
{
	if (!Behaviour::Init(rm, data))
	{
		return false;
	}
	return true;
}
bool BeTraceOne::IsDo(float dt)
{
	if (true)
	{
		m_DoType = DoType::Executeing;
		m_Role->m_BehaviourCanDo = false;
		return true;
	}
	return false;
}
void BeTraceOne::Do(float dt)
{

	if (m_Role->IsRuning() == false)
	{
		DoEnd();
	}
	if (m_Role != nullptr)
	{
		MapBase * map = m_Role->GetMyMap();
		
	}
}
bool BeTraceOne::IsCanContinueExecuteing(float dt)
{
	return false;
}
void BeTraceOne::SetData(void * data)
{
	if (data != nullptr)
	{
		CfgBeTrace * cfg = (CfgBeTrace *)data;
		m_Id = cfg->Id;
		m_Loop = cfg->Loop;
		m_TraceId = cfg->TraceId;
		m_TraceLenth = cfg->TraceLenth;
	}
}
void BeTraceOne::TraceRun()
{
	
}






/*
[--------------------------------------------------]
//--TraceOne 跟随一个目标 END
[--------------------------------------------------]
*/