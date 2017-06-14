#ifndef  _HEAD_H_
#define  _HEAD_H_
/*
* 所有需要引用的基础头文件
*/
#include "cocos2d.h"
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#include <io.h>
#endif
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <random> 
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <time.h>
#include <math.h>
#include <tuple>
using namespace  cocos2d::ui;
using namespace cocostudio;
using namespace std;
USING_NS_CC;
/*
* 一些宏定义配置	
*/
#ifndef	 __GetPointX
#define  __GetPointX(y_obj) y_obj->getPosition().x
#endif

#ifndef	 __GetPointY
#define  __GetPointY(y_obj) y_obj->getPosition().y
#endif

#ifndef	 __VisibleSize
#define  __VisibleSize Director::getInstance()->getVisibleSize()
#endif

#ifndef	 __VisibleSize_Height
#define  __VisibleSize_Height Director::getInstance()->getVisibleSize().height
#endif

#ifndef	 __VisibleSize_Width
#define  __VisibleSize_Width Director::getInstance()->getVisibleSize().width
#endif

#ifndef	 MENBER//(type,name)
#define  MENBER(type,name)\
	private: type m_##name; \
	public: type name() const { return this->m_##name; }\
	public: void name(type _arg){ this->m_##name = _arg; }
#endif
/*
	获取子节点，并且给予赋值
*/
#ifndef __GetChildAndAssignment//(_yChild,yPNode,_yNmae)
#define __GetChildAndAssignment(_yChild,yPNode,_yNmae)\
{\
	_yChild = (Layout *)(Helper::seekWidgetByName(yPNode, _yNmae));\
	if (nullptr == _yChild)\
	{\
		CCLOG("[%s] Can't find it", _yNmae);\
	}\
}
#endif

/*
获取子节点，并且给予赋值
*/
#ifndef __GetChildAndAssignmentWithOB//(_yChild,yPNode,_yNmae,_NodeOb)
#define __GetChildAndAssignmentWithOB(_yChild,yPNode,_yNmae,_NodeOb)\
{\
	_yChild = (_NodeOb *)(Helper::seekWidgetByName(yPNode, _yNmae)); \
if (nullptr == _yChild)\
{\
	_yChild = (_NodeOb *)(yPNode->getChildByName(_yNmae)); \
	if(nullptr == _yChild)\
	{\
		CCLOG("[%s] Can't find it", _yNmae); \
	}\
}\
}
#endif

/*
	@从父节点获取按钮并且设置回调
*/
#ifndef __AddTouchEventWithNameAndPNode//(_yNmae,_yPNode,_eventCall)
#define __AddTouchEventWithNameAndPNode(_yNmae,_yPNode,_eventCall){\
	auto btn = (Button *)Helper::seekWidgetByName(_yPNode, _yNmae); \
	if (btn)\
	{\
		btn->addTouchEventListener(CC_CALLBACK_2(_eventCall, this));\
	}\
	else{\
		CCLOG("[%s] Can't find it", _yNmae);\
	}\
}
#endif

//--给按钮设置回调事件的宏
#ifndef _AddTouchLis_//(y_parent,y_name,y_callback)
#define _AddTouchLis_(y_parent,y_name,y_callback)\
{\
	auto btn = (Button *)y_parent->getChildByName(y_name); \
	btn->addTouchEventListener(CC_CALLBACK_2(y_callback, this));\
}

#endif
//--给按钮设置回调事件的宏
#ifndef _SetButtonVisible_\//(y_parent,y_name,is_visi)
#define _SetButtonVisible_(y_parent,y_name,is_visi)\
{\
	auto btn = (Button *)y_parent->getChildByName(y_name); \
	btn->setVisible(is_visi); \
	btn->setTouchEnabled(is_visi); \
}

#endif
/*
	@一些结构体
*/

/*
	@一些枚举
*/
#ifndef	 __ControlTouchType_Enum
#define  __ControlTouchType_Enum
enum ControlTouchType
{
	ControlTouchType_Began = 1,
	ControlTouchType_Moved,
	ControlTouchType_Ended
};
#endif

#ifndef __CellState_Enum
#define __CellState_Enum
enum CellStateType
{
	Default = 1,//默认
	Walk ,//行走
	BallType_Life		//生命球
};

#endif

//------------------------------------------------


#ifndef __Controller_Square_Dir_Enum
#define __Controller_Square_Dir_Enum

enum Square_DirType
{
	Square_DirType_Null = 0,//无方向
	Square_DirType_Up,      //上
	Square_DirType_Donw,      //上
	Square_DirType_Left,      //上
	Square_DirType_Right,      //上
};
typedef void (Ref::*SEL_CallFunc_SquareEnum)(Square_DirType);
#define CallFunc_SquareEnum_SELECTOR(_SELECTOR) static_cast<SEL_CallFunc_SquareEnum>(&_SELECTOR)
#define CallFunc_SquareEnum_selector(_SELECTOR) CallFunc_SquareEnum_SELECTOR(_SELECTOR)
typedef std::function<void(Square_DirType)> CallFunc_Function_SquareEnum;
#endif // !__Controller_Square_Dir_Enum



/*
	@位置类
*/
#ifndef __PosInt
#define __PosInt
class PosInt
{
public:
	PosInt()
	{
		m_PosX = 0;
		m_PosY = 0;
	}
	PosInt(int posx, int posy) :
		m_PosX(posx),
		m_PosY(posy)
	{}
	PosInt(const PosInt &posInt){
		m_PosX = posInt.getX();
		m_PosY = posInt.getY();
	}
	~PosInt()
	{

	}
	CC_SYNTHESIZE(int, m_PosX, X);//X
	CC_SYNTHESIZE(int, m_PosY, Y);//Y
	/*
		@
	*/
	static bool IsSamePosInt(const PosInt &p_1, const PosInt &p_2)
	{
		bool _is = false;
		if (p_1.getX() == p_2.getX() && p_1.getY() == p_2.getY())
		{
			_is = true;
		}
		return _is;
	}
	PosInt& operator=(const PosInt posInt){
		m_PosX = posInt.getX();
		m_PosY = posInt.getY();
		return *this;
	}
	PosInt operator+(const PosInt posInt){
		PosInt new_pos;
		new_pos.setX(m_PosX + posInt.getX());
		new_pos.setY(m_PosY + posInt.getY());
		return new_pos;
	}
	PosInt operator-(const PosInt posInt){
		PosInt new_pos;
		new_pos.setX(m_PosX - posInt.getX());
		new_pos.setY(m_PosY - posInt.getY());
		return new_pos;
	}
	bool operator==(const PosInt &posInt)const{
		if (m_PosX == posInt.getX() && m_PosY == posInt.getY())
		{
			return true;
		}
		return false;
	}
};
#endif // !__BallInfo_Data

#ifndef	 __SquareTouchDel
#define  __SquareTouchDel 10.0
#endif //最小判断距离



/*
@ 道具类数据
*/
#ifndef __GameMoveItem_Data
#define __GameMoveItem_Data

class GameMoveItemData
{
public:
	GameMoveItemData() :
		m_Id(0),
		m_FriendId(0),
		m_Name(""),
		m_Description(""),
		m_Src(""),
		m_Type(0),
		m_SPos(Vec2(0,0)),
		m_EndSrc("")
	{
	}
	~GameMoveItemData()
	{

	}
	CC_SYNTHESIZE(int, m_Id, MyId);//道具的Id
	CC_SYNTHESIZE(int, m_FriendId, MyFriendId);//道具的Id
	CC_SYNTHESIZE(string, m_Name, MyName);//道具的名字
	CC_SYNTHESIZE(string, m_Description, MyDescription);//道具的描述
	CC_SYNTHESIZE(int, m_Type, MyType);//道具的描述
	CC_SYNTHESIZE(string, m_Src, MySrc);//道具的资源路径
	CC_SYNTHESIZE(Vec2, m_SPos, MySPos);//道具的起始位置
	CC_SYNTHESIZE(string, m_EndSrc, MyEndSrc);//背包里面的图像
    CC_SYNTHESIZE(Rect, m_EndRect, MyEndRect);//背包里面的图像
	CC_SYNTHESIZE(string, m_Value, MyValue);//值
	CC_SYNTHESIZE(int, m_MusicId, MyMusicId);//音乐Id
};

class GameMoveTimeItemData
{
public:
    GameMoveTimeItemData() :
    m_Id(0),
    m_FriendId(0),
    m_Name(""),
    m_Description(""),
    m_Src(""),
    m_Type(0),
    m_SPos(Vec2(0,0)),
    m_EndRect(0,0,0,0),
    m_PointRect(0,0,0,0)
    {
    }
    ~GameMoveTimeItemData()
    {
        
    }
    CC_SYNTHESIZE(int, m_Id, MyId);//道具的Id
    CC_SYNTHESIZE(int, m_FriendId, MyFriendId);//道具的Id
    CC_SYNTHESIZE(string, m_Name, MyName);//道具的名字
    CC_SYNTHESIZE(string, m_Description, MyDescription);//道具的描述
    CC_SYNTHESIZE(int, m_Type, MyType);//道具的描述
    CC_SYNTHESIZE(string, m_Src, MySrc);//道具的资源路径
    CC_SYNTHESIZE(Vec2, m_SPos, MySPos);//道具的起始位置
    CC_SYNTHESIZE(Rect, m_EndRect, MyEndRect);//起效范围
    CC_SYNTHESIZE(Rect, m_PointRect, MyPointRect);//起效位置
    CC_SYNTHESIZE(string, m_Value, MyValue);//值
    CC_SYNTHESIZE(int, m_MusicId, MyMusicId);//音乐Id
};

class GameMixItemData
{
public:
    GameMixItemData() :
    m_Id(0),
    m_FriendId(0),
    m_Name(""),
    m_Description(""),
    m_Src(""),
    m_Type(0),
    m_SPos(Vec2(0,0)),
    m_Value("")
    {
    }
    ~GameMixItemData()
    {
        
    }
    CC_SYNTHESIZE(int, m_Id, MyId);//道具的Id
    CC_SYNTHESIZE(int, m_FriendId, MyFriendId);//道具的Id
    CC_SYNTHESIZE(string, m_Name, MyName);//道具的名字
    CC_SYNTHESIZE(string, m_Description, MyDescription);//道具的描述
    CC_SYNTHESIZE(int, m_Type, MyType);//道具的描述
    CC_SYNTHESIZE(string, m_Src, MySrc);//道具的资源路径
    CC_SYNTHESIZE(Vec2, m_SPos, MySPos);//道具的起始位置
    CC_SYNTHESIZE(string, m_Value, MyValue);//值
    CC_SYNTHESIZE(int, m_MusicId, MyMusicId);//音乐Id
};


class ItemKindData
{
public:
    ItemKindData() :
    m_Id(0),
    m_Name(""),
    m_Description(""),
    m_Src(""),
    m_Type(0),
    m_MusicId(0)
    {
    }
    ~ItemKindData()
    {
        
    }
    CC_SYNTHESIZE(int, m_Id, MyId);//道具的Id
    CC_SYNTHESIZE(string, m_Name, MyName);//道具的名字
    CC_SYNTHESIZE(string, m_Description, MyDescription);//道具的描述
    CC_SYNTHESIZE(int, m_Type, MyType);//道具的描述
    CC_SYNTHESIZE(string, m_Src, MySrc);//道具的资源路径
    CC_SYNTHESIZE(int, m_MusicId, MyMusicId);//音乐Id
};

class ItemChildData
{
public:
    ItemChildData() :
    m_Id(0),
    m_Name(""),
    m_Description(""),
    m_Src(""),
    m_Type(0),
    m_MusicId(0),
    m_Tag(1),
    m_Ico(""),
    m_IsLock(true)
    {
    }
    ~ItemChildData()
    {
        
    }
    CC_SYNTHESIZE(int, m_Id, MyId);//道具的Id
    CC_SYNTHESIZE(string, m_Name, MyName);//道具的名字
    CC_SYNTHESIZE(string, m_Description, MyDescription);//道具的描述
    CC_SYNTHESIZE(int, m_Type, MyType);//道具的描述
    CC_SYNTHESIZE(string, m_Src, MySrc);//道具的资源路径
    CC_SYNTHESIZE(int, m_MusicId, MyMusicId);//音乐Id
    CC_SYNTHESIZE(int, m_Tag, MyTag);//标签值
    CC_SYNTHESIZE(string, m_Ico, MyIco);//道具的小图标
    CC_SYNTHESIZE(bool, m_IsLock, MyIsLock);//道具的小图标
};


class NoticeStringData
{
public:
    NoticeStringData() :
    m_Id(0),
    m_Description(""),
    m_Value("")
    {
    }
    ~NoticeStringData()
    {
        
    }
    CC_SYNTHESIZE(int, m_Id, MyId);//道具的Id
    CC_SYNTHESIZE(string, m_Description, MyDescription);//道具的描述
    CC_SYNTHESIZE(string, m_Value, MyValue);//值
};


#endif // !__GameItem_Data


/*
@ 关卡数据
*/
#ifndef __GameLevel_Data
#define __GameLevel_Data


class MusicData
{
public:
	MusicData(){

	}
	~MusicData(){

	}
	CC_SYNTHESIZE(int, m_Id, MyId);//音乐的Id
	CC_SYNTHESIZE(string, m_MusicPath, MyMusicPath);//音乐路径
};

#endif // !__GameLevel_Data

/*
	@一些函数选择器
*/
#ifndef __FUNCTIONST
#define __FUNCTIONST 
/*
	@带一个bool值的函数选择器
*/
typedef void (Ref::*SEL_CallFunc_Bool)(bool);
#define CallFunc_Bool_SELECTOR(_SELECTOR) static_cast<SEL_CallFunc_Bool>(&_SELECTOR)
#define CallFunc_Bool_selector(_SELECTOR) CallFunc_Bool_SELECTOR(_SELECTOR)
typedef std::function<void(bool)> CallFunc_Function_Bool;

/*
@带一个int值的函数选择器
*/
typedef void (Ref::*SEL_CallFunc_Int)(int);
#define CallFunc_Int_SELECTOR(_SELECTOR) static_cast<SEL_CallFunc_Int>(&_SELECTOR)
#define CallFunc_Int_selector(_SELECTOR) CallFunc_Int_SELECTOR(_SELECTOR)
typedef std::function<void(int)> CallFunc_Function_Int;

/*
@带一个int值的函数选择器
@带一个Node指针的
*/
typedef void (Ref::*SEL_CallFunc_IntAndNode)(int, Node *);
#define CallFunc_IntAndNode_SELECTOR(_SELECTOR) static_cast<SEL_CallFunc_IntAndNode>(&_SELECTOR)
#define CallFunc_IntAndNode_selector(_SELECTOR) CallFunc_IntAndNode_SELECTOR(_SELECTOR)
typedef std::function<void(int, Node *)> CallFunc_Function_IntAndNode;

/*
 @带一个按钮状态参数
 @带一个当前点击坐标的函数选择器
 */
typedef void (Ref::*SEL_CallFunc_ButtonTypeAndPoint)(ui::Widget::TouchEventType, Vec2);
#define CallFunc_ButtonTypeAndPoint_SELECTOR(_SELECTOR) static_cast<SEL_CallFunc_ButtonTypeAndPoint>(&_SELECTOR)
#define CallFunc_ButtonTypeAndPoint_selector(_SELECTOR) CallFunc_ButtonTypeAndPoint_SELECTOR(_SELECTOR)
typedef std::function<void(ui::Widget::TouchEventType, Vec2)> CallFunc_Function_ButtonTypeAndPoint;

/*
	@无参数的函数选择器
*/
typedef void (Ref::*SEL_CallFunc_Void)();
#define CallFunc_Void_SELECTOR(_SELECTOR) static_cast<SEL_CallFunc_Void>(&_SELECTOR)
#define CallFunc_Void_selector(_SELECTOR) CallFunc_Void_SELECTOR(_SELECTOR)
typedef std::function<void()> CallFunc_Function_Void;


/*
@带一个int值的函数选择器
@带一个Node指针的
*/
typedef void (Ref::*SEL_CallFunc_StringIntAndNode)(std::string, int, Node *);
#define CallFunc_StringIntAndNode_SELECTOR(_SELECTOR) static_cast<SEL_CallFunc_StringIntAndNode>(&_SELECTOR)
#define CallFunc_StringIntAndNode_selector(_SELECTOR) CallFunc_StringIntAndNode_SELECTOR(_SELECTOR)
typedef std::function<void(std::string,int, Node *)> CallFunc_Function_StringIntAndNode;



#endif //__FUNCTIONST 




#ifndef	 __HeadFileIncludeNum  
#define  __HeadFileIncludeNum 1
#define  __HeadFileHavePath 1
#define  __HeadFileNoPath 2
//--设计模式
#define __DesignResolutionWidth 960.0
#define __DesignResolutionHight 640.0

//--适配宏
#define __ResolutionPolicy_FIXED_WIDTH  (__VisibleSize_Height - __DesignResolutionHight * (__VisibleSize_Width / __DesignResolutionWidth)) / 2.0

//--是否是debug模式

#define __IsDebugModel 1

//--转换百分比的宽和高
#define __TransformPercentWidthToViewSize(_p_width) (( _p_width / 100.0) * __VisibleSize_Width)
#define __TransformPercentHeightToViewSize(_p_height) (( _p_height / 100.0) * __VisibleSize_Height)

#define __TransformPercentWidthToViewSizeWithPrents(_p_width,_p_p) (( _p_width / 100.0) * _p_p->getContentSize().width;)
#define __TransformPercentHeightToViewSizeWithPrents(_p_height,_p_p) (( _p_height / 100.0) * _p_p->getContentSize().height)
//--重置所有子节点的百分比坐标
#define __SetAllChildPercentPosition(y_node)\
{\
auto all_child = y_node->getChildren();\
for(auto & ite : all_child)\
{\
auto position_c = (Layout *) ite;\
position_c->setPositionPercent(position_c->getPositionPercent());\
}\
}
#endif //头文件引用关系


//======map
#define __MapPngName "Map/%s.csb"
#define __MapDataName "Map/Data%s.txt"
#define __splitEAndB "+_)(*&^%$#@!<>?|"
#define __BlockPosSave "%d,"
#define __ElementSave "%d,%d,%d#"
#define __ElementDataOneLineNum 10
#define __BlockSize 64

class OhterElement
{
public:
	OhterElement(){
		e_pos.setX(0);
		e_pos.setY(0);
		e_id = 0;
	}
	OhterElement(OhterElement * o_e){
		e_id = o_e->e_id;
		e_pos.setX(o_e->e_pos.getX());
		e_pos.setY(o_e->e_pos.getY());
	}
	~OhterElement(){
	}
	PosInt e_pos;
	int  e_id;
private:
	
};

#define  PLAYER_1_UID 1
//======map end

/*
@dir 1.上,2.下，3.左，4.右
*/
enum DirType
{
	DirT_Up = 1,
	DirT_Down,
	DirT_Left,
	DirT_Right,
	DirT_MAX
};
/*
	@RoleModel 类型
*/
enum ModelType
{
	ModelT_Role = 1,
	ModelT_Goods,
	ModelT_Transfer,
	ModelT_MAX
};

enum TalkType
{
	TalkType_Alone = 1,//单人称述
	TalkType_People,//多人对话
	TalkType_Select,//有选择对话
	TalkType_MAX
};

enum ActionType
{
	Action_Run = 2,
	Action_Walk = 3,
	Action_Attack = 4,
	Action_BeAttack = 5,
	Action_Skill = 6,
};

enum OperationType
{
	OP_Move = 1,
	OP_Jump = 2,
};
//===================
#endif