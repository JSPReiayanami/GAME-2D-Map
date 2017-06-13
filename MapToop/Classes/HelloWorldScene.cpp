#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "direct.h"
#include "Controller/Controller_Point.h"
USING_NS_CC;
#include "Entity/Block.h"
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include<io.h>
#include "Tool/StringHelper.h"
#include "Entity/Slots.h"
#include "Tool/MapEditor.h"
using namespace cocostudio::timeline;
#include "Entity/GLTestingLayer.h"
#define __MapSrc "MapBackground/"
#define __MapHeadName "map"
#define __MapEndName ".csb"
#define __BlockSize 64
#define __OpenFunction_1 true//false // 移动到的块会有动画效果功能
#define __OpenFunction_2 true// 按F2隐藏块层
#define __OpenFunction_3 true//按F3隐藏普通块
#define __OpenFunction_4 true// 地图选择功能
#include "network/WebSocket.h"
#include "Tool/TabFileReader.h"
#include "Entity/Effect.h"
#include "Entity/EffectSprite.h"
void HelloWorld::LoadFile(string fileName)
{
	TFReader file = TFReader::create(fileName.c_str());
	if (!file)
		return ;
	int count = file.GetItemCount();

	for (int i = 0; i < count; i ++) {
		uint32_t aId = 0;
		string actTxt;
		file.Read(i, "id", aId);
		file.Read(i, "actTxt", actTxt);
		CCLOG("actTxt:[%s]",actTxt.c_str());
	}
}
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
	
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
{
	m_BlockTypeString.clear();
	uint8_t b_1 = 0;
	uint8_t b_2 = 1;
	uint8_t b_3 = 2;
	m_BlockTypeString.insert(make_pair(0, "0000"));
	m_BlockTypeString.insert(make_pair(1, "0101"));
	m_BlockTypeString.insert(make_pair(2, "0202"));
	m_Blocks.clear();
	m_ConPoint = nullptr;
	m_CurPoint = Vec2(0,0);
	m_LastPoint = Vec2(0,0);
	m_IsMove = false;
	m_IsPoint = false;
	m_PosIsVisi = false;
	m_CurScale = 1.0;
	m_CurPointBlock = nullptr;
	m_IsBlockVisi = true;
}

HelloWorld::~HelloWorld()
{

}


void getAllFiles(string path, vector<string>& files)//lxk20160302
{
	//文件句柄
	intptr_t  hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	cout << path.c_str() << endl;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		_findnext(hFile, &fileinfo);
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				cout << fileinfo.name << endl;
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				continue;
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
// on "init" you need to initialize your instance
#include "Entity/RichTextEntity.h"
#include "Entity/EffectAnimation.h"
#include "Ui/ItemOption.h"
#include "MapRoot.h"
bool HelloWorld::init()
{
	//LoadFile("activity.txt");
	//测试有效,可以迭代获取该目录下所有子目录和本目录所有文件
 	//vector<string> myFilePhth;
	//StringHelper::getAllFileInPathWithType("Map", myFilePhth,"csb");
    if ( !Layer::init() )
    {
        return false;
    }
    if (true)
    {
		m_Root = MapEditor::create();
		addChild(m_Root);
		//MapRoot * mr = MapRoot::Create(123);
		//addChild((Layout *)mr);
		//ItemOption * itemO = ItemOption::create();
		//itemO->setPosition(__VisibleSize / 2);
		//addChild(itemO, 999, "itemO");
		//Sprite * sp = Sprite::create("1.png");
		//ProgressTimer * proTimer = ProgressTimer::create(sp);
		//proTimer->setPosition(__VisibleSize / 2);
		//addChild(proTimer, 999,"proTimer");
		//scheduleUpdate();


// 		string testStr = "{#color=255,0,255;FontSize=35;asa;;;;dsaaaaaaaaaaaaaaaaaaaaaaaaaaaadasd#}{#ImagePath=1.png;sdadsaasd#}{#color=255,0,0;asdasdadadasdaaaaaaaaaaaaaaaaaaaaaaaaa#}";
// 		auto rt = RichTextEntity::CreateWithContent(testStr,Size(200,200));
// 		rt->setPosition(__VisibleSize / 2);
// 		m_Root->addChild(rt, 9999);
		//== 3D
		/*m_ListView3D = ListView3D::create();
		m_ListView3D->SetOnePageSize(Size(300, 300));
		m_ListView3D->SetAllPageNum(30);
		m_ListView3D->SetDelegete(this);
		m_ListView3D->setAnchorPoint(Vec2(0.5,0.5));
		m_ListView3D->setPosition(__VisibleSize / 2);*/
		//addChild(m_ListView3D, 999);
		//==
// 		Armature * am = Armature::create("");
// 		am->getAnimation()->play("");
		/*for (int count = 1; count <= 1;count++)
		{
		EffectArmatureFuncData func = EffectArmatureFuncData("Rotate", 4, CC_CALLBACK_3(HelloWorld::AnimaCallFunc, this));
		EffectArmatureFuncData func2 = EffectArmatureFuncData("Rotate", 2, CC_CALLBACK_3(HelloWorld::AnimaCallFunc, this));
		auto effctAnima = EffectArmature::create("1.png");
		effctAnima->AddAnimateCallFunc(func);
		effctAnima->AddAnimateCallFunc(func2);
		Vector<SpriteFrame*> amV;
		amV.clear();
		for (int i = 1; i <= 5; i++)
		{
		SpriteFrame * sf = SpriteFrame::create(StringUtils::format("%d.png", i).c_str(), Rect(0, 0, 107, 134));
		amV.pushBack(sf);
		}
		Animation * am = Animation::createWithSpriteFrames(amV, 0.1, -1);
		Animate * amte = Animate::create(am);
		effctAnima->AddAnimate(amte, "Rotate");
		effctAnima->setPosition(__VisibleSize / 2);
		effctAnima->setPositionX(effctAnima->getPositionX() + 10 * count);
		effctAnima->setEffect(EffectOutline::create());
		addChild(effctAnima, 999);
		effctAnima->PlayAnimation("Rotate");
		}*/
		//==3D乌龟
		/*
		
		Sprite3D * s3d = Sprite3D::create("Spirte3D/tortoise.c3b");
		//s3d->setScale(0.1f);
		s3d->setScale(0.1);
		s3d->setPositionY(s3d->getContentSize().height / 20);
		s3d->setPositionX(s3d->getContentSize().width / 20);
		s3d->setGlobalZOrder(10);
		s3d->setCameraMask((unsigned short)CameraFlag::DEFAULT);
		auto animation = Animation3D::create("Spirte3D/tortoise.c3b");
		Animate3D * anim = Animate3D::create(animation);
		if (animation)
		{
		auto animate = Animate3D::create(animation, 0.f, 1.933f);
		s3d->runAction(RepeatForever::create(animate));
		}
		auto moveTo1 = MoveTo::create(5, __VisibleSize);
		auto moveTo2 = MoveTo::create(5, Vec2(s3d->getContentSize().width / 20, s3d->getContentSize().height / 20));
		auto seq = Sequence::create(moveTo1, moveTo2, NULL);
		addChild(s3d);
		//s3d->runAction(RepeatForever::create(seq));


		auto listener1 = EventListenerTouchOneByOne::create();
		listener1->setSwallowTouches(true);

		listener1->onTouchBegan = [](Touch* touch, Event* event){
		auto target = static_cast<Sprite3D*>(event->getCurrentTarget());

		Rect rect = target->getBoundingBox();
		if (rect.containsPoint(touch->getLocation()))
		{
		log("sprite3d began... x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);
		//target->setOpacity(100);
		return true;
		}
		return false;
		};
		listener1->onTouchMoved = [](Touch* touch, Event* event){
		auto target = static_cast<Sprite3D*>(event->getCurrentTarget());
		//target->setPosition(target->getPosition() + touch->getDelta());
		Vec2 vec2 = target->getPosition() + touch->getDelta();
		target->setPosition3D(Vec3(vec2.x, vec2.y,0));
		};

		listener1->onTouchEnded = [=](Touch* touch, Event* event){
		auto target = static_cast<Sprite3D*>(event->getCurrentTarget());
		log("sprite3d onTouchesEnded.. ");
		target->setOpacity(255);
		};

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, s3d);
		
		*/
	}
	else
	{
		m_Root = (Layout *)CSLoader::createNode("Interface_MapTool.csb");
		__GetChildAndAssignmentWithOB(m_LayoutBack, m_Root, "Panel_Background", Layout);
		__GetChildAndAssignmentWithOB(m_LayoutBlock, m_Root, "Panel_Block", Layout);
		__GetChildAndAssignmentWithOB(m_LayoutBType, m_Root, "Panel_BlockType", Layout);
		__GetChildAndAssignmentWithOB(m_LayoutBlockPoint, m_Root, "Panel_BlockPoint", Layout);
		__GetChildAndAssignmentWithOB(m_LayoutChoice, m_Root, "Panel_Button", Layout);
		__GetChildAndAssignmentWithOB(m_LayoutMapChoice, m_Root, "Panel_MapChoice", Layout);
		__GetChildAndAssignmentWithOB(m_ImageBack, m_LayoutBack, "Image_Background", ImageView);
		__GetChildAndAssignmentWithOB(m_ListMaps, m_LayoutMapChoice, "ListView_Map", ListView);
		__AddTouchEventWithNameAndPNode("Button_EdietS", m_LayoutChoice, HelloWorld::EditButtonEvent);
		__AddTouchEventWithNameAndPNode("Button_Save", m_LayoutChoice, HelloWorld::SaveButtonEvent);
		__AddTouchEventWithNameAndPNode("Button_MapChoice", m_LayoutChoice, HelloWorld::ChoiceMapButtonEvent);
		__AddTouchEventWithNameAndPNode("Button_Close", m_LayoutBType, HelloWorld::CloseEditButtonEvent);
		__AddTouchEventWithNameAndPNode("Button_Close", m_LayoutMapChoice, HelloWorld::CloseMapChoiceButtonEvent);
		for (int i = 1; i <= 3; i++)
		{
			__AddTouchEventWithNameAndPNode(StringUtils::format("Button_Type%d", i), m_LayoutBType, HelloWorld::TypeButtonEvent);
		}
		m_LayoutBType->setVisible(false);
		m_LayoutMapChoice->setVisible(false);
		//InitListMapData();
		InitMapToolSet();
		RestoreMapData();
		m_ConPoint = Controller_Point::create();
		m_ConPoint->OpenTouch();
		m_ConPoint->RegisterCallback(CallFunc_ButtonTypeAndPoint_SELECTOR(HelloWorld::TouchEvent), this);
		this->addChild(m_ConPoint, 99);


		//创建鼠标监听
		auto listener = EventListenerMouse::create();
		//分发MouseMove事件
		listener->onMouseMove = CC_CALLBACK_1(HelloWorld::MouseMove, this);
		listener->onMouseScroll = CC_CALLBACK_1(HelloWorld::MouseScroll, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


		//注册捕捉监听
		auto listenerkeyPad = EventListenerKeyboard::create();
		listenerkeyPad->onKeyReleased = CC_CALLBACK_2(HelloWorld::OnKeyReleased, this);
		listenerkeyPad->onKeyPressed = CC_CALLBACK_2(HelloWorld::OnKeyPressed, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

		m_Slots = Slots::create();
		m_LayoutBlockPoint->addChild(m_Slots, 99);
		m_Slots->setVisible(false);
	}

	//addChild(m_Root);
	
// 	JniMethodInfo minfo;//定义Jni函数信息结构体
// 	bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/chukong/cocosplay/clientCocosPlayClient","isNotifyFileLoadedEnabled", "()Z");
// 	if (!isHave) {
// 		//CCLog("jni:此函数不存在");
// 	}
// 	else{
// 		//CCLog("jni:此函数存在");
// 		//调用此函数
// 		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
// 	}
    return true;
}
//--初始化地图设置

void HelloWorld::InitListMapData()
{
	int start_index = 1;
	m_ListMaps->removeAllItems();
	while (1)
	{
		string map_name = StringUtils::format("%s%s", __MapSrc, __MapHeadName);
		if (start_index < 10)
		{
			map_name.append("00");
		}
		else if (start_index < 100)
		{
			map_name.append("0");
		}
		map_name.append(StringUtils::format("%d%s", start_index, __MapEndName));
		//ImageView * imgae = nullptr;
		//imgae = ImageView::create(map_name);
		Node * node = nullptr;
		node = CSLoader::createNode(map_name);
		Button * button = nullptr;
		button = Button::create("B_1.jpg");
		ifstream OpenFile(map_name);
		if (node == nullptr)
		{
			break;
		}
		else{
			auto size = button->getContentSize();
			float s_float = 1 / (size.width / m_ListMaps->getContentSize().width);
			button->setScale(s_float);
			button->setTag(start_index);
			auto lay = Layout::create();
			lay->addChild(button);
			lay->setContentSize(size * s_float);
			button->setPosition(lay->getContentSize() / 2);
			m_ListMaps->pushBackCustomItem(lay);
		}
		//if (nullptr != imgae)
		//{
		//	auto size = button->getContentSize();
		//	button->setScale(1 / (size.width / m_ListMaps->getContentSize().width));
		//	button->setTag(start_index);
		//	m_ListMaps->pushBackCustomItem(button);
		//}
		//else
		//{
		//	break;
		//}
		start_index++;
	}
}


void HelloWorld::InitMapToolSet()
{
	m_CurScale = 1;
	m_LayoutBack->setScale(m_CurScale);
	m_LayoutBlockPoint->setScale(m_CurScale);

	auto pos = m_ImageBack->getPositionPercent();
	auto par = m_ImageBack->getParent();
	auto z_lo = m_ImageBack->getLocalZOrder();
	m_ImageBack->removeFromParent();
	m_ImageBack = nullptr;
	m_Blocks.clear();
	m_WdithNum = 0;
	m_HeightNum = 0;
	auto map_node = (Layout *)CSLoader::createNode("MapBackground/map001.csb");
	par->addChild(map_node, -1);
	auto size = map_node->getContentSize();
	CCLOG("Iamge width:%0.2f  height:%0.2f", size.width, size.height);

	auto width_num = ceil(size.width / __BlockSize);
	auto height_num = ceil(size.height / __BlockSize);
	m_WdithNum = width_num;
	m_HeightNum = height_num;
	for (int i = 0; i < height_num; i++)
	{
		for (int j = 0; j < width_num; j++)
		{
			auto block = Block::CreateWithInfo(Size(__BlockSize, __BlockSize), 1);
			m_LayoutBlockPoint->addChild(block);
			block->SetPos(PosInt(j, i));
			if (m_Blocks.find(j) == m_Blocks.end())
			{
				map<int, Block *> temp;
				temp.clear();
				temp.insert(make_pair(i, block));
				m_Blocks.insert(make_pair(j, temp));
			}
			else
			{
				m_Blocks.at(j).insert(make_pair(i, block));
			}
		}
	}
	//m_ImageBack = ImageView::create("MapBackground/map001.jpg");
	//if (nullptr != m_ImageBack)
	//{
	//	
	//	//m_ImageBack = ImageView::create("MapBackground_1.jpg");
	//	m_ImageBack->setAnchorPoint(Vec2::ZERO);
	//	m_ImageBack->setPositionPercent(pos);
	//	par->addChild(m_ImageBack, -1);
	//	//--
	//	auto size = m_ImageBack->getContentSize();
	//	CCLOG("Iamge width:%0.2f  height:%0.2f", size.width, size.height);

	//	auto width_num = ceil(size.width / __BlockSize);
	//	auto height_num = ceil(size.height / __BlockSize);
	//	m_WdithNum = width_num;
	//	m_HeightNum = height_num;
	//	for (int i = 0; i < height_num; i++)
	//	{
	//		for (int j = 0; j < width_num; j++)
	//		{
	//			auto block = Block::CreateWithInfo(Size(__BlockSize, __BlockSize), 1);
	//			m_LayoutBlockPoint->addChild(block);
	//			block->SetPos(PosInt(j, i));
	//			if (m_Blocks.find(j) == m_Blocks.end())
	//			{
	//				map<int, Block * > temp;
	//				temp.clear();
	//				temp.insert(make_pair(i, block));
	//				m_Blocks.insert(make_pair(j, temp));
	//			}
	//			else
	//			{
	//				m_Blocks.at(j).insert(make_pair(i, block));
	//			}
	//		}
	//	}
	//}
}

Size mapsize;
uint8_t *mapdata = NULL; // 目前只有2个状态，bit0 = 半透明显示 bit1 = 不可行走CPoint dragPos;
Point prevPos;
Point showPos;
Point logicPos;
BOOL inDrag = false;
std::string pathutf8;
std::string fileutf8;
std::string rawutf8;


Node *layer = NULL;
Node *map = NULL;
using namespace CocosDenshion;

bool g_ShowDisplayStats = false;

extern void utf16_2_utf8(const wchar_t* in, std::string& out);
void HelloWorld::RestoreMapData()
{
	rawutf8 = "MapBackground/map001.msk";
	mapdata = new uint8_t[m_WdithNum*m_HeightNum];
	mapsize.width = m_WdithNum;
	mapsize.height = m_HeightNum;
	FILE *fp = fopen(rawutf8.c_str(), "rb");
	if (fp != NULL) {
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (size != mapsize.width * mapsize.height) {
			//MessageBox(CCEGLView::sharedOpenGLView()->getHWnd(), L"MASK数据大小和地图宽高不匹配", L"出错啦", 0);
			memset(mapdata, 0, mapsize.width * mapsize.height);
		}
		else {
			fread(mapdata, 1, mapsize.width * mapsize.height, fp);
			fclose(fp);
			for (int pos_y = 0; pos_y < m_HeightNum; pos_y++)
			{
				for (int pos_x = 0; pos_x < m_WdithNum; pos_x++)
				{
					if (m_Blocks.find(pos_x) != m_Blocks.end())
					{
						if (m_Blocks.at(pos_x).find(pos_y) != m_Blocks.at(pos_x).end())
						{
							auto block = m_Blocks.at(pos_x).at(pos_y);
							block->SetBlockType(mapdata[pos_y * m_WdithNum + pos_x]);
						}
					}
				}
			}
		}
	}
	else {
		//MessageBox(CCEGLView::sharedOpenGLView()->getHWnd(), L"找不到MASK数据文件", L"提示", 0);
		memset(mapdata, 0, mapsize.width * mapsize.height);
	}
	
	//for ()
	//{
	//}
	//
	/*vector<std::string> vect_string;
	ifstream OpenFile("MapBackground/map001.msk");
	if (OpenFile.fail())
	{
	return;
	}
	else{
	}
	char buf[10000];
	do{
	OpenFile.getline(buf, 10000);
	string FileString;
	FileString.append(buf);
	vect_string.push_back(FileString);
	if (OpenFile.eof())
	break;
	} while (!OpenFile.eof());

	int pos_y = 0;
	for (auto & ite : vect_string)
	{
	auto a_str = StringHelper::splitString(ite, " ");
	int pos_x = 0;
	for (auto & str : a_str)
	{
	if (m_Blocks.find(pos_x) != m_Blocks.end())
	{
	if (m_Blocks.at(pos_x).find(pos_y) != m_Blocks.at(pos_x).end())
	{
	auto block = m_Blocks.at(pos_x).at(pos_y);
	int type = 1;
	if (str == m_BlockTypeString.at(1))
	{
	type = 1;
	}
	else if (str == m_BlockTypeString.at(2))
	{
	type = 2;
	}
	else if (str == m_BlockTypeString.at(3))
	{
	type = 3;
	}
	block->SetBlockType(type);
	}
	}
	pos_x++;
	}
	pos_y++;

	}*/
}
void  HelloWorld::EditButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		m_LayoutBType->setVisible(true);
		m_LayoutChoice->setVisible(false);
		m_CurType = 0;
		m_IsEditModel = true;
	}
}

void HelloWorld::ChoiceMapButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		m_LayoutMapChoice->setVisible(true);
		m_LayoutChoice->setVisible(false);
	}
}

void  HelloWorld::SaveButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		SaveFileToMsk("MapBackground/map001.msk");
	}
}
void  HelloWorld::TypeButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		auto btn = (Button *)pSender;
		m_CurType = btn->getTag();
		m_IsEditModel = true;
	}
	else if (eventType == ui::Widget::TouchEventType::BEGAN)
	{
		m_IsEditModel = false;
	}
	else if (eventType == ui::Widget::TouchEventType::CANCELED)
	{
		m_IsEditModel = true;
	}
}
void  HelloWorld::CloseEditButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		m_LayoutBType->setVisible(false);
		m_LayoutChoice->setVisible(true);
		
	}
	else if(eventType == ui::Widget::TouchEventType::BEGAN)
	{
		m_IsEditModel = false;
	}
	else if (eventType == ui::Widget::TouchEventType::CANCELED)
	{
		m_IsEditModel = true;
	}
}

void HelloWorld::CloseMapChoiceButtonEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		m_LayoutMapChoice->setVisible(false);
		m_LayoutChoice->setVisible(true);

	}
}

void HelloWorld::TouchEvent(ui::Widget::TouchEventType touch_type, Vec2 touch_point)
{
	if (/*m_IsEditModel == */true)
	{
		switch (touch_type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			//CCLOG("[ControlTouchType_Began ] TPOINT[%0.2f : %0.2f]", touch_point.x, touch_point.y);
			m_CurPoint = touch_point;
			break;
		case ui::Widget::TouchEventType::MOVED:
			//CCLOG("[ControlTouchType_Moved ] TPOINT[%0.2f : %0.2f]", touch_point.x, touch_point.y);
			m_LastPoint = m_CurPoint;
			m_CurPoint = touch_point;
			m_IsMove = true;
			m_LayoutBack->setPosition(m_LayoutBack->getPosition() + m_CurPoint - m_LastPoint);
			m_LayoutBlockPoint->setPosition(m_LayoutBlockPoint->getPosition() + m_CurPoint - m_LastPoint);
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

void HelloWorld::MouseMove(Event * evnet)
{
	//CCLog("mouse  move!!!!!!!!!!!!!!!!!!!!!!");
	EventMouse* e = (EventMouse*)evnet;
	//CCLOG("%0.2f %0.2f",e->getCursorX() ,e->getCursorY());//
	float x = e->getCursorX();
	float y = e->getCursorY();
	m_CurMousePoint.x = x;
	m_CurMousePoint.y = y;
	if (m_IsBoolPressed == true && m_IsEditModel == true)
	{
		auto move_point = m_LayoutBack->getPosition();
		int wdith_num = (int)(m_CurMousePoint.x - move_point.x) / (__BlockSize*m_CurScale);
		int height_num = (int)(m_CurMousePoint.y - move_point.y) / (__BlockSize*m_CurScale);
		if (m_Blocks.find(wdith_num) != m_Blocks.end())
		{
			if (m_Blocks.at(wdith_num).find(height_num) != m_Blocks.at(wdith_num).end())
			{
				auto block = m_Blocks.at(wdith_num).at(height_num);
				block->SetBlockType(m_CurType);
			}

		}
	}
	if (__OpenFunction_1)
	{
		PointBlock();
	}
	
}

void HelloWorld::MouseScroll(Event * evnet)
{
	EventMouse* e = (EventMouse*)evnet;
	CCLOG("%0.2f %0.2f",e->getScrollX() ,e->getScrollY());//
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
	m_LayoutBack->setScale(m_CurScale);
	m_LayoutBlockPoint->setScale(m_CurScale);
	if (is_c)
	{
		m_LayoutBack->setPosition(m_LayoutBack->getPosition() + m_LayoutBack->getContentSize() * (o_s - m_CurScale));
		m_LayoutBlockPoint->setPosition(m_LayoutBlockPoint->getPosition() + m_LayoutBack->getContentSize() * (o_s - m_CurScale));
	}
}

//捕捉监听
void HelloWorld::OnKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet)
{
	if (keycode == EventKeyboard::KeyCode::KEY_CTRL && m_IsEditModel == true)  //返回
	{
		CCLOG("%d",(int)keycode);
		auto move_point = m_LayoutBack->getPosition();
		int wdith_num = (int)(m_CurMousePoint.x - move_point.x) / (__BlockSize*m_CurScale);
		int height_num = (int)(m_CurMousePoint.y - move_point.y) / (__BlockSize*m_CurScale);
		if (m_Blocks.find(wdith_num) != m_Blocks.end())
		{
			if (m_Blocks.at(wdith_num).find(height_num) != m_Blocks.at(wdith_num).end())
			{
				auto block = m_Blocks.at(wdith_num).at(height_num);
				block->SetBlockType(m_CurType);
			}

		}
		if (m_Slots->m_IsOver == true)
		{
			m_Slots->RunToIndex(1);
		}
		else
		{
			if (m_Slots->m_IsStop == true)
			{
				m_Slots->Contine();
			}
			else
			{
				m_Slots->Stop();
			}
		}
		//m_Bug->MoveToPoint(Point(m_CurMousePoint.x - move_point.x, m_CurMousePoint.y - move_point.y));
	}
	

	if (keycode == EventKeyboard::KeyCode::KEY_CTRL)  //返回
	{
		m_IsBoolPressed = false;
	}
	//--
	
}
void HelloWorld::OnKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet)
{
	if (keycode == EventKeyboard::KeyCode::KEY_CTRL && m_IsEditModel == true)
	{
		m_IsBoolPressed = true;
		CCLOG("[HelloWorld::OnKeyPressed]");
	}

	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		SaveFileToMsk("MapBackground/map001.msk");
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
		SaveFileToMsk("");
		CCLOG("Save !!!!!!!!!");
	}

}
void HelloWorld::ChoiceMapTypeButtonEvent(ui::Widget::TouchEventType touch_type, Vec2 touch_point)
{

}


bool HelloWorld::SaveFileToMsk(const std::string& pFileName)
{
// 	ofstream myfile(pFileName, ios::out);
// 	if (!myfile)
// 	{
// 		CCLOG("[SaveFileToMsk]:	error !");
// 		return false;
// 	}
// 	else
// 	{
// 		/*for (auto & ite : pContentString)
// 		{
// 		myfile << ite << endl;
// 		}*/
// 		for (int pos_y = 0; pos_y < m_HeightNum; pos_y++)
// 		{
// 			string pos_data = "";
// 			for (int pos_x = 0; pos_x < m_WdithNum; pos_x ++)
// 			{
// 				if (m_Blocks.find(pos_x) != m_Blocks.end())
// 				{
// 					if (m_Blocks.at(pos_x).find(pos_y) != m_Blocks.at(pos_x).end())
// 					{
// 						auto block = m_Blocks.at(pos_x).at(pos_y);
// 						pos_data.append(StringUtils::format("%-6s", m_BlockTypeString.at(block->m_Type).c_str()));
// 					}
// 				}
// 			}
// 			myfile << pos_data << endl;
// 		}
// 		myfile.close();
// 		CCLOG("[SaveFileToMsk]:	[%s] Save Is Success !",pFileName.c_str());
// 	}
	uint8_t *new_data = new uint8_t[m_WdithNum*m_HeightNum];

	for (int pos_y = 0; pos_y < m_HeightNum; pos_y++)
	{
		for (int pos_x = 0; pos_x < m_WdithNum; pos_x++)
		{
			new_data[pos_y * m_WdithNum + pos_x] = (uint8_t)m_Blocks.at(pos_x).at(pos_y)->m_Type;
		}
	}
	FILE *fp = fopen("MapBackground/map001.msk", "wb");
	if (fp != NULL) {
		fwrite(new_data, 1, mapsize.width * mapsize.height, fp);
		fclose(fp);
	}
	else {
		//MessageBox(CCEGLView::sharedOpenGLView()->getHWnd(), L"无法创建MASK数据文件", L"出错啦", 0);
	}
	return true;
}


void HelloWorld::PointBlock()
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


Block * HelloWorld::GetCurPointBlock()
{
	Block * point_block = nullptr;
	auto move_point = m_LayoutBack->getPosition();
	int wdith_num = (int)(m_CurMousePoint.x - move_point.x) / (__BlockSize*m_CurScale);
	int height_num = (int)(m_CurMousePoint.y - move_point.y) / (__BlockSize*m_CurScale);
	if (m_Blocks.find(wdith_num) != m_Blocks.end())
	{
		if (m_Blocks.at(wdith_num).find(height_num) != m_Blocks.at(wdith_num).end())
		{
			point_block = m_Blocks.at(wdith_num).at(height_num);
		}

	}
	return point_block;
}
void HelloWorld::update(float delta)
{
	ProgressTimer * pro = (ProgressTimer *)this->getChildByName("proTimer");
	int pre = pro->getPercentage();
	pre++;
	if (pre > 100){
		pre = 0;
	}
	pro->setPercentage(pre);
}
//--3DpageView协议
Layout * HelloWorld::GetPageLayoutWithIndex(int index){
	Size size = m_ListView3D->GetOnePageSize();
	Layout * lay = Layout::create();
	lay->setContentSize(size);
	ImageView * image = ImageView::create();
	image->loadTexture("WaterShader/water.png", TextureResType::LOCAL);
	image->setAnchorPoint(Vec2(0.5,0.5));
	image->setScale(0.2);
	image->setPosition(size / 2);
	lay->addChild(image);
	return lay;
}
void HelloWorld::CurShowPage(Layout * layout, int index){

}

void HelloWorld::AnimaCallFunc(std::string name, int index, Node * node)
{
	CCLOG("[%s] -- [%d]",name.c_str(),index);
	EffectArmature * effctAnima = (EffectArmature *)node;
	if (index == 2){
		//effctAnima->setEffect(EffectOutline::create());
	}
	else if (index == 4)
	{
		effctAnima->setEffect(EffectOrigin::create());
	}
}