#include "Interface_MapCreate.h"
#include "Tool/MapEditor.h"
Interface_MapCreate::Interface_MapCreate()
{
	this->m_Root = nullptr;
	this->m_LayerSrc = "";
}
Interface_MapCreate::~Interface_MapCreate()
{

}
bool Interface_MapCreate::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}
	m_MapEditor = MapEditor::create();
	this->addChild(m_MapEditor);
	return true;
}
/*
@设置自己的名字
*/
void Interface_MapCreate::SetInterfaceName(const string & name)
{
	this->setName(name);
}
/*
@初始化资源
*/
void Interface_MapCreate::InitSrcLayout()
{

}
/*
@初始化控件
*/
void Interface_MapCreate::InitWidget()
{

}
/*
@初始化按钮回调
*/
void Interface_MapCreate::InitButtonEvent()
{

}

void Interface_MapCreate::OnIntoScene()
{
	Interface_Entity::OnIntoScene();
	//..
}
void Interface_MapCreate::OnExitScene()
{
	//..
	Interface_Entity::OnExitScene();
}
void Interface_MapCreate::OnClose()
{
	//..
	Interface_Entity::OnClose();
}