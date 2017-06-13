#ifndef _Astar_H_
#define _Astar_H_
#include <vector>
#include <map>
class ASNode
{
public:
	ASNode();
	~ASNode();
	void RefreshFGH(ASNode * fnode, ASNode * enode);//刷新自己的GFH值根据传入的节点
	void IsLock(bool is);
	bool IsTheSameType(std::vector<int> types);//判断是否属于这些类型
	void SetNodeType(std::vector<int> types,bool isNew = true);
	void InitData();
	ASNode * m_fatherNode;//这个的上一个节点
	int m_G;//已经走过的路径的值
	int m_F;//G+H 用于逻辑判断
	int m_H;//距离终点的值
	bool m_isLock;//是否锁住
	std::map<int,int> m_types;//定义是啥类型,用于判断是否可以被寻路
	int m_posX;
	int m_posY;
private:

};

/*
@=======================================
*/

class AStar
{
public:
	AStar();
	~AStar();
	//设置新地图数据的时候久的会被清理掉
	void SetMapData(std::map<int , std::map<int,std::vector<int>>> mapData);
	bool FindPath(int posx, int posy, int s_posx, int s_posy, std::vector<int> canNotMoveTypes = {});
	std::map<int,std::map<int,int>> GetPath();
	void PrintPath();
	void ClearMapDate();
	void SetFindUseEight(bool is);
private:
	std::map<int, std::map<int, ASNode *>> m_mapNodes;
	std::vector<int> curCanNotMoveTypes;
	std::map<ASNode *,bool> openList;//开放的节点
	ASNode * start_node;
	ASNode * end_node;
	void InitAllNodeState();
	ASNode * GetNodeWithPos(int posx, int posy);
	ASNode * SetOpenNewNode(ASNode * node);
	ASNode * AddToOpenList(int posx, int posy, ASNode * node);
	ASNode * GetFindFminNode();
	int FindCount;
	bool isEight;
};



#endif //!_Astar_H_