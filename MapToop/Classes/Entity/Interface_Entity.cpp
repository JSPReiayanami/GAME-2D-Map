#include "Interface_Entity.h"
#include "Scene/GameScene.h"
Interface_Entity::Interface_Entity()
{
	m_Root = nullptr;
	m_LayerSrc = "";
}
Interface_Entity::~Interface_Entity()
{

}

Interface_Entity * Interface_Entity::PushIntoSceneWithName(const string & name)
{
	int result = GameScene::GetInstance()->PushInterface(name);
	if (result == 0)
	{
		return GameScene::GetInstance()->GetUiLayout(name);
	}
	return nullptr;
}
/*
	@固定创建方法
*/
bool Interface_Entity::init(){
	if (!Layout::init())
	{
		return false;
	}
	//SetInterfaceName();
	InitSrcLayout();
	InitWidget();
	InitButtonEvent();
	return true;
}

/*
@设置自己的名字
*/
void Interface_Entity::SetInterfaceName(const string & name)
{
	this->setName(name);
	CCLOG("Interface_Entity::SetInterfaceName()");
}
/*
	@初始化资源
*/
void Interface_Entity::InitSrcLayout()
{

}
/*
	@初始化控件
*/
void Interface_Entity::InitWidget()
{

}
/*
	@初始化按钮回调
*/
void Interface_Entity::InitButtonEvent()
{

}

void Interface_Entity::OnIntoScene()
{
	this->setVisible(true);
	m_InScene = true;
}
void Interface_Entity::OnExitScene()
{
	this->setVisible(false);
	m_InScene = false;
}
void Interface_Entity::OnClose()
{

}

void Interface_Entity::PushIntoScene()
{
	GameScene::GetInstance()->PushInterface(this);
}

void Interface_Entity::PopToScene(bool isRemove)
{
	GameScene::GetInstance()->PopInterface(this, isRemove);
}