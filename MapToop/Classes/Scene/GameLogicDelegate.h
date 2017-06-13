#ifndef __GameLogicDelegate_H__
#define __GameLogicDelegate_H__
#include "Config/Head.h"
class GameScene;
class RoleModel;
class MapRoot;
class GameLogicDelegate
{
public:
	GameLogicDelegate(GameScene * gameScene);
	~GameLogicDelegate();
	bool DoTalk(int npcId = -1);
	bool DoExplore();
	RoleModel * GetPlayer();
	RoleModel * GetPlayerFrontOrIn();
	MapRoot   * GetMap();
	RoleModel * GetNpc(int npcId);
	RoleModel * GetRoleModelWithPos(PosInt pos);
private:
	GameScene * m_GameScene;
};

#endif