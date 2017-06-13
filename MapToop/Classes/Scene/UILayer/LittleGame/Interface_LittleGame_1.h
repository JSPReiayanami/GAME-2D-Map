#ifndef __Interface_LittleGame_1_H__
#define __Interface_LittleGame_1_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
#include "Scene/GameComponent/GameButton.h"

class Interface_LittleGame_1 : public Interface_Entity
{
public:
	Interface_LittleGame_1();
	~Interface_LittleGame_1();
	CREATE_FUNC(Interface_LittleGame_1);
	CREATEPush_FUNC(Interface_LittleGame_1, "Interface_LittleGame_1");
	bool init();
	/*
	@�����Լ�������
	*/
	void SetInterfaceName(const string & name = "Interface_LittleGame_1");
	/*
	@��ʼ����Դ
	*/
	void InitSrcLayout();
	/*
	@��ʼ���ؼ�
	*/
	void InitWidget();
	/*
	@��ʼ����ť�ص�
	*/
	void InitButtonEvent();

	void OnIntoScene();
	void OnExitScene();
	void OnClose();
public:
	void InitGame(int level);
	virtual void update(float delta);
public:
	int m_GameLevel;
	int m_GameState;
	Vec2 m_VecNum;
	Layout * m_LayoutView;
	vector<GameButton *> m_AllGameButtons;
	string m_ViewRes_1;
	string m_ViewRes_2;
	Color3B m_ViewColor_1;
	Color3B m_ViewColor_2;
	GameButton * m_ChangeGB;
	GameButton * m_ChangePreGB;
	bool m_CanTouch;
	float m_ChangeTime;
	float m_Time;
};


#endif //!__Interface_LittleGame_1_H__zhuan