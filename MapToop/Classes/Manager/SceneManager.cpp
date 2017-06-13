#include "SceneManager.h"
#include "Head.h"
//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Config/SceneManagerCofing.h"
#else
#include "SceneManagerCofing.h"
#endif//判断路径的引用

USING_NS_CC;
SceneManager::SceneManager():
m_OperationScene(nullptr),
m_SceneOffeSet(0)
{
}


SceneManager::~SceneManager()
{
}

//===== get the manger ====
SceneManager * SceneManager::s_SceneManger = nullptr;
SceneManager * SceneManager::getInstance(){
	if (s_SceneManger == nullptr)
	{
		s_SceneManger = new SceneManager();
	}
	return s_SceneManger;
}

//---
Scene * SceneManager::createScene(){
	auto scene = Scene::create();
	auto manger = SceneManager::create();
	scene->addChild(manger);
	return scene;
}
bool SceneManager::init(){
	if (!Node::init())
	{
		return false;
	}
	return true;
}
//===== replace into all scene==
void SceneManager::replaceSceneWithType(SceneType type, ReplaceAnimationType aniType, float time){
	m_OperationScene = nullptr;
	switch (type)
	{
	case SceneManager::SceneType_Login:
          //m_OperationScene = Interface_Login::createScene();
         break;
	case SceneManager::SceneType_Game_1:
        m_OperationScene = Interface_Game_Scene_1::createScene();
        break;
    case SceneManager::SceneType_Game_2:
        m_OperationScene = Interface_Game_Scene_2::createScene();
        break;
    case SceneManager::SceneType_Game_3:
        m_OperationScene = Interface_Game_Scene_3::createScene();
        break;
    case SceneManager::SceneType_Game_4:
        m_OperationScene = Interface_Game_Scene_4::createScene();
        break;
    case SceneManager::SceneType_Game_5:
        m_OperationScene = Interface_Game_Scene_5::createScene();
        break;
    case SceneManager::SceneType_Game_6:
        m_OperationScene = Interface_Game_Scene_5::createScene();
        break;
    case SceneManager::SceneType_Game_7:
        m_OperationScene = Interface_Game_Scene_7::createScene();
            break;
	case  SceneManager::SceneType_Menu:
      //m_OperationScene = Interface_Main::createScene();
         break;
	case SceneManager::SceneType_Other:
	{
										 break;
	}
	default:
		break;
	}
	//===
	if (m_OperationScene != nullptr)
	{
		m_OperationScene->setPositionY(m_OperationScene->getPositionY()+(m_SceneOffeSet));
		auto AniScene = this->replaceSceneAnimation(aniType, m_OperationScene, time);
		//===go to the scene
		if (!Director::getInstance()->getRunningScene())
		{
			Director::getInstance()->runWithScene(AniScene);
		}
		else
		{
			Director::getInstance()->replaceScene(AniScene);
		}
	}
	
}
/*
* 函数名:replaceSceneWithType 根据枚举值切换动画场景
* 参数 ：aniType 动画枚举值，切换场景时所需要的动画
* 参数 ：scene 场景
* 参数 ：time 切换场景所需的时间
*/
Scene * SceneManager::replaceSceneAnimation(ReplaceAnimationType aniType, Scene * scene, float time){
	Scene * trScene = nullptr;
	switch (aniType)
	{
	case SceneManager::Animation_Junmp:
	{
										 trScene = TransitionJumpZoom::create(time, scene);
										 break;
	}
    case SceneManager::Animation_FlipX:
    {
                                        trScene =  TransitionFlipX::create(time, scene);
        break;
    }
	case SceneManager::Animation_NUll:
	{
										trScene = scene;
										break;
	}
	default:
		trScene = scene;
		break;
	}
    
    
	return trScene;
}

/*
@ 函数名:replaceSceneWithType 根据枚举值切换场景
@ 参数 ：type 场景枚举值，切换枚举值代表的场景
@ 参数 ：aniType 动画枚举值，切换场景时所需要的动画，默认是没动画
@ 参数 ：time 切换场景所需的时间，默认是 1秒
*/
Node * SceneManager::replaceSceneWithType_NODE(SceneType type, ReplaceAnimationType aniType,const float time, const std::string & node_name)
{
	Node * interface_e = nullptr;
	replaceSceneWithType(type, aniType, time);
	if (node_name != "" &&  nullptr != m_OperationScene)
	{
		//CCLOG("[SceneManager::replaceSceneWithType_NODE] Find [%s] Node ",node_name);
		interface_e = m_OperationScene->getChildByName(node_name);
	}
	return interface_e;
}