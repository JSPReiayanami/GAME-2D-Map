#include "GameLogicDelegate.h"
#include "GameScene.h"
#include "MapRoot.h"
#include "Entity/MapBase.h"
#include "Entity/RoleModel.h"
#include "Tool/MapHelper.h"
#include "Scene/UILayer/Common/Interface_Dialog.h"
GameLogicDelegate::GameLogicDelegate(GameScene * gameScene)
{
	m_GameScene = gameScene;
}
GameLogicDelegate::~GameLogicDelegate()
{

}

bool GameLogicDelegate::DoTalk(int npcId)
{
	if (npcId > 0)
	{
		Interface_Dialog::PlayerTalk(npcId);
		return true;
	}
	RoleModel * rm_front = GetPlayerFrontOrIn();
	if (rm_front != nullptr && rm_front->GetModelType() == ModelType::ModelT_Role)
	{
		Interface_Dialog::PlayerTalk(rm_front->GetId());
		return true;
	}
	return false;
}

bool GameLogicDelegate::DoExplore()
{
	RoleModel * rm_front = GetPlayerFrontOrIn();
	if (rm_front != nullptr && rm_front->GetModelType() == ModelType::ModelT_Goods)
	{
		return true;
	}
	return false;
}

RoleModel * GameLogicDelegate::GetPlayerFrontOrIn()
{
	RoleModel * player = GetPlayer();
	RoleModel * rm_front = nullptr;
	if (player != nullptr)
	{
		PosInt pos_in = player->GetPos();
		DirType dir_in = player->GetDir();
		PosInt pos_front = MapHelper::GetPosIntWithDir(dir_in, pos_in, 1);
		rm_front = GetRoleModelWithPos(pos_front);
		if (rm_front == nullptr)
		{
			rm_front = GetRoleModelWithPos(pos_in);
		}
	}
	return rm_front;
}

RoleModel * GameLogicDelegate::GetPlayer()
{
	RoleModel * rm = GetNpc(PLAYER_1_UID);
	return rm;
}
MapRoot	* GameLogicDelegate::GetMap()
{
	MapRoot * mr = nullptr;
	if (m_GameScene != nullptr)
	{
		mr = (MapRoot *)m_GameScene->GetCurMap();
	}
	return mr;
}

RoleModel * GameLogicDelegate::GetNpc(int npcId)
{
	RoleModel * rm = nullptr;
	MapRoot * mr = GetMap();
	if (mr != nullptr)
	{
		rm = mr->GetRoleById(npcId);
	}
	return rm;
}

RoleModel * GameLogicDelegate::GetRoleModelWithPos(PosInt pos)
{
	RoleModel * rm = nullptr;
	MapRoot * mr = GetMap();
	if (mr != nullptr)
	{
		rm = mr->GetRoleByPos(pos);
	}
	return rm;
}