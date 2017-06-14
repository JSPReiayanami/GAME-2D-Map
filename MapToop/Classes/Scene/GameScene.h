#ifndef __GameScene_H__
#define __GameScene_H__
#include "Config/Head.h"
class MapBase;
class GameLogicDelegate;
class Interface_Entity;
class GameScene: public Layout
{
public:
	enum SceneType
	{
		StoryGame = 1,
		Ui
	};
public:
	GameScene();
	~GameScene();
	CREATE_FUNC(GameScene);
	static Scene* GetGameScene(bool isPhysics = false);
	static GameScene * GetInstance();
	static void RunGame();
	static void RunPhysicsGame();
	virtual bool init();
public:
	void SetPhysicsWorld(PhysicsWorld * world);
	PhysicsWorld * GetPhysicsWorld();
	int PushIntoMap(int mapId);
	int PushInterface(Interface_Entity * lay);
	int PushInterface(const string & name);
	int PopInterface(const string & name, bool isRemove = true);
	int PopInterface(Interface_Entity * lay, bool isRemove = true);
	int SetInterfaceToTop(const string & name);
	bool ExitMap();
	MapBase * GetCurMap();
	MapBase * GetMapWithId(int mapId);
	GameLogicDelegate * GetGameLogic();
	Interface_Entity * GetUiLayout(string name);
private:
	static Scene * m_Scene;
	static GameScene * m_GameScene;
private:
	Layout * m_LayoutMap;
	Layout * m_LayoutUi;
	Layout * m_LayoutTop;
	SceneType m_CurShowType;
	map<int, MapBase *> m_Maps;
	map<string, Interface_Entity *> m_UiLayout;
	int m_CurMapId;
	GameLogicDelegate * m_GameLogic;
	bool m_PressedALT;
	bool m_PressedM;
	bool m_PressedC;
	bool m_PressedG;
	string m_TopInterfaceName;
	cocos2d::PhysicsWorld * m_world;
	bool m_BoolShowDebug;
private:
	int StartIntoMap(int mapId);
	void ClearAllMap();
	void OnKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet);
	void OnKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *evnet);
private:
	virtual void update(float delta);
};



#endif // __GameScene_H__