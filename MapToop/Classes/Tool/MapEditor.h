#ifndef __MapEditor_H__
#define __MapEditor_H__
#include "Config/Head.h"
using namespace std;
class Block;
class Slots;
class AStar;
class Controller_Point;
class RoleModel;
class MapEditor : public Layout
{
public:
	enum EditorType{
		ET_NOType = 0,
		ET_BlockType,
		ET_NpcType
	};
public:
	MapEditor();
	~MapEditor();
	CREATE_FUNC(MapEditor);
	virtual bool init();
private:
	void ShowChoiceMap();
	void CloseChoiceMap();
	void InitCanEditorMapChoice();
	void InitEditorMap();
	void InitMapBlock();
	void InitMapOhterElement();
	void LoadMapData();
	void SaveMapData();
	void RefreshEditorMap();
	void SetScaleMap(float sc);
	void ShowNpcIdText();
	void CloseNpcIdText();
	RoleModel * AddRoleModel(int NpcId);
	void RemoveRoleModel(int NpcId);
	RoleModel *  GetRoleModelWithPos(PosInt pos);
	PosInt GetCurMousePos();
	void NpcModelFollowMouseMove();
	void MapLayoutSetPostion(Vec2 postions);
	void MapLayoutSetScale(float scaleNum);
private:
	Layout * m_RootWidget;
	Layout * m_LayoutBack;
	Layout * m_LayoutBlock;
	Layout * m_LayoutBlockPoint;
	Layout * m_LayoutChoice;
	Layout * m_LayoutBType;
	Layout * m_LayoutMapChoice;
	Layout * m_LayoutRole;
	ListView * m_ListMaps;
	ListView * m_ListRoleModel;
	Button * m_ButtonChoice;
	Size m_BlockSize;
	Size m_MapSize;
	PosInt m_MapInt;
	Slots * m_slots;
	TextField * m_TextFiledNpcId;
private:
	Controller_Point * m_ConPoint;
	bool m_IsEditModel;
	bool m_IsPoint;
	bool m_IsMove;
	EditorType m_EditModelType;
	Vec2 m_CurPoint;
	Vec2 m_LastPoint;
	int  m_CurMapId;
	string m_CurMapName;
	int  m_CurType;
	int  m_CurMapNum;
	int  m_WdithNum;
	int  m_HeightNum;
	Vec2 m_CurMousePoint;
	bool m_IsBoolPressed;
	bool m_PosIsVisi;
	bool m_IsBlockVisi;
	bool m_IsComBlockVisi;
	float m_CurScale;
	bool m_IsCanPoint;
	Block * m_CurPointBlock;
	std::map<int, std::map<int, int>> m_BlockData;
	map<int, map<int, vector<int>>> m_ASTARData;
    vector<OhterElement> m_Element;
	std::map<int, std::map<int, Block *>> m_Blocks;
	AStar * m_AStar;
	std::map<int, RoleModel *> m_RoleModels;
	RoleModel * m_CurRoleModel;
private:
	void InitWidget();
	void InitEvent();
	void PointBlock();
	Block * GetCurPointBlock();
private:
	void EditButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void ChoiceMapButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void ChoiceMapKindButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void SaveButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void TypeButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void CloseEditButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void CloseMapChoiceButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void NpcButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void NpcIdTextFiledEvent(Ref * pSender, ui::TextField::EventType eventType);
	void NpcChoiceButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
//--一些鼠标键盘事件
private:
	void MouseMove(Event * evnet);
	void MouseScroll(Event * evnet);
	void OnKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet);
	void OnKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet);
	void TouchEvent(ui::Widget::TouchEventType touch_type, Vec2 touch_point);
};



#endif //!__MapEditor_H__