#include "Interface_Temp.h"
Interface_Temp::Interface_Temp()
{
	this->m_Root = nullptr;
	this->m_LayerSrc = "";
}
Interface_Temp::~Interface_Temp()
{

}
bool Interface_Temp::init()
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
void Interface_Temp::SetInterfaceName(const string & name)
{
	this->setName(name);
}
/*
@初始化资源
*/
void Interface_Temp::InitSrcLayout()
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
void Interface_Temp::InitWidget()
{

}
/*
@初始化按钮回调
*/
void Interface_Temp::InitButtonEvent()
{

}

void Interface_Temp::OnIntoScene()
{
	Interface_Entity::OnIntoScene();
	//..
}
void Interface_Temp::OnExitScene()
{
	//..
	Interface_Entity::OnExitScene();
}
void Interface_Temp::OnClose()
{
	//..
	Interface_Entity::OnClose();
}