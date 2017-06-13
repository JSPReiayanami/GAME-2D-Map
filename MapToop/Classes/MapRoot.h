#ifndef _MapRoot_H_
#define _MapRoot_H_
#include "Config/Head.h"
#include "Entity/MapBase.h"
class RoleModel;
class MapRoot :public MapBase
{
public:
	MapRoot();
	~MapRoot();
	static MapRoot * Create(int mapId);
	bool init(int mapId);
	void AddPlayer();
	void PlayerRunToPos(int roleId, PosInt pos);
private:
	void TouchBlockPos(int posx, int posy);
	void update(float delta);
	bool TouchNpc(PosInt pos);
	bool TouchGoods(PosInt pos);
};



#endif //!_MapRoot_H_