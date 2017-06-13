#include "Astar.h"
#include "Config/Head.h"
#define  H_Value 10
#define  HH_Value 15
#define  F_Value 10
ASNode::ASNode()
{

}
ASNode::~ASNode()
{

}
void ASNode::RefreshFGH(ASNode * fnode, ASNode * enode)
{
	int delete_x = fnode->m_posX - m_posX;
	int delete_y = fnode->m_posY - m_posY;
	int dend_x = enode->m_posX - m_posX;
	int dend_y = enode->m_posY - m_posY;
	//计算路径叠加值
	if (abs(delete_x) == 1 && abs(delete_y) == 1)
	{
		m_G = fnode->m_G + HH_Value;
	}
	else
	{
		m_G = fnode->m_G + H_Value;
	}
	//计算距离终点的距离
	m_F = (abs(dend_x) + abs(dend_y)) * F_Value;
}
void ASNode::IsLock(bool is)
{
	m_isLock = is;
}
bool ASNode::IsTheSameType(std::vector<int> types)
{
	if (types.size() == 0 && m_types.size() != 0) {return false;}
	int same_count = 0;
	for (auto & type : types)
	{
		if (m_types.find(type) != m_types.end())
		{
			same_count++;
			return true;//暂时有就说明不能移动过去
		}
	}
	if (same_count == types.size()) return true;

	return false;
}
void ASNode::SetNodeType(std::vector<int> types, bool isNew)
{	
	if (true == isNew){ m_types.clear(); }
	for (auto & type : types)
	{
		if (m_types.find(type) == m_types.end())
		{
			m_types.insert(std::make_pair(type, 1));
		}
	}
}
void ASNode::InitData()
{
	m_H = 0;
	m_G = 0;
	m_F = 0;
	m_fatherNode = nullptr;
	m_isLock = false;
}
/*
@=======================================
*/

AStar::AStar()
{
	m_mapNodes.clear();
	isEight = false;
}

AStar::~AStar()
{
	ClearMapDate();
}

void AStar::SetMapData(std::map<int, std::map<int, std::vector<int>>> mapData)
{
	ClearMapDate();
	for (auto & Map : mapData)
	{
		for (auto & data : Map.second)
		{
			ASNode * node = new ASNode();
			node->m_posX = data.first;
			node->m_posY = Map.first;
			node->SetNodeType(data.second);
			if (m_mapNodes.find(data.first) == m_mapNodes.end())
			{
				std::map<int, ASNode *> temp;
				temp.clear();
				temp.insert(std::make_pair(Map.first, node));
				m_mapNodes.insert(std::make_pair(data.first, temp));
			}
			else
			{
				m_mapNodes.at(data.first).insert(std::make_pair(Map.first, node));
			}
		}
	}
}
bool AStar::FindPath(int posx, int posy, int s_posx, int s_posy, std::vector<int> canNotMoveTypes)
{
	InitAllNodeState();
	//--准备一些要寻找的数据
	end_node = GetNodeWithPos(posx, posy);
	//判断终点是不是不能移动过去
	if (end_node == nullptr || end_node->IsTheSameType(canNotMoveTypes))
	{
		return false;
	}
	start_node = GetNodeWithPos(s_posx, s_posy);
	if (nullptr == end_node || nullptr == start_node) { return false; }

	if (posx == s_posx && posy == s_posy) { return false; }
	start_node->IsLock(true);
	for (auto type : canNotMoveTypes)
	{
		curCanNotMoveTypes.push_back(type);
	}

	ASNode * findNode = start_node;
	do
	{
		ASNode * isEndNode = SetOpenNewNode(findNode);
		if (isEndNode != nullptr) {
			end_node->m_fatherNode = isEndNode;
			return true;
		}
		
		ASNode * minNode = GetFindFminNode();
		if (minNode == findNode || minNode == nullptr){ return false; }
		findNode = minNode;
	} while (findNode != nullptr);
	return false;
}

std::map<int, std::map<int, int>> AStar::GetPath()
{
	std::map<int, std::map<int, int>> APath;
	APath.clear();
	if (end_node == nullptr)
	{
		return APath;
	}
	ASNode * node = end_node;
	while (node != nullptr)
	{

		std::map<int, int> pos;
		pos.clear();
		pos.insert(std::make_pair(node->m_posX, node->m_posY));
		APath.insert(std::make_pair(APath.size() + 1, pos));
		node = node->m_fatherNode;
	}
	return APath;
}

void AStar::PrintPath()
{
	std::map<int, std::map<int, int>> APath;
	APath.clear();
	if (end_node == nullptr)
	{
		return;
	}
	int count = 1;
	ASNode * node = end_node;
	while (node != nullptr)
	{
		
		CCLOG("PATH [%d] [%d,%d]", count, node->m_posX, node->m_posY);
		node = node->m_fatherNode;
		count++;
	}
}
//===
void AStar::ClearMapDate()
{
	for (auto & mapNode : m_mapNodes)
	{
		for (auto & node : mapNode.second)
		{
			delete node.second;
		}
	}
	m_mapNodes.clear();
}

void AStar::SetFindUseEight(bool is)
{
	isEight = is;
}
void AStar::InitAllNodeState()
{
	end_node = nullptr;
	start_node = nullptr;
	curCanNotMoveTypes.clear();
	openList.clear();
	for (auto & mapNode : m_mapNodes)
	{
		for (auto & node : mapNode.second)
		{
			node.second->InitData();
		}
	}
}

ASNode * AStar::GetNodeWithPos(int posx, int posy)
{
	ASNode * node = nullptr;
	if (m_mapNodes.find(posx) != m_mapNodes.end())
	{
		if (m_mapNodes.at(posx).find(posy) != m_mapNodes.at(posx).end())
		{
			node = m_mapNodes.at(posx).at(posy);
		}
	}
	return node;
}

ASNode * AStar::SetOpenNewNode(ASNode * node)
{
	int posy = node->m_posY;
	int posx = node->m_posX;
	FindCount = 0;
	if (AddToOpenList(posx, posy + 1, node) != nullptr) return node;
	if (AddToOpenList(posx, posy - 1, node) != nullptr) return node;
	if (AddToOpenList(posx + 1, posy, node) != nullptr) return node;
	if (AddToOpenList(posx - 1, posy, node) != nullptr) return node;
	if (isEight == true)
	{
		if (AddToOpenList(posx + 1, posy + 1, node) != nullptr) return node;
		if (AddToOpenList(posx + 1, posy - 1, node) != nullptr) return node;
		if (AddToOpenList(posx - 1, posy + 1, node) != nullptr) return node;
		if (AddToOpenList(posx - 1, posy - 1, node) != nullptr) return node;
	}
	if (FindCount == 0)
	{
		//openList.erase(openList.find(node));
		node->IsLock(true);
	}
	return nullptr;
}

ASNode * AStar::AddToOpenList(int posx,int posy,ASNode * node)
{
	ASNode * curNode = GetNodeWithPos(posx, posy);
	if (curNode == end_node) return node;//找到了终点
	if (curNode == nullptr || curNode->m_isLock ==  true) return nullptr;

	if (curNode->IsTheSameType(curCanNotMoveTypes) == true){
		curNode->IsLock(true);
		return nullptr;
	}
	curNode->RefreshFGH(node, end_node);
	//--说明已经存在于其中了
	if (openList.find(curNode) != openList.end())
	{
		//--比较2个父节点的F值谁小点
		if (curNode->m_fatherNode != nullptr)
		{
			if (curNode != node->m_fatherNode && curNode->m_fatherNode->m_F > node->m_F)
			{
				curNode->m_fatherNode = node;
			}
		}
		else//没有父节点，说明。。。
		{
			curNode->m_fatherNode = node;
		}
	}
	else
	{
		openList.insert(std::make_pair(curNode, true));
		curNode->m_fatherNode = node;
	}
	return nullptr;
}

ASNode * AStar::GetFindFminNode()
{
	ASNode * minNode = nullptr;
	for (auto & node : openList)
	{
		if (minNode == nullptr && node.first->m_isLock == false)
		{
			minNode = node.first;
		}
		else{
			if (node.first->m_isLock == false)
			minNode = (minNode->m_F < node.first->m_F) ? minNode : node.first;
		}
	}
	return minNode;
}