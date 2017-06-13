#include "Interface_Main.h"
#include "Scene/GameScene.h"
#include "Manager/StoryDirector.h"
#include "Tool/StringHelper.h"
Interface_Main::Interface_Main()
{
	this->m_Root = nullptr;
	this->m_LayerSrc = "UILayer/Interface_Main.csb";
}
Interface_Main::~Interface_Main()
{

}
bool Interface_Main::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}
	this->scheduleUpdate();
	return true;
}
/*
@设置自己的名字
*/
void Interface_Main::SetInterfaceName(const string & name)
{
	this->setName(name);
}
/*
@初始化资源
*/
void Interface_Main::InitSrcLayout()
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
void Interface_Main::InitWidget()
{
	__GetChildAndAssignmentWithOB(m_LayoutView, m_Root, "Panel_View", Layout);
	__GetChildAndAssignmentWithOB(m_LayoutButton, m_Root, "Panel_Button", Layout);
	__GetChildAndAssignmentWithOB(m_TextTime, m_LayoutView, "Text_Time", Text);
}
/*
@初始化按钮回调
*/
void Interface_Main::InitButtonEvent()
{

}

void Interface_Main::OnIntoScene()
{
	Interface_Entity::OnIntoScene();
	//..
	GameScene::GetInstance()->SetInterfaceToTop(this->getName());
}
void Interface_Main::OnExitScene()
{
	//..
	Interface_Entity::OnExitScene();
}
void Interface_Main::OnClose()
{
	//..
	Interface_Entity::OnClose();
}

void Interface_Main::update(float delta)
{
	if (m_TextTime)
	{
		long curGameTime = StoryDirector::GetInstance()->GetGameTime();
		string timeStr = StringHelper::longToStringTime(curGameTime, 3);
		m_TextTime->setString(timeStr);
	}
}