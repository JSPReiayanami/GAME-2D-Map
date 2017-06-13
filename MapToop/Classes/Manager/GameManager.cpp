#include "GameManager.h"

//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/SceneManager.h"
#include "Manager/DataManager.h"
#else
#include "SceneManager.h"
#include "DataManager.h"
#endif//判断路径的引用
USING_NS_CC;
GameManager::GameManager():
m_CurType(GameStateType_Null),
m_InterfaceGame(nullptr),
m_GameLevelNum(1),
m_CurChoicePropId(0),
m_DosingTag(0),
m_GamePizza(nullptr)
{
}


GameManager::~GameManager()
{
}

//===== get the manger ====
GameManager * GameManager::m_GameManager = nullptr;
GameManager * GameManager::getInstance(){
	if (m_GameManager == nullptr)
	{
		m_GameManager = new GameManager();
	}
	return m_GameManager;
}

//---
Scene * GameManager::createScene(){
	auto scene = Scene::create();
	auto manger = GameManager::create();
	scene->addChild(manger);
	return scene;
}
bool GameManager::init(){
	if (!Node::init())
	{
		return false;
	}
	
	return true;
}
/*
	@ 函数名:SetGameState 设置当前游戏状态
	@ 参数 ：GameStateType 游戏状态枚举值
*/
void GameManager::SetGameState(GameStateType g_type)
{
	if (m_CurType != g_type)
	{
		m_CurType = g_type;
		switch (g_type)
		{
		case GameManager::GameStateType_Null:

			break;
		case GameManager::GameStateType_Scene_1:
			//游戏场景 1
            StartScene_1();
			break;
        case GameManager::GameStateType_Scene_2:
            //游戏场景 2
            StartScene_2();
            break;
        case GameManager::GameStateType_Scene_3:
            //游戏场景 3
            StartScene_3();
            break;
        case GameManager::GameStateType_Scene_4:
            //游戏场景 4
            StartScene_4();
            break;
        case GameManager::GameStateType_Scene_5:
            //游戏场景 5
            StartScene_5();
            break;
        case GameManager::GameStateType_Scene_6:
            //游戏场景 6
            StartScene_6();
            break;
        case GameManager::GameStateType_Scene_7:
            //游戏场景 7
            StartScene_7();
            break;
        case GameManager::GameStateType_Scene_8:
            //游戏场景 8
            StartScene_8();
            break;
		case GameManager::GameStateType_Stop:
			//游戏暂停
			GameStop();
			break;
		case GameManager::GameStateType_Continue:
			//游戏继续
			GameContinue();
			break;
		case GameManager::GameStateType_End:
			//游戏结束
			GameEnd();
			break;
		case GameManager::GameStateType_Exit:
			//退出游戏
			GameExit();
			break;
		case GameManager::GameStateType_Pass:
			//游戏过关
			GamePass();
			break;
		default:
			break;
		}
	}
}

/*
	@游戏开始
*/
void GameManager::GameStart()
{
	if (nullptr != m_InterfaceGame)
	{
		GameEnd();
	}
	
	/*m_InterfaceGame = (Interface_Game *)SceneManager::getInstance()->replaceSceneWithType_NODE(SceneManager::SceneType_Game, SceneManager::Animation_NUll, 1, "Interface_Game");
	m_InterfaceGame->InitGameWithLevel();*/
	InitGameDataOnChangeLevel();
	
}
/*
@游戏暂停
*/
void GameManager::GameStop()
{

}
/*
@游戏继续
*/
void GameManager::GameContinue()
{

}
/*
@游戏过关
*/
void GameManager::GamePass()
{

}
/*
@游戏结束
*/
void GameManager::GameEnd()
{
	
}
/*
@退出游戏
*/
void GameManager::GameExit()
{
	m_InterfaceGame = nullptr;
	m_CurType = GameStateType_Null;
	SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType_Menu);
}
/*
@在每次更换关卡，需要刷新或初始化的数据
*/
void GameManager::InitGameDataOnChangeLevel()
{

	
}
/*
@进入下一关
*/
void GameManager::GoToNextLevel()
{
    
}

/*
@进入上一关
*/
void GameManager::GoToUpLevel()
{

}
/*
 @进入游戏场景
 */
void GameManager::StartScene_1()
{
    //SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType::SceneType_Game_1);
    SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType::SceneType_Game_1,SceneManager::Animation_FlipX,1);
    SetDosingTag(0);
}
void GameManager::StartScene_2()
{
    SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType::SceneType_Game_2,SceneManager::Animation_FlipX,1);
}
void GameManager::StartScene_3()
{
    SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType::SceneType_Game_3,SceneManager::Animation_FlipX,1);
}
void GameManager::StartScene_4()
{
    SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType::SceneType_Game_4,SceneManager::Animation_FlipX,1);
}
void GameManager::StartScene_5()
{
    SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType::SceneType_Game_5,SceneManager::Animation_FlipX,1);
}
void GameManager::StartScene_6()
{
    
}
void GameManager::StartScene_7()
{
    SceneManager::getInstance()->replaceSceneWithType(SceneManager::SceneType::SceneType_Game_7,SceneManager::Animation_FlipX,1);
}
void GameManager::StartScene_8()
{
    
}
void GameManager::StartScene_9()
{
    
}
void GameManager::StartScene_10()
{
    
}
/*
 @设置配料的tag
 */
void GameManager::SetDosingTag(const int & d_tag)
{
    m_DosingTag = d_tag;
}
int GameManager::GetDosingTag()
{
    return m_DosingTag;
}
void GameManager::SetPizzaImageView(ImageView * pizza)
{
    if(m_GamePizza != nullptr)
    {
        m_GamePizza->release();
    }
    m_GamePizza = pizza;
    m_GamePizza->retain();
}
