#ifndef __MapHelper_H__
#define __MapHelper_H__
#include "Config/Head.h"
#include "Entity/RoleModel.h"
#include "Entity/MapBase.h"
#include "MapRoot.h"
class Behaviour;
namespace MapHelper{
	PosInt GetPosIntWithDir(DirType dir,PosInt pos,int lenth = 1);
	float    GetTwoPosLenth(PosInt pos, PosInt posTarget);
	Animate * CreateAnimate(string name, int num, int speed);
	Behaviour * BindBehaviour(int beId, RoleModel * bindRm);
	bool IsCanMoveToForNpc(RoleModel * rm,PosInt pos);
	DirType GetDirWithPosToPos(PosInt pos, PosInt targetPos);
	DirType GetOppositeDir(DirType dir);

	bool IsInPolygon(Vec2 touch);
}
#endif //!__MapHelper_H__