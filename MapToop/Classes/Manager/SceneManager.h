#ifndef __SCENEMANGER_H_
#define __SCENEMANGER_H_
#include "cocos2d.h"
#include <iostream>
class SceneManager :
	public cocos2d::Node
{
public:
	SceneManager();
	~SceneManager();
	/*
	*固定创建方法
	*/
	static cocos2d::Scene * createScene();
	virtual bool init();
	CREATE_FUNC(SceneManager);
	/*
	* 需要切换的场景的枚举--可自行扩张（replaceSceneWithType），扩展即可
	*/
	enum SceneType
	{
		SceneType_Login = 0,
		SceneType_Menu,
		SceneType_ChoiceGame,
		SceneType_CardPackg,
		SceneType_Intorduce,
		SceneType_Map,
		SceneType_Game,
		SceneType_SkillLearing,
		SceneType_Other,
        SceneType_Game_1,
        SceneType_Game_2,
        SceneType_Game_3,
        SceneType_Game_4,
        SceneType_Game_5,
        SceneType_Game_6,
        SceneType_Game_7,
        SceneType_Game_8
	};
	/*
	* 需要切换的场景的动画枚举--可自行扩张，在（replaceSceneAnimation）里面实现动画即可
	*/
	enum ReplaceAnimationType
	{
		Animation_Junmp = 0,
        Animation_FlipX,
		Animation_NUll
	};
	/*
	* 获取单例场景管理者
	*/
	static SceneManager * getInstance();
	/*
		@ 函数名:replaceSceneWithType 根据枚举值切换场景
		@ 参数 ：type 场景枚举值，切换枚举值代表的场景
		@ 参数 ：aniType 动画枚举值，切换场景时所需要的动画，默认是没动画
		@ 参数 ：time 切换场景所需的时间，默认是 1秒
	*/
	void replaceSceneWithType(SceneType type, ReplaceAnimationType aniType = Animation_NUll, float time = 1.0);

	/*
	@ 函数名:replaceSceneWithType 根据枚举值切换场景
	@ 参数 ：type 场景枚举值，切换枚举值代表的场景
	@ 参数 ：aniType 动画枚举值，切换场景时所需要的动画，默认是没动画
	@ 参数 ：time 切换场景所需的时间，默认是 1秒
	*/
	Node * replaceSceneWithType_NODE(SceneType type, ReplaceAnimationType aniType = Animation_NUll,const float time = 1.0,const std::string & node_name = "");
	/*
		@ 函数名:replaceSceneWithType 根据枚举值切换动画场景
		@ 参数 ：aniType 动画枚举值，切换场景时所需要的动画
		@ 参数 ：scene 场景
		@ 参数 ：time 切换场景所需的时间
	*/
	cocos2d::Scene * replaceSceneAnimation(ReplaceAnimationType aniType, cocos2d::Scene * scene, float time);
	cocos2d::Scene * m_OperationScene;
	/*
		*单例的指针
	*/
	static SceneManager * s_SceneManger;
	float m_SceneOffeSet;
};
#endif

