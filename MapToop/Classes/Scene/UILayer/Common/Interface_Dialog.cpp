#include "Interface_Dialog.h"
#include "Scene/GameScene.h"
#include "Scene/GameLogicDelegate.h"
#include "Entity/RichTextEntity.h"
#include "FileData/FileData_TalkData.h"
#include "FileData/FileData_Npc.h"
Interface_Dialog::Interface_Dialog()
{
	this->m_Root = nullptr;
	this->m_LayerSrc = "UILayer/Interface_Dialog.csb";
	m_TalkId = 0;
	m_CurTalkType = TalkType::TalkType_MAX;
}
Interface_Dialog::~Interface_Dialog()
{

}
bool Interface_Dialog::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}

	return true;
}
/*
@设置自己的名字
*/
void Interface_Dialog::SetInterfaceName(const string & name)
{
	this->setName(name);
}
/*
@初始化资源
*/
void Interface_Dialog::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(this->m_LayerSrc);
	if (m_Root != nullptr)
	{
		this->addChild(m_Root);
		this->setContentSize(m_Root->getContentSize());
	}
}
/*
@初始化控件
*/
void Interface_Dialog::InitWidget()
{
	__GetChildAndAssignmentWithOB(m_LayoutBackground , m_Root, "Panel_Background", Layout);
	__GetChildAndAssignmentWithOB(m_ImageLeft, m_LayoutBackground, "Image_Left", ImageView);
	__GetChildAndAssignmentWithOB(m_ImageRight, m_LayoutBackground, "Image_Right", ImageView);
	__GetChildAndAssignmentWithOB(m_ImageDialog, m_LayoutBackground, "Image_Dialog", ImageView);
	m_RTContent = RichTextEntity::CreateWithContent("...",m_ImageDialog->getContentSize() * 0.8);
	m_LayoutBackground->addChild(m_RTContent,99);
	Vec2 pos = m_ImageDialog->getPosition();
	Size size = m_RTContent->getContentSize() / 2;
	m_RTContent->setPosition(m_ImageDialog->getPosition() - m_RTContent->getContentSize()/2);
}
/*
@初始化按钮回调
*/
void Interface_Dialog::InitButtonEvent()
{
	__AddTouchEventWithNameAndPNode("Button_Close", m_LayoutBackground, Interface_Dialog::ButtonEventClose)
	m_LayoutBackground->addClickEventListener(CC_CALLBACK_1(Interface_Dialog::ButtonClickClose,this));
}

void Interface_Dialog::OnIntoScene()
{
	Interface_Entity::OnIntoScene();
}

void Interface_Dialog::OnExitScene()
{
	Interface_Entity::OnExitScene();
}

void Interface_Dialog::OnClose()
{
	Interface_Entity::OnClose();
}

void Interface_Dialog::PlayerTalk(int NpcId, int whoId)
{
	Interface_Dialog * inter = Interface_Dialog::CreateToScene();
	inter->ShowTalk(NpcId, whoId);
}

void Interface_Dialog::ButtonEventClose(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == TouchEventType::ENDED)
	{
		PopToScene(false);
	}
}

void Interface_Dialog::ButtonClickClose(Ref * pSender)
{
	if (DoTalk() == false)
	{
		PopToScene(false);
	}
}

//public:
void Interface_Dialog::ShowTalk(int NpcId, int whoId)
{
	const CfgNpc * cfgNpc = FileDataNpc::GetInstance()->GetCfg(NpcId);
	if (cfgNpc != nullptr)
	{
		m_TouchNpcId = NpcId;
		m_TouchWhoId = whoId;
		if (cfgNpc->TalksId.size() >= 1)
		{
			m_TalkId = *cfgNpc->TalksId.begin();
			DoTalk();
		}
	}
}
bool Interface_Dialog::DoTalk()
{
	const CfgTalkData * cfg = FileDataTalkData::GetInstance()->GetCfg(m_TalkId);
	m_TalkId = 0;//防止出现对话无限的情况
	bool result = false;
	if (cfg != nullptr)
	{
		m_CurTalkType = (TalkType)cfg->TalkType;
		switch (m_CurTalkType)
		{
		case TalkType_Alone:
			result = DoAloneTalk(cfg);
			break;
		case TalkType_People:
			result = DoPeopleTalk(cfg);
			break;
		case TalkType_Select:
			result = DoSelectTalk(cfg);
			break;
		case TalkType_MAX:
			break;
		default:
			break;
		}
	}
	return result;
}

void Interface_Dialog::DoFunctionInterface()
{

}
bool Interface_Dialog::DoAloneTalk(const CfgTalkData * cfg)
{
	m_RTContent->SetContent(cfg->Content);
	m_RTContent->RefrshView();
	m_TalkId = cfg->NextId;
	RefreshHead(cfg->UseHeadNpcId);
	return false;
}
bool Interface_Dialog::DoPeopleTalk(const CfgTalkData * cfg)
{
	m_RTContent->SetContent(cfg->Content);
	m_RTContent->RefrshView();
	m_TalkId = cfg->NextId;
	RefreshHead(cfg->UseHeadNpcId);
	m_TalkId = cfg->Id;
	return false;
}
bool Interface_Dialog::DoSelectTalk(const CfgTalkData * cfg)
{
	m_TalkId = cfg->Id;
	return false;
}

void Interface_Dialog::RefreshHead(int leftHeadId, int rightHeadId)
{
	if (m_ImageRight)
	{
		m_ImageRight->setVisible(false);
	}
	if (m_ImageLeft)
	{
		m_ImageLeft->setVisible(false);
	}
	if (leftHeadId == -1)
	{
		m_ImageLeft->setVisible(true);
		GameScene::GetInstance()->GetGameLogic()->GetPlayer();
	}else if (leftHeadId == 0)
	{
		m_ImageRight->setVisible(true);
	}
	else if (leftHeadId > 0)
	{
		m_ImageRight->setVisible(true);
	}
}