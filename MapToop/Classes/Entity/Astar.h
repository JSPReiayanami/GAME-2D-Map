#ifndef _Astar_H_
#define _Astar_H_
#include <vector>
#include <map>
class ASNode
{
public:
	ASNode();
	~ASNode();
	void RefreshFGH(ASNode * fnode, ASNode * enode);//ˢ���Լ���GFHֵ���ݴ���Ľڵ�
	void IsLock(bool is);
	bool IsTheSameType(std::vector<int> types);//�ж��Ƿ�������Щ����
	void SetNodeType(std::vector<int> types,bool isNew = true);
	void InitData();
	ASNode * m_fatherNode;//�������һ���ڵ�
	int m_G;//�Ѿ��߹���·����ֵ
	int m_F;//G+H �����߼��ж�
	int m_H;//�����յ��ֵ
	bool m_isLock;//�Ƿ���ס
	std::map<int,int> m_types;//������ɶ����,�����ж��Ƿ���Ա�Ѱ·
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
	//�����µ�ͼ���ݵ�ʱ��õĻᱻ�����
	void SetMapData(std::map<int , std::map<int,std::vector<int>>> mapData);
	bool FindPath(int posx, int posy, int s_posx, int s_posy, std::vector<int> canNotMoveTypes = {});
	std::map<int,std::map<int,int>> GetPath();
	void PrintPath();
	void ClearMapDate();
	void SetFindUseEight(bool is);
private:
	std::map<int, std::map<int, ASNode *>> m_mapNodes;
	std::vector<int> curCanNotMoveTypes;
	std::map<ASNode *,bool> openList;//���ŵĽڵ�
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