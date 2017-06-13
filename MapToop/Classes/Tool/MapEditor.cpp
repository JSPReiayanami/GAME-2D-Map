#include "MapEditor.h"
#include "Tool/StringHelper.h"
#include "Entity/EffectSprite.h"
#include "Entity/Effect.h"
#include "Entity/Block.h"
#include "Entity/Slots.h"
#include "Controller/Controller_Point.h"
#include <fstream>
#include "Entity/RoleModel.h"
#define __OpenFunction_1 true//false // 移动到的块会有动画效果功能
#define __OpenFunction_2 true// 按F2隐藏块层
#define __OpenFunction_3 true//按F3隐藏普通块
#define __OpenFunction_4 true// 地图选择功能
#define __TestAStarFunction true //开启测试A星寻路功能
#include "Entity/Astar.h"
#include "FileData/FileData_Npc.h"
//===[


//===]
MapEditor::MapEditor():
m_CurMapId(0), m_BlockSize(__BlockSize, __BlockSize), m_MapSize(0, 0), m_MapInt(0, 0),
m_CurPointBlock(nullptr), m_CurMapName("")
{
	m_BlockData.clear();
	m_Element.clear();
	m_ConPoint = nullptr;
	m_CurPoint = Vec2(0, 0);
	m_LastPoint = Vec2(0, 0);
	m_IsMove = false;
	m_IsPoint = false;
	m_PosIsVisi = false;
	m_CurScale = 1.0;
	m_IsBlockVisi = true;
	m_IsCanPoint = true;
	m_RoleModels.clear();
	m_EditModelType = ET_NOType;
	m_CurRoleModel = nullptr;
}

MapEditor::~MapEditor()
{
	delete m_AStar;
}

bool MapEditor::init(){
	if (!Layout::init())
	{
		return false;
	}
	m_RootWidget = (Layout *)CSLoader::createNode("Interface_MapTool.csb");
	this->addChild(m_RootWidget);
	InitWidget();
	InitEvent();
	ShowChoiceMap();
	return true;
}

void MapEditor::ShowChoiceMap(){
	if (m_CurPointBlock != nullptr)
	{
		m_CurPointBlock->IsPoint(false);
	}
	m_CurPointBlock = nullptr;
	m_IsCanPoint = false;
	m_LayoutMapChoice->setVisible(true);
	m_LayoutChoice->setVisible(false);
	m_ListMaps->removeAllItems();
	InitCanEditorMapChoice();
}
void MapEditor::CloseChoiceMap()
{
	m_LayoutMapChoice->setVisible(false);
	m_LayoutChoice->setVisible(true);
	m_ListMaps->removeAllItems();
}
void MapEditor::InitCanEditorMapChoice(){
	int index_count = 1;
	Size lay_size = Size(m_ListMaps->getContentSize().width,200);
	vector<string> allFile;
	StringHelper::getAllFileInPathWithType("./Map", allFile, "csb");
	for (auto & mapNa : allFile)
	{
		Layout * map_root = (Layout *)CSLoader::createNode(StringUtils::format("map/%s", mapNa.c_str()));
		if (map_root == nullptr) continue;
		Layout * lay = Layout::create();
		lay->setContentSize(lay_size);
		map_root->setScaleX(1 / (map_root->getContentSize().width / lay_size.width));
		map_root->setScaleY(1 / (map_root->getContentSize().height / lay_size.height));
		

		Button * btn = Button::create("Map/ImageFile.png");
		btn->setName(StringHelper::deleteStrTypeBack(mapNa,"."));
		btn->setContentSize(lay_size);
		btn->setScale9Enabled(true);
		btn->setPosition(btn->getContentSize() / 2);
		btn->setOpacity(0);
		lay->addChild(map_root, 4);
		btn->addTouchEventListener(CC_CALLBACK_2(MapEditor::ChoiceMapKindButtonEvent,this));
		btn->setTag(index_count);
		lay->addChild(btn,5);
		m_ListMaps->pushBackCustomItem(lay);
		index_count++;
	}
}
void MapEditor::InitEditorMap()
{
	Widget * mapBack = (Layout *)CSLoader::createNode(StringUtils::format(__MapPngName,m_CurMapName.c_str()));
	if (mapBack != nullptr)
	{
		m_LayoutBack->addChild(mapBack);
		m_MapSize = mapBack->getContentSize();
		m_MapInt.setX((int)ceil(m_MapSize.width / m_BlockSize.width));
		m_MapInt.setY((int)ceil(m_MapSize.height / m_BlockSize.height));
	}
}

void MapEditor::InitMapBlock()
{
	m_LayoutBlockPoint->removeAllChildren();
	m_Blocks.clear();
	m_ASTARData.clear();
	for (int posY = 0; posY < m_BlockData.size(); posY++)
	{
		for (int posX = 0; posX< m_BlockData.at(posY).size(); posX++)
		{
			auto block = Block::CreateWithInfo(Size(__BlockSize, __BlockSize), (BlockType)(m_BlockData.at(posY).at(posX)));
			m_LayoutBlockPoint->addChild(block);
			block->SetPos(PosInt(posX,posY));
			vector<int> types;
			types.clear();
			types.push_back(m_BlockData.at(posY).at(posX));
			if (m_Blocks.find(posY) == m_Blocks.end())
			{
				map<int, Block *> temp;
				temp.clear();
				temp.insert(make_pair(posX, block));
				m_Blocks.insert(make_pair(posY, temp));
				map<int, vector<int>> Atemp;
				Atemp.insert(make_pair(posX, types));
				m_ASTARData.insert(make_pair(posY, Atemp));
			}
			else
			{
				m_Blocks.at(posY).insert(make_pair(posX, block));
				m_ASTARData.at(posY).insert(make_pair(posX, types));
			}
		}
	}
}
void MapEditor::InitMapOhterElement()
{
	for (auto & ele : m_Element)
	{
		RoleModel * rm = AddRoleModel(ele.e_id);
		if (rm != nullptr)
		{
			rm->SetPosForEditor(ele.e_pos.getX(), ele.e_pos.getY());
			Vec2 poss = Vec2(__BlockSize * (ele.e_pos.getX() + 0.5), __BlockSize * ele.e_pos.getY());
			rm->setPosition(poss);
		}
	}
}

void MapEditor::LoadMapData()
{
	m_BlockData.clear();
	m_Element.clear();
	vector<std::string> vect_string;
	string file_path = StringUtils::format(__MapDataName, m_CurMapName.c_str());
	ifstream OpenFile(file_path);
	if (OpenFile.fail())
	{
		CCLOG("[LoadMapData]:open [%s] is error", file_path.c_str());
		CCLOG("[LoadMapData]:create new [%s] ", file_path.c_str());
		for (int index_y = 0; index_y < m_MapInt.getY(); index_y++)
		{
			map<int, int> temp;
			for (int index_x = 0; index_x < m_MapInt.getX(); index_x++)
			{
				temp.insert(std::make_pair(index_x, 0));
			}
			m_BlockData.insert(std::make_pair(index_y, temp));
		}
		SaveMapData();
		return;
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
		if (strcmp(__splitEAndB, data.c_str()) == 0){ isBlock = false; continue;}
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
}
void MapEditor::SaveMapData()
{
	string file_path = StringUtils::format(__MapDataName, m_CurMapName.c_str());
	ofstream myfile(file_path, ios::out);
	if (!myfile)
	{
		CCLOG("[SaveMapData]:save [%s] is error", file_path.c_str());
	}
	else
	{

		for (int posY = 0; posY < m_BlockData.size(); posY++)
		{
			string one_line_data = "";
			for (int posX = 0; posX < m_BlockData.at(posY).size(); posX++)
			{
				int typeNum = 0;
				if (m_Blocks.find(posY) != m_Blocks.end())
				{
					if (m_Blocks.at(posY).find(posX) != m_Blocks.at(posY).end())
					{
						typeNum = m_Blocks.at(posY).at(posX)->m_Type;
					}

				}
				one_line_data.append(StringUtils::format(__BlockPosSave, typeNum));
			}
			myfile << one_line_data << endl;
		}
// 		for (auto & block_x : m_BlockData)
// 		{
// 			string one_line_data = "";
// 			for (auto & block_y : block_x.second)
// 			{
// 				int typeNum = 0;
// 				if (m_Blocks.find(block_x.first) != m_Blocks.end())
// 				{
// 					if (m_Blocks.at(block_x.first).find(block_y.first) != m_Blocks.at(block_x.first).end())
// 					{
// 						typeNum = m_Blocks.at(block_x.first).at(block_y.first)->m_Type;
// 					}
// 					
// 				}
// 				one_line_data.append(StringUtils::format(__BlockPosSave,typeNum));
// 			}
// 			myfile << one_line_data << endl;
// 		}
		myfile << __splitEAndB << endl;
		int count_line = 0;
		string one_line_data = "";
		if (m_CurRoleModel != nullptr){
			RemoveRoleModel(m_CurRoleModel->GetId());
		}
		for (auto & ele : m_RoleModels)
		{
			PosInt e_pos = ele.second->GetPos();
			one_line_data.append(StringUtils::format(__ElementSave, e_pos.getX(), e_pos.getY(), ele.second->GetId()));
			if (count_line >= __ElementDataOneLineNum)
			{
				myfile << one_line_data << endl;
				one_line_data = "";
			}
		}
		if (one_line_data.empty() == false) myfile << one_line_data << endl;

		myfile.close();
		CCLOG("[SaveMapData]:save [%s] is success", file_path.c_str());
	}
}
void MapEditor::RefreshEditorMap()
{
	m_LayoutBack->removeAllChildren();
	m_LayoutBack->setPosition(Vec2(0,0));
	m_LayoutBlock->removeAllChildren();
	m_LayoutBlock->setPosition(Vec2(0, 0));
	m_LayoutBlockPoint->removeAllChildren();
	m_LayoutBlockPoint->setPosition(Vec2(0, 0));
	InitEditorMap();
	LoadMapData();
	InitMapBlock();
	InitMapOhterElement();
	m_AStar->SetMapData(m_ASTARData);
}
void MapEditor::SetScaleMap(float sc)
{
	m_LayoutBack->setScale(sc);
	m_LayoutBlockPoint->setScale(sc);
	m_LayoutBlock->setScale(sc);;
}
void MapEditor::ShowNpcIdText()
{
	m_CurRoleModel = nullptr;
	m_TextFiledNpcId->setVisible(true);
	_SetButtonVisible_(m_LayoutBType, "Button_AddNpc", true);
	_SetButtonVisible_(m_LayoutBType, "Button_ReNpcId", true);
	_SetButtonVisible_(m_LayoutBType, "Button_ExitNpc", true);
	_SetButtonVisible_(m_LayoutBType, "Button_NpcId", false);
	for (int i = 1; i <= 3; i++)
	{
		_SetButtonVisible_(m_LayoutBType, StringUtils::format("Button_Type%d", i), false);
	}
	m_ListRoleModel->setVisible(true);
	//--
	m_ListRoleModel->removeAllItems();
	auto allCfg = FileDataNpc::GetInstance()->GetAllCfg();
	for (auto & cfg : *allCfg)
	{
		Layout * lay = Layout::create();
		lay->setContentSize(Size(__BlockSize,80));
		lay->setColor(Color3B::RED);
		lay->setBackGroundColor(Color3B::YELLOW);
		lay->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
		RoleModel * rm = RoleModel::Create(cfg.second.Id);
		rm->setPositionX(__BlockSize / 2);
		lay->addChild(rm);
		Button * btn = Button::create("ImageFile.png");
		btn->setScale9Enabled(true);
		btn->setContentSize(lay->getContentSize());
		btn->setPosition(Size(__BlockSize, 80) / 2);
		btn->setOpacity(0);
		btn->setTag(cfg.second.Id);
		btn->addTouchEventListener(CC_CALLBACK_2(MapEditor::NpcChoiceButtonEvent,this));
		lay->addChild(btn,99);
		Text * text = Text::create();
		text->setString(cfg.second.Name);
		text->setTextHorizontalAlignment(TextHAlignment::CENTER);
		text->setTextAreaSize(Size(__BlockSize,20));
		text->setAnchorPoint(Vec2::ZERO);
		lay->addChild(text);
		m_ListRoleModel->pushBackCustomItem(lay);
	}
}
void MapEditor::CloseNpcIdText()
{
	m_CurRoleModel = nullptr;
	m_TextFiledNpcId->setVisible(false);
	_SetButtonVisible_(m_LayoutBType, "Button_AddNpc", false);
	_SetButtonVisible_(m_LayoutBType, "Button_ReNpcId", false);
	_SetButtonVisible_(m_LayoutBType, "Button_ExitNpc", false);
	_SetButtonVisible_(m_LayoutBType, "Button_NpcId", true);
	for (int i = 1; i <= 3; i++)
	{
		_SetButtonVisible_(m_LayoutBType, StringUtils::format("Button_Type%d", i), true);
	}
	m_ListRoleModel->setVisible(false);
	m_ListRoleModel->removeAllItems();
}
RoleModel * MapEditor::AddRoleModel(int NpcId)
{
	RoleModel * rm = nullptr;
	if (m_RoleModels.find(NpcId) != m_RoleModels.end())
	{
		rm = m_RoleModels.at(NpcId);
	}

	if (rm == nullptr && FileDataNpc::GetInstance()->GetCfg(NpcId) != nullptr)
	{
		rm = RoleModel::Create(NpcId);
		m_LayoutRole->addChild(rm);
		m_RoleModels.insert(std::make_pair(NpcId, rm));
	}
	return rm;
}
void MapEditor::RemoveRoleModel(int NpcId)
{
	if (m_RoleModels.find(NpcId) != m_RoleModels.end())
	{
		RoleModel * rm = m_RoleModels.at(NpcId);
		rm->removeFromParent();
		m_RoleModels.erase(m_RoleModels.find(NpcId));
	}
	else
	{
		if (m_CurRoleModel != nullptr)
		{
			RemoveRoleModel(m_CurRoleModel->GetId());
		}
	}


}

RoleModel * MapEditor::GetRoleModelWithPos(PosInt pos)
{
	for (auto & rm : m_RoleModels)
	{
		if (PosInt::IsSamePosInt(pos, rm.second->GetPos()))
		{
			return rm.second;
		}
	}
	return nullptr;
}

PosInt MapEditor::GetCurMousePos()
{
	auto move_point = m_LayoutBack->getPosition();
	int posx = floor((m_CurMousePoint.x - move_point.x) / (__BlockSize*m_CurScale));
	int posy = floor((m_CurMousePoint.y - move_point.y) / (__BlockSize*m_CurScale));
	return PosInt(posx, posy);
}
void MapEditor::NpcModelFollowMouseMove()
{
	if (m_EditModelType == EditorType::ET_NpcType && m_CurRoleModel != nullptr)
	{
		auto move_point = m_LayoutRole->getPosition();
		float posx = (m_CurMousePoint.x - move_point.x) / m_CurScale;
		float posy = (m_CurMousePoint.y - move_point.y) / m_CurScale;
		m_CurRoleModel->setPosition(Vec2(posx, posy - m_CurRoleModel->getContentSize().height / 4));
	}
}

void MapEditor::MapLayoutSetPostion(Vec2 postions)
{
	//m_LayoutBack->setPosition(m_LayoutBack->getPosition() + m_CurPoint - m_LastPoint);
	//m_LayoutBlockPoint->setPosition(m_LayoutBlockPoint->getPosition() + m_CurPoint - m_LastPoint);
	//m_LayoutRole->setPosition(m_LayoutRole->getPosition() + m_CurPoint - m_LastPoint);
	m_LayoutBack->setPosition(postions);
	m_LayoutBlockPoint->setPosition(postions);
	m_LayoutRole->setPosition(postions);
}

void MapEditor::MapLayoutSetScale(float scaleNum)
{
	m_LayoutBack->setScale(scaleNum);
	m_LayoutBlockPoint->setScale(scaleNum);
	m_LayoutRole->setScale(scaleNum);
}

void MapEditor::InitWidget()
{
	__GetChildAndAssignmentWithOB(m_LayoutBack, m_RootWidget, "Panel_Background", Layout);
	__GetChildAndAssignmentWithOB(m_LayoutBlock, m_RootWidget, "Panel_Block", Layout);
	__GetChildAndAssignmentWithOB(m_LayoutBType, m_RootWidget, "Panel_BlockType", Layout);
	__GetChildAndAssignmentWithOB(m_LayoutBlockPoint, m_RootWidget, "Panel_BlockPoint", Layout);
	__GetChildAndAssignmentWithOB(m_LayoutChoice, m_RootWidget, "Panel_Button", Layout);
	__GetChildAndAssignmentWithOB(m_LayoutRole, m_RootWidget, "Panel_Role", Layout);
	__GetChildAndAssignmentWithOB(m_LayoutMapChoice, m_RootWidget, "Panel_MapChoice", Layout);
	__GetChildAndAssignmentWithOB(m_ListMaps, m_LayoutMapChoice, "ListView_Map", ListView);
	__GetChildAndAssignmentWithOB(m_ButtonChoice, m_RootWidget, "Button_MapChoice", Button);
	__GetChildAndAssignmentWithOB(m_TextFiledNpcId, m_LayoutBType, "TextField_NpcId",TextField);
	__GetChildAndAssignmentWithOB(m_ListRoleModel, m_LayoutBType, "ListView_NpcModel", ListView);
	

// 	m_LayoutBType->setVisible(false);
// 	m_ButtonChoice->setVisible(true);
// 	auto e_sp2 = EffectSprite::create("1.png");
// 	auto e_sp = EffectSprite::create("WaterShader/water.png");
//  auto e_sp = EffectSprite::create("icon.png");
// 	e_sp->setPosition(__VisibleSize / 2);
// 	e_sp2->setPosition(__VisibleSize / 2);
// 	m_LayoutBlockPoint->addChild(e_sp);
// 	e_sp->setEffect(EffectWater::create());
// 	e_sp2->setEffect(EffectOutline::create());
// 	m_LayoutBlockPoint->addChild(e_sp2,999);
	m_AStar = new AStar();
}
void MapEditor::InitEvent(){
	__AddTouchEventWithNameAndPNode("Button_MapChoice", m_LayoutChoice, MapEditor::ChoiceMapButtonEvent);
	__AddTouchEventWithNameAndPNode("Button_Close", m_LayoutMapChoice, MapEditor::CloseMapChoiceButtonEvent);
	__AddTouchEventWithNameAndPNode("Button_EdietS", m_LayoutChoice, MapEditor::EditButtonEvent);
	__AddTouchEventWithNameAndPNode("Button_Save", m_LayoutChoice, MapEditor::SaveButtonEvent);
	__AddTouchEventWithNameAndPNode("Button_Close", m_LayoutBType, MapEditor::CloseEditButtonEvent);
	__AddTouchEventWithNameAndPNode("Button_Close", m_LayoutMapChoice, MapEditor::CloseMapChoiceButtonEvent);
	for (int i = 1; i <= 3; i++)
	{
		__AddTouchEventWithNameAndPNode(StringUtils::format("Button_Type%d", i), m_LayoutBType, MapEditor::TypeButtonEvent);
	}
	__AddTouchEventWithNameAndPNode("Button_ExitNpc", m_LayoutBType, MapEditor::NpcButtonEvent);
	__AddTouchEventWithNameAndPNode("Button_NpcId", m_LayoutBType, MapEditor::NpcButtonEvent);
	__AddTouchEventWithNameAndPNode("Button_ReNpcId", m_LayoutBType, MapEditor::NpcButtonEvent);
	__AddTouchEventWithNameAndPNode("Button_AddNpc", m_LayoutBType, MapEditor::NpcButtonEvent);
	m_LayoutBType->setVisible(false);
	m_LayoutMapChoice->setVisible(false);
	m_ConPoint = Controller_Point::create();
	m_ConPoint->OpenTouch();
	m_ConPoint->RegisterCallback(CallFunc_ButtonTypeAndPoint_SELECTOR(MapEditor::TouchEvent), this);
	this->addChild(m_ConPoint, 99);

	m_TextFiledNpcId->addEventListener(CC_CALLBACK_2(MapEditor::NpcIdTextFiledEvent,this));
	// 创建鼠标监听
	auto listener = EventListenerMouse::create();
	//分发MouseMove事件
	listener->onMouseMove = CC_CALLBACK_1(MapEditor::MouseMove, this);
	listener->onMouseScroll = CC_CALLBACK_1(MapEditor::MouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//注册捕捉监听
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(MapEditor::OnKeyReleased, this);
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(MapEditor::OnKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}

void MapEditor::PointBlock()
{

	Block * get_block = GetCurPointBlock();
	if (m_CurPointBlock != get_block)
	{
		if (nullptr != m_CurPointBlock)
		{
			m_CurPointBlock->IsPoint(false);
			//m_CurPointBlock->stopAllActions();
			//m_CurPointBlock->setScale(1);
			m_CurPointBlock = nullptr;
		}

		if (nullptr != get_block)
		{
			m_CurPointBlock = get_block;
			m_CurPointBlock->IsPoint(true);
			//float s_time = 0.4;
			//m_CurPointBlock = get_block;
			//m_CurPointBlock->setScale(0.8);
			//auto seq = Sequence::create(ScaleTo::create(s_time, 0.8), ScaleTo::create(s_time, 0.5), NULL);
			//m_CurPointBlock->runAction(RepeatForever::create(seq));
		}
	}


}


Block * MapEditor::GetCurPointBlock()
{
	Block * point_block = nullptr;
	auto move_point = m_LayoutBack->getPosition();
	int height_num = (int)(m_CurMousePoint.x - move_point.x) / (__BlockSize*m_CurScale);
	int wdith_num = (int)(m_CurMousePoint.y - move_point.y) / (__BlockSize*m_CurScale);
	if (m_Blocks.find(wdith_num) != m_Blocks.end())
	{
		if (m_Blocks.at(wdith_num).find(height_num) != m_Blocks.at(wdith_num).end())
		{
			point_block = m_Blocks.at(wdith_num).at(height_num);
		}

	}
	return point_block;
}

void  MapEditor::EditButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		m_LayoutBType->setVisible(true);
		m_LayoutChoice->setVisible(false);
		CloseNpcIdText();
		m_CurType = 0;
		m_IsEditModel = true;
		m_EditModelType = EditorType::ET_BlockType;
	}
}

void MapEditor::ChoiceMapButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		ShowChoiceMap();
	}
}
void MapEditor::ChoiceMapKindButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		Button * btn = (Button *)pSender;
		m_CurMapName = btn->getName();
		CloseChoiceMap();
		RefreshEditorMap();
		m_IsCanPoint = true;
	}
}

void MapEditor::CloseMapChoiceButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		CloseChoiceMap();
	}
}

void MapEditor::NpcButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	Button * btn = (Button *)pSender;
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		if (btn->getName() == "Button_NpcId")
		{
			ShowNpcIdText();
			m_EditModelType = EditorType::ET_NpcType;
		}
		else if (btn->getName() == "Button_ExitNpc")
		{
			CloseNpcIdText();
			m_EditModelType = EditorType::ET_BlockType;
		}
		else if (btn->getName() == "Button_AddNpc")
		{
			int npcId = atoi(m_TextFiledNpcId->getString().c_str());
			m_CurRoleModel = AddRoleModel(npcId);
		}
		else if (btn->getName() == "Button_ReNpcId")
		{
			int npcId = atoi(m_TextFiledNpcId->getString().c_str());
			RemoveRoleModel(npcId);
			m_CurRoleModel = nullptr;
		}
		
	}
}

void MapEditor::NpcIdTextFiledEvent(Ref * pSender, ui::TextField::EventType eventType)
{
	TextField * tf = (TextField *)pSender;
	switch (eventType)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
		CCLOG("[NpcIdTextFiledEvent]=[ATTACH_WITH_IME]:1");
		break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
		CCLOG("[NpcIdTextFiledEvent]=[DETACH_WITH_IME]:2");
		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		CCLOG("[NpcIdTextFiledEvent]=[INSERT_TEXT]:3<%s>", tf->getString().c_str());
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
		CCLOG("[NpcIdTextFiledEvent]=[DELETE_BACKWARD]:4");
		break;
	default:
		break;
	}
}

void MapEditor::NpcChoiceButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	Button * btn = (Button *)pSender;
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		if (m_CurRoleModel != nullptr)
		{
			RemoveRoleModel(m_CurRoleModel->GetId());
			m_CurRoleModel = nullptr;
		}
		m_CurRoleModel = AddRoleModel(btn->getTag());
	}
}
void  MapEditor::SaveButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		SaveMapData();
	}
}
void  MapEditor::TypeButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		auto btn = (Button *)pSender;
		m_CurType = btn->getTag();
		m_IsEditModel = true;
		m_EditModelType = EditorType::ET_BlockType;
	}
	else if (eventType == ui::Widget::TouchEventType::BEGAN)
	{
		m_IsEditModel = false;
		m_EditModelType = EditorType::ET_NOType;
	}
	else if (eventType == ui::Widget::TouchEventType::CANCELED)
	{
		m_IsEditModel = true;
		m_EditModelType = EditorType::ET_BlockType;
	}
}
void  MapEditor::CloseEditButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		CloseNpcIdText();
		m_LayoutBType->setVisible(false);
		m_LayoutChoice->setVisible(true);
	}
	else if (eventType == ui::Widget::TouchEventType::BEGAN)
	{
		m_IsEditModel = false;
		m_EditModelType = EditorType::ET_NOType;
	}
	else if (eventType == ui::Widget::TouchEventType::CANCELED)
	{
		m_IsEditModel = true;
		m_EditModelType = EditorType::ET_BlockType;
	}
}
//=========
void MapEditor::MouseMove(Event * evnet)
{
	//CCLog("mouse  move!!!!!!!!!!!!!!!!!!!!!!");
	EventMouse* e = (EventMouse*)evnet;
	//CCLOG("%0.2f %0.2f",e->getCursorX() ,e->getCursorY());//
	float x = e->getCursorX();
	float y = e->getCursorY();
	m_CurMousePoint.x = x;
	m_CurMousePoint.y = y;
	if (m_IsBoolPressed == true && m_EditModelType == EditorType::ET_BlockType)
	{
		auto move_point = m_LayoutBack->getPosition();
		int height_num = (int)(m_CurMousePoint.x - move_point.x) / (__BlockSize*m_CurScale);
		int wdith_num = (int)(m_CurMousePoint.y - move_point.y) / (__BlockSize*m_CurScale);
		if (m_Blocks.find(wdith_num) != m_Blocks.end())
		{
			if (m_Blocks.at(wdith_num).find(height_num) != m_Blocks.at(wdith_num).end())
			{
				auto block = m_Blocks.at(wdith_num).at(height_num);
				block->SetBlockType(m_CurType);
			}
		}
	}
	if (__OpenFunction_1 && m_IsCanPoint)
	{
		PointBlock();
	}
	NpcModelFollowMouseMove();
}

void MapEditor::MouseScroll(Event * evnet)
{
	EventMouse* e = (EventMouse*)evnet;
	CCLOG("%0.2f %0.2f", e->getScrollX(), e->getScrollY());//
	m_CurScale -= (e->getScrollY() / 20.0);
	bool is_c = true;
	if (m_CurScale >= 2)
	{
		is_c = false;
		m_CurScale = 2;
	}

	if (m_CurScale <= 0.3)
	{
		is_c = false;
		m_CurScale = 0.3;
	}
	float o_s = m_LayoutBack->getScale();
	MapLayoutSetScale(m_CurScale);
	if (is_c)
	{
		//m_LayoutBack->setPosition(m_LayoutBack->getPosition() + m_LayoutBack->getContentSize() * (o_s - m_CurScale));
		//m_LayoutBlockPoint->setPosition(m_LayoutBlockPoint->getPosition() + m_LayoutBlockPoint->getContentSize() * (o_s - m_CurScale));
		//m_LayoutRole->setPosition(m_LayoutRole->getPosition() + m_LayoutRole->getContentSize() * (o_s - m_CurScale));
		MapLayoutSetPostion(m_LayoutBack->getPosition() + m_LayoutBack->getContentSize() * (o_s - m_CurScale));
	}
}

//捕捉监听
void MapEditor::OnKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet)
{
	if (keycode == EventKeyboard::KeyCode::KEY_CTRL && m_EditModelType == EditorType::ET_BlockType)  //返回
	{
		CCLOG("%d", (int)keycode);
		auto move_point = m_LayoutBack->getPosition();
		int height_num = (int)(m_CurMousePoint.x - move_point.x) / (__BlockSize*m_CurScale);
		int wdith_num = (int)(m_CurMousePoint.y - move_point.y) / (__BlockSize*m_CurScale);
		if (m_Blocks.find(wdith_num) != m_Blocks.end())
		{
			if (m_Blocks.at(wdith_num).find(height_num) != m_Blocks.at(wdith_num).end())
			{
				auto block = m_Blocks.at(wdith_num).at(height_num);
				block->SetBlockType(m_CurType);
			}
		}
	}
	//--npc 模型放下或者拿起来逻辑
	if (keycode == EventKeyboard::KeyCode::KEY_CTRL && m_EditModelType == EditorType::ET_NpcType)
	{
		PosInt touchPos = GetCurMousePos();
		CCLOG("[%d]--[%d]", touchPos.getX(),touchPos.getY());
		if (m_CurRoleModel != nullptr)
		{
			if (!(touchPos.getX() < 0 || touchPos.getY() < 0 || touchPos.getX() > (m_MapInt.getX() - 1) || (touchPos.getY() > m_MapInt.getY())))
			{
				m_CurRoleModel->SetPosForEditor(touchPos.getX(), touchPos.getY());
				Vec2 poss = Vec2(__BlockSize * (touchPos.getX() + 0.5), __BlockSize * touchPos.getY());
				m_CurRoleModel->setPosition(poss);
				m_CurRoleModel = nullptr;
			}
		}
		else
		{
			m_CurRoleModel = GetRoleModelWithPos(touchPos);
			NpcModelFollowMouseMove();
		}
	}

	if (keycode == EventKeyboard::KeyCode::KEY_CTRL)  //返回
	{
		m_IsBoolPressed = false;
	}
	//--

}
void MapEditor::OnKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet)
{
	if (keycode == EventKeyboard::KeyCode::KEY_CTRL && m_EditModelType == EditorType::ET_BlockType)
	{
		m_IsBoolPressed = true;
		CCLOG("[MapEditor::OnKeyPressed]");
		
	}
	
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		SaveMapData();
		cocos2d::Director::getInstance()->end();
	}

	if (keycode == EventKeyboard::KeyCode::KEY_F1)
	{
		bool _is = false;
		if (m_PosIsVisi == false)
		{
			_is = true;
		}
		m_PosIsVisi = _is;
		for (auto & ite : m_Blocks)
		{
			for (auto & ite2 : ite.second)
			{
				ite2.second->SetPosVisi(m_PosIsVisi);
			}
		}
	}

	if (keycode == EventKeyboard::KeyCode::KEY_F2 && __OpenFunction_2)
	{
		bool _is = false;
		if (m_IsBlockVisi == false)
		{
			_is = true;
		}
		m_IsBlockVisi = _is;
		m_LayoutBlockPoint->setVisible(m_IsBlockVisi);
	}

	if (keycode == EventKeyboard::KeyCode::KEY_F3 && __OpenFunction_3)
	{
		bool _is = false;
		if (m_IsComBlockVisi == false)
		{
			_is = true;
		}
		m_IsComBlockVisi = _is;
		for (auto & ite : m_Blocks)
		{
			for (auto & ite2 : ite.second)
			{
				if (ite2.second->m_Type == 0 || true)
				{
					ite2.second->SetComVisi(m_IsComBlockVisi);
				}
			}
		}
	}

	if (keycode == EventKeyboard::KeyCode::KEY_S)
	{
		SaveMapData();
		CCLOG("Save !!!!!!!!!");
	}

}

void MapEditor::TouchEvent(ui::Widget::TouchEventType touch_type, Vec2 touch_point)
{
	if (/*m_IsEditModel == */true)
	{
		switch (touch_type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			//CCLOG("[ControlTouchType_Began ] TPOINT[%0.2f : %0.2f]", touch_point.x, touch_point.y);
			m_CurPoint = touch_point;
			if (__TestAStarFunction && m_AStar != nullptr)
			{
				auto move_point = m_LayoutBack->getPosition();
				int height_num = (int)(m_CurMousePoint.x - move_point.x) / (__BlockSize*m_CurScale);
				int wdith_num = (int)(m_CurMousePoint.y - move_point.y) / (__BlockSize*m_CurScale);
				clock_t start, finish;
				start = clock();
				std::vector<int> temp;
				temp.push_back(1);
				auto node = m_AStar->FindPath(wdith_num, height_num, 0, 0, temp);
				finish = clock();
				CCLOG("TIME : %d", (finish - start));
				m_AStar->PrintPath();
				auto APth = m_AStar->GetPath();
				if (node == false)
				{
				}
				else{
				}
			}
			break;
		case ui::Widget::TouchEventType::MOVED:
			//CCLOG("[ControlTouchType_Moved ] TPOINT[%0.2f : %0.2f]", touch_point.x, touch_point.y);
			m_LastPoint = m_CurPoint;
			m_CurPoint = touch_point;
			m_IsMove = true;
			MapLayoutSetPostion(m_LayoutBack->getPosition() + m_CurPoint - m_LastPoint);
			break;
		case ui::Widget::TouchEventType::ENDED:
		{
			//CCLOG("[ControlTouchType_Ended ] TPOINT[%0.2f : %0.2f]", touch_point.x, touch_point.y);
			m_LastPoint = m_CurPoint;
			m_CurPoint = touch_point;
			auto move_point = m_LayoutBack->getPosition();
			if (m_IsMove == false)
			{
				m_IsPoint = true;
				/*int wdith_num = (int)(m_curmousepoint.x - move_point.x) / __blocksize;
				int height_num = (int)(m_curmousepoint.y - move_point.y) / __blocksize;
				if (m_blocks.find(wdith_num) != m_blocks.end())
				{
				if (m_blocks.at(wdith_num).find(height_num) != m_blocks.at(wdith_num).end())
				{
				auto block = m_blocks.at(wdith_num).at(height_num);
				block->setblocktype(m_curtype);
				}
				}*/
			}
			m_IsMove = false;
			m_IsPoint = false;
			break;
		}

		default:
			break;
		}
	}
}