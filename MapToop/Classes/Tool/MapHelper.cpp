#include "MapHelper.h"
#include "FileData/FileData_Behaviour.h"
#include "Entity/Behaviour.h"
#include "Entity/MapBase.h"
#include "Entity/RoleModel.h"
#include <math.h>
PosInt MapHelper::GetPosIntWithDir(DirType dir, PosInt pos, int lenth)
{
	switch (dir)
	{
	case DirType::DirT_Up:
		pos.setY(pos.getY() + lenth);
		break;
	case DirType::DirT_Down:
		pos.setY(pos.getY() + lenth);
		break;
	case DirType::DirT_Left:
		pos.setX(pos.getX() + lenth);
		break;
	case DirType::DirT_Right:
		pos.setX(pos.getX() - lenth);
		break;
	default:
		break;
	}
	return pos;
}

float MapHelper::GetTwoPosLenth(PosInt pos, PosInt posTarget)
{
	int dis_x = abs(pos.getX() - posTarget.getX());
	int dis_y = abs(pos.getY() - posTarget.getY());
	float lenth = sqrt(pow(dis_x, 2) + pow(dis_y, 2));
	return lenth;
}

Animate * MapHelper::CreateAnimate(string name, int num, int speed)
{
	Vector<SpriteFrame*> amV;
	amV.clear();
	for (int i = 1; i <= num; i++)
	{
		Sprite * sp = Sprite::create(StringUtils::format(name.c_str(), i).c_str());
		//SpriteFrame * sf = SpriteFrame::create(StringUtils::format(name.c_str(), i).c_str());
		SpriteFrame * sf = sp->getSpriteFrame()->clone();
		amV.pushBack(sf);
	}
	Animation * am = Animation::createWithSpriteFrames(amV, 0.5, -1);
	Animate * amte = Animate::create(am);
	return amte;
}

Behaviour * MapHelper::BindBehaviour(int beId,RoleModel * bindRm)
{
	int BeType = 0;
	const void * beData = FileDataBehaviour::GetInstance()->GetCfg(beId, BeType);
	Behaviour * behaviour = nullptr;
	switch (BeType)
	{
	case 1:
	{
			  const CfgBeMonitor * cfg = (CfgBeMonitor *)beData;
			  behaviour = BeMonitorNodes::CreateBehaviour(bindRm, (void *)cfg);
			  break;
	}
	case 2:
	{
			  const CfgBeRandomRun * cfg = (CfgBeRandomRun *)beData;
			  behaviour = BeRandomRun::CreateBehaviour(bindRm, (void *)cfg);
			  break;
	}
	default:
		break;
	}

	return behaviour;
}

bool MapHelper::IsCanMoveToForNpc(RoleModel * rm, PosInt pos)
{
	MapBase * mp = rm->GetMyMap();
	if (rm && mp)
	{
		if (rm->GetPos() == pos)
		{
			return true;
		}
		RoleModel * rm = mp->GetRoleByPos(pos);
		if (rm == nullptr || rm->GetModelType() == ModelT_Transfer)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

DirType MapHelper::GetDirWithPosToPos(PosInt pos, PosInt targetPos)
{
	DirType dir = DirType::DirT_MAX;
	int dis_x = pos.getX() - targetPos.getX();
	int dis_y = pos.getY() - targetPos.getY();
	if (abs(dis_x) > abs(dis_y))
	{
		if (dis_x >= 0)
		{
			dir = DirType::DirT_Right;
		}
		dir = DirType::DirT_Left;
	}
	else if (abs(dis_x) < abs(dis_y))
	{
		if (dis_y >= 0)
		{
			dir = DirType::DirT_Down;
		}
		dir = DirType::DirT_Up;
	}
	return dir;
}

DirType MapHelper::GetOppositeDir(DirType dir)
{
	DirType op_dir = DirType::DirT_MAX;
	if (dir == DirType::DirT_Left)
	{
		op_dir = DirType::DirT_Right;
	}
	if (dir == DirType::DirT_Right)
	{
		op_dir = DirType::DirT_Left;
	}
	if (dir == DirType::DirT_Down)
	{
		op_dir = DirType::DirT_Up;
	}
	if (dir == DirType::DirT_Up)
	{
		op_dir = DirType::DirT_Down;
	}
	return op_dir;
}

bool MapHelper::IsInPolygon(Vec2 touch)
{
	int i;
	int j;
	bool result = false;
// 	for (i = 0, j = points.length - 1; i < points.length; j = i++) {
// 		if ((points[i].y > test.y) != (points[j].y > test.y) &&
// 			(test.x < (points[j].x - points[i].x) * (test.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)) {
// 			result = !result;
// 		}
// 	}
	return result;
}