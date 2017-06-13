#ifndef __Interface_Dialog_H__
#define __Interface_Dialog_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class RichTextEntity;
class CfgTalkData;
class Interface_Dialog : public Interface_Entity
{
public:
	Interface_Dialog();
	~Interface_Dialog();
	CREATE_FUNC(Interface_Dialog);
	CREATEPush_FUNC(Interface_Dialog,"Interface_Dialog");
	bool init();
	/*
	@设置自己的名字
	*/
	void SetInterfaceName(const string & name = "Interface_Dialog");
	/*
	@初始化资源
	*/
	void InitSrcLayout();
	/*
	@初始化控件
	*/
	void InitWidget();
	/*
	@初始化按钮回调
	*/
	void InitButtonEvent();
	void OnIntoScene();
	void OnExitScene();
	void OnClose();
	static void PlayerTalk(int NpcId,int whoId = 0);
	void ButtonEventClose(Ref * pSender, ui::Widget::TouchEventType eventType);
	void ButtonClickClose(Ref * pSender);
public:
	void ShowTalk(int NpcId, int whoId);
	bool DoTalk();
	void DoFunctionInterface();
	bool DoAloneTalk(const CfgTalkData * cfgv);
	bool DoPeopleTalk(const CfgTalkData * cfg);
	bool DoSelectTalk(const CfgTalkData * cfg);
private:
	void RefreshHead(int leftHeadId = -2,int rightHeadId = -2);
	Layout * m_LayoutBackground;
	ImageView * m_ImageLeft;
	ImageView * m_ImageRight;
	ImageView * m_ImageDialog;
	RichTextEntity * m_RTContent;
	int m_TalkId;
	int m_TouchNpcId;
	int m_TouchWhoId;
	TalkType m_CurTalkType;
};


#endif //!__Interface_Dialog_H__