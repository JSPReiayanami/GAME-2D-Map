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
@�����Լ�������
*/
void Interface_MapCreate::SetInterfaceName(const string & name)
{
	this->setName(name);
}
/*
@��ʼ����Դ
*/
void Interface_MapCreate::InitSrcLayout()
{

}
/*
@��ʼ���ؼ�
*/
void Interface_MapCreate::InitWidget()
{

}
/*
@��ʼ����ť�ص�
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