#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "Config/Head.h"
#include "cocos2d.h"
#include "Ui/ListView3D.h"
class MBug;
class Block;
class Controller_Point;
class Slots;
class HelloWorld : public cocos2d::Layer,public ListView3D_Delegete
{
public:
	HelloWorld();
	~HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	/*
		@属性
	*/
	Layout * m_Root;
	Layout * m_LayoutBack;
	Layout * m_LayoutBlock;
	Layout * m_LayoutBlockPoint;
	Layout * m_LayoutChoice;
	Layout * m_LayoutBType;
	Layout * m_LayoutMapChoice;
	ImageView * m_ImageBack;
	ListView * m_ListMaps;
	Controller_Point * m_ConPoint;
	bool m_IsEditModel;
	bool m_IsPoint;
	bool m_IsMove;
	Vec2 m_CurPoint;
	Vec2 m_LastPoint;
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
	Block * m_CurPointBlock;
	MBug * m_Bug;
	Slots * m_Slots;
	std::map<int, std::map<int, Block *>> m_Blocks;
	std::map<int, string> m_BlockTypeString;
	std::map<int, std::map<int, uint8_t >> m_BlocksType;
	//--初始化地图设置
	void InitListMapData();
	void InitMapToolSet();
	void RestoreMapData();
	/*
		@编辑按键
	*/
	void EditButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void ChoiceMapButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void SaveButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void TypeButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void CloseEditButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void CloseMapChoiceButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	void TouchEvent(ui::Widget::TouchEventType touch_type, Vec2 touch_point);
	void MouseMove(Event * evnet);
	void MouseScroll(Event * evnet);
	void OnKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet);
	void OnKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet);
	void ChoiceMapTypeButtonEvent(ui::Widget::TouchEventType touch_type, Vec2 touch_point);

	bool SaveFileToMsk(const std::string& pFileName);

	void PointBlock();
	Block * GetCurPointBlock();


	void LoadFile(string  fileName);
	void update(float delta);
	//--3DpageView协议
	ListView3D * m_ListView3D;
	Layout * GetPageLayoutWithIndex(int index);
	void CurShowPage(Layout * layout, int index);
	void AnimaCallFunc(std::string name, int index, Node * node);
};

#endif // __HELLOWORLD_SCENE_H__
