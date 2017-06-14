#include "GameScene.h"
#include "MapRoot.h"
#include "GameLogicDelegate.h"
#include "Entity/Interface_Entity.h"
#include "Scene/UILayer/MapCreater/Interface_MapCreate.h"
#include "Manager/StoryDirector.h"
#include "Config/Head.h"
Scene * GameScene::m_Scene = nullptr;
GameScene * GameScene::m_GameScene = nullptr;
GameScene::GameScene()
{
	m_GameLogic = new GameLogicDelegate(this);
	m_PressedALT = false;
	m_PressedM = false;
	m_PressedC = false;
	m_TopInterfaceName = "";
	m_BoolShowDebug = false;
	m_world = nullptr;
}

GameScene::~GameScene()
{
	CC_SAFE_DELETE(m_GameLogic);
}

Scene* GameScene::GetGameScene(bool isPhysics /*= false*/)
{
	if (m_Scene == nullptr)
	{
		if (true == isPhysics) {
			m_Scene = Scene::createWithPhysics();
			GameScene * gameScene = GameScene::GetInstance();
			m_Scene->addChild(gameScene);

			gameScene->SetPhysicsWorld(m_Scene->getPhysicsWorld());
			//Åö×²±ß¿ò
			auto edgeSp = Sprite::create();
			auto boundBody = PhysicsBody::createEdgeBox(__VisibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
			edgeSp->setPosition(Point(__VisibleSize_Width / 2, __VisibleSize_Height / 2));
			edgeSp->setPhysicsBody(boundBody);
			gameScene->addChild(edgeSp);
		}
		else
		{
			m_Scene = Scene::create();
			GameScene * gameScene = GameScene::GetInstance();
			m_Scene->addChild(gameScene);
		}
		
	}
	return m_Scene;
}

GameScene * GameScene::GetInstance()
{
	if (m_GameScene == nullptr)
	{
		m_GameScene = GameScene::create();
	}
	return m_GameScene;
}

void GameScene::RunGame()
{
	if (m_Scene == nullptr)
	{
		auto director = Director::getInstance();
		director->runWithScene(GetGameScene());
	}
}
void GameScene::RunPhysicsGame()
{
	if (m_Scene == nullptr)
	{
		auto director = Director::getInstance();
		director->runWithScene(GetGameScene(true));
	}
}
#define  InitLayoutForMap(who,zder,father)\
	who = Layout::create(); \
	who->setLocalZOrder(zder++); \
	father->addChild(who);

bool GameScene::init()
{
	if (!Layout::init())
	{
		return false;
	}

	int zOrder = 1;
	InitLayoutForMap(m_LayoutMap, zOrder, this)
	InitLayoutForMap(m_LayoutUi, zOrder, this)
	InitLayoutForMap(m_LayoutTop, zOrder, this)
	//×¢²á²¶×½¼àÌı
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::OnKeyReleased, this);
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(GameScene::OnKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	StoryDirector::GetInstance()->CreateNewGameSave();
	this->scheduleUpdate();
	return true;
}
//=====MAP

int GameScene::StartIntoMap(int mapId)
{
	ClearAllMap();
	MapRoot * mr = MapRoot::Create(mapId);
	if (mr == nullptr)
	{
		return -1;
	}
	mr->setTag(mapId);
	m_Maps.insert(std::make_pair(mapId, mr));
	m_LayoutMap->addChild(mr);
	m_CurMapId = mapId;
	m_CurShowType = StoryGame;
	return mapId;
}
void GameScene::SetPhysicsWorld(PhysicsWorld * world)
{
	m_world = world;
}
PhysicsWorld * GameScene::GetPhysicsWorld()
{
	return m_world;
}
int GameScene::PushIntoMap(int mapId)
{
	if (mapId == m_CurMapId)
	{
		return m_CurMapId;
	}

	if (m_CurShowType != StoryGame)
	{
		return StartIntoMap(mapId);
	}
	//===±£´æÉÏÒ»¸öµØÍ¼
	MapBase * mapBLast = GetCurMap();
	MapBase * mapB = nullptr;
	mapB = GetMapWithId(mapId);
	if (mapB != nullptr)
	{
		m_LayoutMap->addChild(mapB);
		mapB->release();
	}
	else
	{
		MapRoot * mr = MapRoot::Create(mapId);
		if (mr == nullptr)
		{
			return -1;
		}
		mr->setTag(mapId);
		m_Maps.insert(std::make_pair(mapId, mr));
		m_LayoutMap->addChild(mr);
	}
	m_CurMapId = mapId;
	//===±£´æÉÏÒ»¸öµØÍ¼
	if (mapBLast != nullptr)
	{
		mapBLast->retain();
		mapBLast->removeFromParent();
	}
	return mapId;
}

int GameScene::PushInterface(Interface_Entity * lay)
{
	if (lay != nullptr)
	{
		string name = lay->getName();
		Interface_Entity * laylast = GetUiLayout(name);
		if (laylast == nullptr)
		{
			lay->SetPhysicsWorld(this->m_world);
			m_UiLayout.insert(make_pair(name, lay));
			m_LayoutUi->addChild(lay);
			lay->OnIntoScene();
			lay->retain();
			return 0;
		}
		else
		{
			laylast->OnIntoScene();
			return 0;
		}
		return 1;
	}
	return 2;
}

int GameScene::PushInterface(const string & name)
{
	Interface_Entity * lay = GetUiLayout(name);
	if (lay != nullptr)
	{
		lay->OnIntoScene();
		return 0;
	}
	return 1;
}
int GameScene::PopInterface( const string & name, bool isRemove )
{
	Interface_Entity * lay = GetUiLayout(name);
	if (lay != nullptr)
	{
		lay->OnExitScene();
		if (isRemove == true)
		{
			lay->release();
			lay->OnClose();
			lay->removeFromParent();
			m_UiLayout.erase(m_UiLayout.find(name));
		}
		return 0;
	}
	return 1;
}

int GameScene::PopInterface(Interface_Entity * lay, bool isRemove)
{
	if (lay != nullptr)
	{
		string name = lay->getName();
		return PopInterface(name,isRemove);
	}
	return 2;
}

int GameScene::SetInterfaceToTop(const string & name)
{
	Interface_Entity * lay = nullptr;
	lay = GetUiLayout(name);
	if (lay)
	{
		lay->setLocalZOrder(999);
	}
	else {
		return 1;
	}
	lay = GetUiLayout(m_TopInterfaceName);
	if (lay)
	{
		lay->setLocalZOrder(998);
	}
	

	return 0;
}

bool GameScene::ExitMap()
{
	return true;
}

MapBase * GameScene::GetCurMap()
{
	MapBase * mapb = GetMapWithId(m_CurMapId);
	return mapb;
}

MapBase * GameScene::GetMapWithId(int mapId)
{
	MapBase * mapb = nullptr;
	if (m_Maps.find(mapId) != m_Maps.end())
	{
		mapb = m_Maps.at(mapId);
	}
	return mapb;
}
GameLogicDelegate * GameScene::GetGameLogic()
{
	return m_GameLogic;
}

Interface_Entity * GameScene::GetUiLayout(string name)
{
	Interface_Entity * lay = nullptr;
	if (m_UiLayout.find(name) != m_UiLayout.end())
	{
		lay = m_UiLayout.at(name);
	}
	return lay;
}
void GameScene::ClearAllMap()
{
	MapBase * mapB = GetCurMap();
	if (mapB != nullptr)
	{
		mapB->retain();
		mapB->removeFromParent();
	}
	for (auto & mapB2 : m_Maps)
	{
		mapB2.second->release();
	}
	m_Maps.clear();
}
//=====MAP
void GameScene::OnKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet)
{
	if (keycode == EventKeyboard::KeyCode::KEY_ALT)
	{
		m_PressedALT = false;
	}
	if (keycode == EventKeyboard::KeyCode::KEY_M)
	{
		m_PressedM = false;
	}
	if (keycode == EventKeyboard::KeyCode::KEY_C)
	{
		m_PressedC = false;
	}
	if (keycode == EventKeyboard::KeyCode::KEY_G)
	{
		m_PressedG = false;
	}
}
void GameScene::OnKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet)
{
	if (keycode == EventKeyboard::KeyCode::KEY_F1)
	{
		if (nullptr != m_world) 
		{
			m_BoolShowDebug = !m_BoolShowDebug;
			if (true == m_BoolShowDebug) 
			{
				m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
			}
			else
			{
				m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
			}
		}
	}

	if (keycode == EventKeyboard::KeyCode::KEY_ALT)
	{
		m_PressedALT = true;
	}
	if (keycode == EventKeyboard::KeyCode::KEY_M)
	{
		m_PressedM = true;
	}
	if (keycode == EventKeyboard::KeyCode::KEY_C)
	{
		m_PressedC = true;
	}
	if (keycode == EventKeyboard::KeyCode::KEY_G)
	{
		m_PressedG = true;
	}
	if (m_PressedM && m_PressedALT)
	{
		Interface_MapCreate::CreateToScene();
	}
	if (m_PressedALT && m_PressedC)
	{
		GameScene::PopInterface("Interface_MapCreate");
	}
	if (m_PressedALT && m_PressedG)
	{
		GameScene::GetInstance()->PushIntoMap(60001001);
	}
	
}

void GameScene::update(float delta)
{
	StoryDirector::GetInstance()->update(delta);
}