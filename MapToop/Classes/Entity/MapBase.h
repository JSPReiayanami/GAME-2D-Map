#ifndef _MapBase_H_
#define _MapBase_H_
#include "Config/Head.h"
class Block;
class AStar;
class RoleModel;

typedef std::map<int, std::map<int, Block *>> MapBlocks;
typedef std::map<int, std::map<int, int>> MapMapInt;
class MapBase : public Layout
{
public:
	MapBase();
	~MapBase();
	CREATE_FUNC(MapBase);
	virtual bool init();
public:
	Layout * m_LayoutGround;
	Layout * m_LayoutBlock;
	Layout * m_LayoutSpirte;
	Layout * m_LayoutSky;
	Layout * m_MapView;
	std::map<int, std::map<int, Block *>> m_Blocks;
	//--
	int m_MapId;
	int m_LockPlayerId;
	float m_CurScale;
	std::map<int, std::map<int, int>> m_BlockData;
	std::map<int, std::map<int, std::vector<int>>> m_ASTARData;
	std::vector<OhterElement> m_Element;
	AStar * m_AStar;
	Size m_MapSize;
	RoleModel * m_Player;
	std::map<int, RoleModel *> m_Roles;
public:
	void ShowBlock();
	void HiedBlock();
	Vec2 GetPos(int posx,int posy);
	int  GetPosType(PosInt pos);
	RoleModel * GetRoleById(int roleId);
	RoleModel * GetRoleByPos(PosInt pos);
	RoleModel * AddPlayer(int roleId);
	void SetLockPlayer(int roleId);
public:
	virtual void InitMapWidget();
	virtual void SetMap(int mapId);
	virtual void ClearMapViewData();
	virtual void RefreshMap();
	virtual void LockPlayer();
	virtual void update(float delta);
public:
	std::string GetMapDataFile();
	std::string GetMapViewFile();
	bool LoadMapData(string mapDataFile);
	Layout * LoadMap(string mapViewFile);
	virtual void LoadRoleModel();
	virtual void TouchEvent(ui::Widget::TouchEventType touch_type, Vec2 touch_point);
	virtual void TouchBlockPos(int posx,int posy);
	void SetPostionMapMove(Vec2 postion);
};





#endif // ! __MapBase_H_