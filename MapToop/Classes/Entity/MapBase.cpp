#include "MapBase.h"
#include <fstream>
#include "Tool/StringHelper.h"
#include "Controller/Controller_Point.h"
#include "Block.h"
#include "Astar.h"
#include "RoleModel.h"
#include "FileData/FileData_Map.h"
MapBase::MapBase()
{
	m_Blocks.clear();
	m_CurScale = 1.0;
	m_AStar = new AStar();
	m_LockPlayerId = PLAYER_1_UID;
}

MapBase::~MapBase()
{
	CC_SAFE_DELETE(m_AStar);
}

bool MapBase::init()
{
	if (!Layout::init())
	{
		return false;
	}
	InitMapWidget();

	auto m_ConPoint = Controller_Point::create();
	m_ConPoint->OpenTouch();
	m_ConPoint->RegisterCallback(CallFunc_ButtonTypeAndPoint_SELECTOR(MapBase::TouchEvent), this);
	this->addChild(m_ConPoint, 99);
	this->scheduleUpdate();
	return true;
}
//public:
void MapBase::ShowBlock()
{
	HiedBlock();
	m_LayoutBlock->removeAllChildren();
	m_Blocks.clear();
	for (int posY = 0; posY < m_BlockData.size(); posY++)
	{
		for (int posX = 0; posX < m_BlockData.at(posY).size(); posX++)
		{
			auto block = Block::CreateWithInfo(Size(64, 64), (BlockType)(m_BlockData.at(posY).at(posX)));
			m_LayoutBlock->addChild(block);
			block->SetPos(PosInt(posX, posY));
			block->SetPosVisi(true);
			if (m_Blocks.find(posY) == m_Blocks.end())
			{
				map<int, Block *> temp;
				temp.clear();
				temp.insert(make_pair(posX, block));
				m_Blocks.insert(make_pair(posY, temp));
			}
			else
			{
				m_Blocks.at(posY).insert(make_pair(posX, block));
			}
		}
	}
}
void MapBase::HiedBlock()
{
	m_LayoutBlock->removeAllChildren();
	m_Blocks.clear();
}

Vec2 MapBase::GetPos(int posx, int posy)
{
	return Vec2(__BlockSize * (posx + 0.5), __BlockSize * posy);
}
int  MapBase::GetPosType(PosInt pos)
{
	if (m_BlockData.find(pos.getY()) != m_BlockData.end())
	{
		map<int, int> data = m_BlockData.at(pos.getY());
		if (data.find(pos.getX()) != data.end())
		{
			return data.at(pos.getX());
		}
	}
	return -1;
}

RoleModel * MapBase::GetRoleById(int roleId)
{
	if (m_Roles.find(roleId) != m_Roles.end())
	{
		return m_Roles.at(roleId);
	}
	else
	{
		return nullptr;
	}
}

RoleModel * MapBase::GetRoleByPos(PosInt pos)
{
	for (auto & roleModel : m_Roles)
	{
		if (PosInt::IsSamePosInt(pos, roleModel.second->GetPos()) == true)
		{
			return roleModel.second;
		}
	}
	return nullptr;
}
RoleModel * MapBase::AddPlayer(int roleId)
{
	RoleModel * rm = nullptr;
	if (m_MapView != nullptr && m_LayoutSpirte != nullptr)
	{
		rm = GetRoleById(roleId);
		if (rm == nullptr)
		{
			rm = RoleModel::Create(roleId);
			rm->SetMapDelegate(this);
			rm->SetPos(0, 0);
			m_LayoutSpirte->addChild(rm);
			m_Roles.insert(std::make_pair(roleId, rm));
		}
	}
	return rm;
}

void MapBase::SetLockPlayer(int roleId)
{
	if (GetRoleById(roleId) != nullptr)
	{
		m_LockPlayerId = roleId;
	}
}
//----
#define  InitLayoutForMap(who,zder,father)\
	who = Layout::create(); \
	who->setLocalZOrder(zder++); \
	father->addChild(who);

void MapBase::InitMapWidget()
{
	int zOrder = 1;
	InitLayoutForMap(m_LayoutGround, zOrder,this)
	InitLayoutForMap(m_LayoutBlock, zOrder, this)
	InitLayoutForMap(m_LayoutSpirte, zOrder, this)
	InitLayoutForMap(m_LayoutSky, zOrder, this)
}

void MapBase::SetMap(int mapId)
{
	m_MapId = mapId;
}

void MapBase::ClearMapViewData()
{
	m_LayoutGround->removeAllChildren();
	m_LayoutSpirte->removeAllChildren();
	m_LayoutSky->removeAllChildren();
	m_Blocks.clear();
	m_MapView = nullptr;
}

void MapBase::RefreshMap()
{
	CCLOG("[RefreshMap]:Waring !!");
	string mapDataFile = GetMapDataFile();
	string mapViewFile = GetMapViewFile();
	m_AStar->ClearMapDate();
	if (mapViewFile != "" && mapDataFile != "")
	{
		
		ClearMapViewData();
		LoadMapData(mapDataFile);
		m_MapView = LoadMap(mapViewFile);
		if (m_MapView != nullptr)
		{
			m_LayoutGround->addChild(m_MapView);
			LoadRoleModel();
			//--初始化寻路可能会用到的数据
			for (int posY = 0; posY < m_BlockData.size(); posY++)
			{
				for (int posX = 0; posX < m_BlockData.at(posY).size(); posX++)
				{
					vector<int> types;
					types.clear();
					types.push_back(m_BlockData.at(posY).at(posX));
					if (m_ASTARData.find(posY) == m_ASTARData.end())
					{
						map<int, vector<int>> Atemp;
						Atemp.insert(make_pair(posX, types));
						m_ASTARData.insert(make_pair(posY, Atemp));
					}
					else
					{
						m_ASTARData.at(posY).insert(make_pair(posX, types));
					}
				}
			}//--初始化寻路可能会用到的数据end
		}
		m_AStar->SetMapData(m_ASTARData);
	}
}
void MapBase::LockPlayer()
{
	if (m_Roles.find(m_LockPlayerId) != m_Roles.end())
	{
		RoleModel * player = m_Roles.at(m_LockPlayerId);
		Vec2 vec2_player = player->getPosition();
		Vec2 vec2_layout = m_LayoutGround->getPosition();
		Vec2 vec2_target = __VisibleSize / 2;
		Vec2 dis_vec = vec2_player - vec2_target;
		if (vec2_target.x / 2 < dis_vec.y || true)
		{
			if (dis_vec.y >= (m_MapSize.height - vec2_target.y)){
				m_LayoutGround->setPositionY(m_MapSize.height - vec2_target.y);
			}
			else if (dis_vec.y <= 0)
			{
				m_LayoutGround->setPositionY(0);
			}
			else
			{
				m_LayoutGround->setPositionY(-dis_vec.y);
			}
		}
		if (vec2_target.x < dis_vec.x || true)
		{
			if (dis_vec.x >= (m_MapSize.width - vec2_target.x)){
				m_LayoutGround->setPositionX(m_MapSize.width - vec2_target.x);
			}
			else if (dis_vec.x <= 0)
			{
				m_LayoutGround->setPositionX(0);
			}
			else
			{
				m_LayoutGround->setPositionX(-dis_vec.x);
			}

		}
		SetPostionMapMove(m_LayoutGround->getPosition());
	}
}
//private
std::string MapBase::GetMapDataFile()
{
	const CfgMap * cfg = FileDataMap::GetInstance()->GetCfg(m_MapId);
	if (cfg != nullptr)
	{
		return cfg->MapDataPath;
	}
	return "";
}
std::string MapBase::GetMapViewFile()
{
	const CfgMap * cfg = FileDataMap::GetInstance()->GetCfg(m_MapId);
	if (cfg != nullptr)
	{
		return cfg->MapSrcPath;
	}
	return "";
}
bool MapBase::LoadMapData(string mapDataFile)
{
	m_BlockData.clear();
	m_Element.clear();
	vector<std::string> vect_string;
	string file_path = mapDataFile;
	ifstream OpenFile(file_path);
	if (OpenFile.fail())
	{
		return false;
	}
	else{
		CCLOG("[LoadMapData]:open [%s] is success", file_path.c_str());
	}
	char buf[1000];
	do{
		OpenFile.getline(buf, 1000);
		string FileString;
		FileString.append(buf);
		vect_string.push_back(FileString);
		if (OpenFile.eof())
			break;
	} while (!OpenFile.eof());
	//---
	bool isBlock = true;
	int block_y = 0;
	for (auto data : vect_string)
	{
		if (strcmp(__splitEAndB, data.c_str()) == 0){ isBlock = false; continue; }
		if (isBlock)
		{
			vector<string> block_str = StringHelper::splitString(data, ",");
			map<int, int> temp;
			for (size_t i = 0; i < block_str.size(); i++)
			{
				temp.insert(std::make_pair(i, atoi(block_str[i].c_str())));
			}
			m_BlockData.insert(std::make_pair(block_y, temp));
			block_y++;
		}
		else{
			vector<string> ele_str = StringHelper::splitString(data, "#");
			for (auto ele_data_ste : ele_str)
			{
				vector<string> ele_data = StringHelper::splitString(ele_data_ste, ",");
				OhterElement o_e;
				o_e.e_id = (int)atoi(ele_data.at(2).c_str());
				o_e.e_pos.setX((int)atoi(ele_data.at(0).c_str()));
				o_e.e_pos.setY((int)atoi(ele_data.at(1).c_str()));
				m_Element.push_back(OhterElement(&o_e));
			}
		}
	}
	return true;
}

Layout * MapBase::LoadMap(string mapViewFile)
{
	Layout * mapBack = (Layout *)CSLoader::createNode(mapViewFile);
	m_MapSize = mapBack->getContentSize();
	return mapBack;
}

void MapBase::LoadRoleModel()
{
	for (auto & ele : m_Element)
	{
		RoleModel * rm = AddPlayer(ele.e_id);
		if (rm != nullptr)
		{
			rm->SetPos(ele.e_pos.getX(), ele.e_pos.getY());
			rm->UpdateBehaviour();
		}
	}
}
void MapBase::TouchEvent(ui::Widget::TouchEventType touch_type, Vec2 touch_point)
{
	if (/*m_IsEditModel == */true)
	{
		switch (touch_type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			//CCLOG("[ControlTouchType_Began ] TPOINT[%0.2f : %0.2f]", touch_point.x, touch_point.y);
			//m_CurPoint = touch_point;
			if (m_AStar != nullptr)
			{
				auto move_point = m_LayoutGround->getPosition();
				int  wdith_num = (int)(touch_point.x - move_point.x) / (__BlockSize*m_CurScale);
				int  height_num = (int)(touch_point.y - move_point.y) / (__BlockSize*m_CurScale);
				
				TouchBlockPos(wdith_num, height_num);
			}
			break;
		default:
			break;
		}
	}
}
void MapBase::TouchBlockPos(int posx, int posy)
{
	int ty = GetPosType(PosInt(posx, posy));
	CCLOG("Touch type:%d",ty);
}
void MapBase::SetPostionMapMove(Vec2 postion)
{
	m_LayoutGround->setPosition(postion);
	m_LayoutBlock->setPosition(postion);
	m_LayoutSpirte->setPosition(postion);
}
void MapBase::update(float delta)
{
	LockPlayer();
}
