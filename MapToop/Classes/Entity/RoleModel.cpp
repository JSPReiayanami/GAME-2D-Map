#include "RoleModel.h"
#include "EffectAnimation.h"
#include "MapBase.h"
#include "Behaviour.h"
#include "Tool/MapHelper.h"
#include "FileData/FileData_Anim.h"
#include "FileData/FileData_Npc.h"
#include "Scene/GameScene.h"
#include "Scene/GameLogicDelegate.h"
RoleModel::RoleModel()
{
	m_MyMap = nullptr;
	m_RunAction = nullptr;
	m_Model = nullptr;
	m_Pos = PosInt(0,0);
	m_Speed = 0.35;
	m_Behaviour.clear();
	m_Dir = DirType::DirT_Down;
	m_CurBehaviour = nullptr;
	m_CanNotTypes.clear();
	m_CanNotTypes.push_back(2);
	m_IsCanMove = true;
	m_Id = 0;
	m_ModelId = 0;
	m_ModelType = ModelType::ModelT_Role;
}

RoleModel::~RoleModel()
{
	for (auto * be : m_Behaviour)
	{
		delete be;
	}
	m_Behaviour.clear();
}

RoleModel * RoleModel::Create(int roleId)
{
	RoleModel * rm = new RoleModel();
	if (rm && rm->init(roleId))
	{
		rm->autorelease();
		return rm;
	}
	else
	{
		CC_SAFE_DELETE(rm);
	}
	return rm;
}
bool RoleModel::init(int roleId)
{
	if (!Layout::init())
	{
		return false;
	}
	//this->setColor(Color3B::RED);
	//this->setBackGroundColor(Color3B::YELLOW);
	//this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	this->setAnchorPoint(Vec2(0.5,0));
	InitRoleWidget();
	SetId(roleId);
	this->scheduleUpdate();
	return true;
}

void RoleModel::SetId(int id)
{
	m_Id = id;
	const CfgNpc* cfg = FileDataNpc::GetInstance()->GetCfg(m_Id);
	if (cfg != nullptr)
	{
		m_Speed = cfg->AnimSpeed;
		m_ModelId = cfg->ModelImageId;
		UpdateView();
		UpdateBehaviour();
	}
	
}
void RoleModel::SetModelId(int modelId)
{
	m_ModelId = modelId;
	const CfgAnim * cfg = FileDataAnim::GetInstance()->GetCfg(m_ModelId);
	if (cfg != nullptr)
	{
		RefreshAnim(cfg);
	}
}
void RoleModel::SetBehaviour(vector<int> BehaviourId)
{
	ClearAllBehaviour();
	for (auto & beId : BehaviourId)
	{
		MapHelper::BindBehaviour(beId, this);
	}
}
void RoleModel::ClearAllBehaviour()
{
	this->unscheduleUpdate();
	m_Behaviour.clear();
	m_CurBehaviour = nullptr;
	m_BehaviourCanDo = true;
	this->scheduleUpdate();
}
void RoleModel::UpdateBehaviour()
{
	const CfgNpc* cfg = FileDataNpc::GetInstance()->GetCfg(m_Id);
	if (cfg != nullptr)
	{
		SetBehaviour(cfg->Behavior);
	}
}
void RoleModel::UpdateView()
{
	SetModelId(m_ModelId);
}
void RoleModel::UpdataBehaviour()
{

}
//public:
void RoleModel::SetMapDelegate(MapBase * mapBase)
{
	m_MyMap = mapBase;
}
void RoleModel::RunWithPath(vector<PosInt> path, bool isTouchNpc)
{
	m_IsTouchNpc = isTouchNpc;
	if (m_RunPath.size() <= 0){
		m_RunPath.clear();
		m_RunPath = path;
		m_RunAction = nullptr;
		RunPath();
	}
	else 
	{
		m_IsTouchNpcCache = isTouchNpc;
		m_RunPathCache = path;
	}
}
void RoleModel::AddBehaviour(Behaviour * haviour)
{
	m_Behaviour.push_back(haviour);
}
void RoleModel::SetIsCanMove(bool is)
{
	if (m_IsCanMove != is)
	{
		m_IsCanMove = is;
		if (m_IsCanMove == false)
		{
			m_RunPath.clear();
		}
	}
}
void RoleModel::SetPos(int posX, int posY)
{
	if (m_MyMap != nullptr)
	{
		SetPos(PosInt(posX,posY));
		this->setPosition(m_MyMap->GetPos(posX, posY));
	}
}
void RoleModel::SetPosForEditor(int posX, int posY)
{
	m_Pos = PosInt(posX,posY);
}
PosInt RoleModel::GetPos()
{
	return m_Pos;
}
PosInt RoleModel::GetRunEndPos()
{
	if (m_RunPath.size() > 0)
	{
		return *m_RunPath.begin();
	}
	else
	{
		return m_Pos;
	}
}
MapBase * RoleModel::GetMyMap()
{
	return m_MyMap;
}
DirType RoleModel::GetDir()
{
	return m_Dir;
}
ModelType RoleModel::GetModelType()
{
	return m_ModelType;
}
vector<int> RoleModel::GetMyCanNotMoveNodeTypes()
{
	return m_CanNotTypes;
}
int RoleModel::GetId()
{
	return m_Id;
}
void RoleModel::SetDir(DirType dir)
{
	if (m_Dir != dir && dir != DirType::DirT_MAX)
	{
		m_Dir = dir;
		RefreshAnimWithDir(m_Dir);
	}
}
void RoleModel::RefreshAnimWithDir(DirType dir)
{
	if (dir != DirType::DirT_MAX)
	{
		m_Model->setScaleX(1);
		if (dir == DirType::DirT_Right)
		{
			m_Model->setScaleX(-1);
			dir = DirType::DirT_Left;
		}
		m_Model->PlayAnimation(StringUtils::format("%d", (int)(dir)));
	}
}
bool RoleModel::IsRuning()
{
	if (m_RunPath.size() <= 0)
	{
		return false;
	}
	return true;
}

//protected:
#define  InitLayoutForMap(who,zder,father)\
	who = Layout::create(); \
	who->setLocalZOrder(zder++); \
	father->addChild(who);
void RoleModel::InitRoleWidget()
{
	int zOder = 1;
	InitLayoutForMap(m_LayoutModel,zOder,this)
	InitLayoutForMap(m_LayoutContent, zOder, this)
}

void RoleModel::SetPos(PosInt pos)
{
	//--判断与之前坐标的方向
	int dis_x = m_Pos.getX() - pos.getX();
	int dis_y = m_Pos.getY() - pos.getY();
	DirType dir = m_Dir;
	if (dis_x == 0 && dis_y > 0)
	{
		dir = DirType::DirT_Down;
	}
	else if (dis_x == 0 && dis_y < 0)
	{
		dir = DirType::DirT_Up;
	}
	else if (dis_y == 0 && dis_x > 0)
	{
		dir = DirType::DirT_Left;
	}
	else if (dis_y == 0 && dis_x < 0)
	{
		dir = DirType::DirT_Right;
	}
	m_Pos = pos;
	SetDir(dir);
}
//private:
void RoleModel::RefreshAnim(const CfgAnim * cfg)
{
	// 		Armature * am = Armature::create("");
	// 		am->getAnimation()->play("");
	if (m_Model != nullptr)
	{
		m_Model->removeFromParent();
	}
	for (int count = 1; count <= 1;count++)
	{
		//EffectArmatureFuncData func = EffectArmatureFuncData("Rotate", 4, CC_CALLBACK_3(HelloWorld::AnimaCallFunc, this));
		//EffectArmatureFuncData func2 = EffectArmatureFuncData("Rotate", 2, CC_CALLBACK_3(HelloWorld::AnimaCallFunc, this));
		auto effctAnima = EffectArmature::create("1.png");
		//effctAnima->AddAnimateCallFunc(func);
		//effctAnima->AddAnimateCallFunc(func2);
		Animate * amte = nullptr;
		amte = MapHelper::CreateAnimate(cfg->FAnim, cfg->FAnimNum, m_Speed);
		if (amte != nullptr)
		{
			effctAnima->AddAnimate(amte,StringUtils::format("%d",(int)(DirType::DirT_Down)));
		}
		amte = nullptr;
		amte = MapHelper::CreateAnimate(cfg->LRAnim, cfg->LRAnimNum, m_Speed);
		if (amte != nullptr)
		{
			effctAnima->AddAnimate(amte, StringUtils::format("%d", (int)(DirType::DirT_Left)));
		}
		amte = nullptr;
		amte = MapHelper::CreateAnimate(cfg->BAnim, cfg->BAnimNum, m_Speed);
		if (amte != nullptr)
		{
			effctAnima->AddAnimate(amte, StringUtils::format("%d", (int)(DirType::DirT_Up)));
		}
		effctAnima->setPosition(effctAnima->getContentSize() / 2);
		m_LayoutModel->addChild(effctAnima);
		m_Model = effctAnima;
		this->setContentSize(effctAnima->getContentSize());
		auto size = this->getContentSize();
		RefreshAnimWithDir(m_Dir);
	}
	Adaptive();
}

void RoleModel::Adaptive()
{
	Size rm_size = this->getContentSize();
	if (rm_size.width > __BlockSize && rm_size.height > 0)
	{
		float sc = 1.0 / (rm_size.width / __BlockSize);
		this->setScale(sc);
	}
}

void RoleModel::RunPath()
{
	//==是否有缓冲的更改
	if (m_RunPathCache.size() > 0)
	{
		m_RunPath.clear();
		m_RunPath = m_RunPathCache;
		m_IsTouchNpc = m_IsTouchNpcCache;
		m_RunPathCache.clear();
	}
	if (m_RunPath.size() > 0 && m_MyMap != nullptr && m_IsCanMove == true)
	{
		/*if (m_RunAction != nullptr)
		{
			m_RunAction->stop();
		}*/
		this->stopActionByTag(888);
		PosInt pos = m_RunPath.at(m_RunPath.size() - 1);
		//--处理一下移动自己脚下格子的蛋疼事情
		if (pos == m_Pos)
		{
			if (m_RunPath.size() > 0)
			{
				m_RunPath.erase(m_RunPath.begin() + (m_RunPath.size() - 1));
			}
			this->RunPath();
			return;
		}
		if (MapHelper::IsCanMoveToForNpc(this,pos) == false)
		{
			
			SetDir(MapHelper::GetDirWithPosToPos(m_Pos, pos));
			m_RunPath.clear();
			RunEnd();
			return;
		}
		SetPos(pos);
		MoveTo * moveTo = MoveTo::create(m_Speed, m_MyMap->GetPos(pos.getX(), pos.getY()));
		//CCLOG("RUN TO :%0.1f %0.1f", m_MyMap->GetPos(pos.getX(), pos.getY()).x, m_MyMap->GetPos(pos.getX(), pos.getY()).y);
		Sequence * seq = Sequence::create(moveTo,CallFunc::create([=](){
			/*if (m_RunPath.size() > 0 && m_MyMap != nullptr && m_IsCanMove == true)
			{
			SetPos(m_RunPath.at(m_RunPath.size() - 1));
			}*/
			if (m_RunPath.size() > 0)
			{
				m_RunPath.erase(m_RunPath.begin() + (m_RunPath.size() - 1));
			}
			this->RunPath();
		}),NULL);
		seq->setTag(888);
		m_RunAction = this->runAction(seq);
	}
	else
	{
		m_RunAction = nullptr;
		RunEnd();
	}
}

void RoleModel::RunEnd()
{
	if (m_IsTouchNpc == true)
	{
		bool isOk = false;
		isOk = GameScene::GetInstance()->GetGameLogic()->DoTalk();
		if (isOk == false)
		{
			isOk = GameScene::GetInstance()->GetGameLogic()->DoExplore();
		}
	}
	m_IsTouchNpc = false;
}

void RoleModel::update(float delta)
{
	ActiveBehaviour(delta);
	//ExecuteBehaviour(delta);
}

void RoleModel::ActiveBehaviour(float delta)
{
	if (m_MyMap != nullptr)
	{
		for (auto & be : m_Behaviour)
		{
			if (m_BehaviourCanDo == true){
				if (be->WillDo(delta))
				{
					m_CurBehaviour = be;
				}
			}
			else
			{
				if (m_CurBehaviour == nullptr)
				{
					m_BehaviourCanDo = true;
				}
				else
				{
					m_CurBehaviour->Do(delta);
				}
				break;
			}
		}
	}
}

void RoleModel::ExecuteBehaviour(float delta)
{
	if (m_MyMap != nullptr)
	{
		for (auto & be : m_Behaviour)
		{
			if (m_CurBehaviour != be && be->m_DoType == Behaviour::Executeing && m_BehaviourCanDo == true)
			{
				be->Do(delta);
			}
		}
	}
}