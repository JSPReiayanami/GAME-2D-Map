#ifndef _Map2DBase_H_
#define _Map2DBase_H_
#include "Config/Head.h"
#include "Entity/MapBase.h"
;
class Map2DBase : public MapBase
{
public:
	Map2DBase();
	~Map2DBase();
	static Map2DBase * Create(int mapId);
	bool init(int mapId);
	void AddPlayer();
	void PlayerRunToPos(int roleId, PosInt pos);
private:
	void TouchBlockPos(int posx, int posy);
	void update(float delta);
	bool TouchNpc(PosInt pos);
	bool TouchGoods(PosInt pos);
};





#endif // ! __Map2DBase_H_