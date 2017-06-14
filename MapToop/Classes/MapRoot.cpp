#include "MapRoot.h"
#include "Entity/RoleModel.h"
#include "Entity/Astar.h"
#include "Entity/Behaviour.h"
#include "Tool/MapHelper.h"
#include "Scene/GameScene.h"
#include "Scene/GameLogicDelegate.h"
#include "FileData/FileData_Map.h"
MapRoot::MapRoot()
{
	m_Player = nullptr;
}

MapRoot::~MapRoot()
{
}

MapRoot * MapRoot::Create(int mapId)
{
	MapRoot * mr = new MapRoot();
	if (mr && mr->init(mapId))
	{
		mr->autorelease();
		return mr;
	}
	else
	{
		CC_SAFE_DELETE(mr);
		mr = nullptr;
	}
	return mr;
}
bool MapRoot::init(int mapId)
{
	if (!MapBase::init())
	{
		return false;
	}
	SetMap(mapId);
	RefreshMap();
	ShowBlock();
	AddPlayer();
	return true;
}

void MapRoot::AddPlayer()
{

}

void MapRoot::PlayerRunToPos(int roleId, PosInt pos)
{
	RoleModel * rm = GetRoleById(roleId);
	if (rm && PosInt::IsSamePosInt(pos,rm->GetRunEndPos()) == false)
	{
		PosInt pos_rm = rm->GetPos();
		auto node = m_AStar->FindPath(pos.getX(), pos.getY(), pos_rm.getX(), pos_rm.getY(), rm->GetMyCanNotMoveNodeTypes());
		if (node == false)
		{
		}
		else{
			auto APth = m_AStar->GetPath();
			//
			vector<PosInt> intPath;
			intPath.clear();
			for (auto & pathx : APth)
			{
				for (auto & pa : pathx.second)
				{
					intPath.push_back(PosInt(pa.first, pa.second));
				}
			}
			//--一定要放在之后设置,因为在移动的时候会提前设置为false
			RoleModel * touchRM = GetRoleByPos(pos);
			bool isTouchNpc = false;
			if (touchRM != nullptr && touchRM != GetRoleById(roleId))
			{
				isTouchNpc = true;
			}
			rm->RunWithPath(intPath, isTouchNpc);
		}
	}
}

void MapRoot::TouchBlockPos(int posx, int posy)
{
	if (TouchNpc(PosInt(posx, posy)) == true)
	{
		return;
	}
	
	PlayerRunToPos(PLAYER_1_UID, PosInt(posx, posy));
}

void MapRoot::update(float delta)
{
	MapBase::update(delta);
}

bool MapRoot::TouchNpc(PosInt pos)
{
	RoleModel * rm = GetRoleByPos(pos);
	RoleModel * rm_p1 = GetRoleById(PLAYER_1_UID);
	if (rm && rm_p1 && rm != rm_p1)
	{
		float lenth = MapHelper::GetTwoPosLenth(rm->GetPos(), rm_p1->GetPos());
		if (lenth <= 1)
		{
			//--
			rm_p1->SetDir(MapHelper::GetDirWithPosToPos(rm_p1->GetPos(), rm->GetPos()));
			GameScene::GetInstance()->GetGameLogic()->DoTalk();
			return true;
		}
	}
	return false;
}

bool MapRoot::TouchGoods(PosInt pos)
{
	RoleModel * rm = GetRoleByPos(pos);
	RoleModel * rm_p1 = GetRoleById(PLAYER_1_UID);
	if (rm && rm_p1 && rm != rm_p1 && rm->GetModelType() == ModelType::ModelT_Goods)
	{
		int lenth = (int)MapHelper::GetTwoPosLenth(rm->GetPos(), rm_p1->GetPos());
		if (lenth == 1)
		{
			//--
			rm_p1->SetDir(MapHelper::GetDirWithPosToPos(rm_p1->GetPos(), rm->GetPos()));
			GameScene::GetInstance()->GetGameLogic()->DoExplore();
			return true;
		}
	}
	return false;
}