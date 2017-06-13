#include "Ui_Notice_Text.h"
#define  _Ui_Notice_Text_Src "Ui/Ui_Notice_Text.csb"
/*
@构造,析构
*/
Ui_Notice_Text::Ui_Notice_Text():
m_EndFunction(nullptr),
m_EndFunctionUser(nullptr)
{
	m_LayerSrc = _Ui_Notice_Text_Src;
}
Ui_Notice_Text::~Ui_Notice_Text()
{

}
/*
@固定创建方法
*/
bool Ui_Notice_Text::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
	return true;
}
/*
@设置自己的名字
*/
void Ui_Notice_Text::SetInterfaceName()
{
	this->setName("Ui_Notice_Text");
	CCLOG("Ui_Notice_Text::SetInterfaceName()");
}

/*
开始自己的动画效果
*/
void Ui_Notice_Text::StartRun()
{
	/*
		@向下移动
	*/
	auto move_by = MoveBy::create(1, Vec2(0,this->getContentSize().height));
	auto seq = Sequence::create(move_by, CallFunc::create([&](){
		if (nullptr != m_EndFunction && m_EndFunctionUser != nullptr)
		{
			//结束告诉外面自己的动画结束
			(m_EndFunctionUser->*m_EndFunction)();
		}
	}), NULL);

	this->runAction(seq);
}

/*
	@结束回调设置
*/
void Ui_Notice_Text::EndFunctionSet(SEL_CallFunc_Void callback, Ref * register_user)
{
	m_EndFunction = callback;
	m_EndFunctionUser = register_user;
}
