#ifndef __GAMEMANGER_H_
#define __GAMEMANGER_H_
#include "Head.h"
class Interface_Game;
class Game_Item;
class GameManager :
	public cocos2d::Node
{
public:
	GameManager();
	~GameManager();
	/*
		@固定创建方法
	*/
	static cocos2d::Scene * createScene();
	virtual bool init();
	CREATE_FUNC(GameManager);
	/*
		@游戏场景状态枚举值	
	*/
	enum GameStateType
	{
		GameStateType_Null = -1, //没有游戏
		GameStateType_Scene_1 = 1, //游戏开始
        GameStateType_Scene_2 , //游戏开始
        GameStateType_Scene_3 , //游戏开始
        GameStateType_Scene_4 , //游戏开始
        GameStateType_Scene_5 , //游戏开始
        GameStateType_Scene_6 , //游戏开始
        GameStateType_Scene_7 , //游戏开始
        GameStateType_Scene_8 , //游戏开始
        GameStateType_Scene_9 , //游戏开始
        GameStateType_Scene_10 , //游戏开始
		GameStateType_Stop, //暂停当前游戏
		GameStateType_Continue,//继续游戏
		GameStateType_End, //游戏结束
		GameStateType_Exit,//退出游戏
		GameStateType_Pass,//游戏过关
	};
	/*
		@获取单例场景管理者
	*/
	static GameManager * getInstance();
	/*
		@ 函数名:SetGameState 设置当前游戏状态
		@ 参数 ：GameStateType 游戏状态枚举值
	*/
	void SetGameState(GameStateType g_type);
	/*
		@单例的指针
	*/
	static GameManager * m_GameManager;
	/*
		@属性
	*/
	GameStateType m_CurType;
	Interface_Game * m_InterfaceGame;
    ImageView * m_GamePizza;
	int m_GameLevelNum;
	int m_CurChoicePropId;
    int m_DosingTag;
	/*
		@游戏开始
	*/
	void GameStart();
	/*
		@游戏暂停
	*/
	void GameStop();
	/*
		@游戏继续
	*/
	void GameContinue();
	/*
		@游戏过关
	*/
	void GamePass();
	/*
		@游戏结束
	*/
	void GameEnd();
	/*
		@退出游戏
	*/
	void GameExit();
	/*
		@在每次更换关卡，需要刷新或初始化的数据
	*/
	void InitGameDataOnChangeLevel();
	/*
		@进入下一关
	*/
	void GoToNextLevel();
	/*
		@进入上一关
	*/
	void GoToUpLevel();
	/*
        @进入游戏场景
     */
    void StartScene_1();
    void StartScene_2();
    void StartScene_3();
    void StartScene_4();
    void StartScene_5();
    void StartScene_6();
    void StartScene_7();
    void StartScene_8();
    void StartScene_9();
    void StartScene_10();
    /*
        @设置配料的tag
    */
    void SetDosingTag(const int & d_tag);
    int GetDosingTag();
    void SetPizzaImageView(ImageView * pizza);

};
#endif

