#include "Ui_Notice_Text.h"
#define  _Ui_Notice_Text_Src "Ui/Ui_Notice_Text.csb"
/*
@����,����
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
@�̶���������
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
@�����Լ�������
*/
void Ui_Notice_Text::SetInterfaceName()
{
	this->setName("Ui_Notice_Text");
	CCLOG("Ui_Notice_Text::SetInterfaceName()");
}

/*
��ʼ�Լ��Ķ���Ч��
*/
void Ui_Notice_Text::StartRun()
{
	/*
		@�����ƶ�
	*/
	auto move_by = MoveBy::create(1, Vec2(0,this->getContentSize().height));
	auto seq = Sequence::create(move_by, CallFunc::create([&](){
		if (nullptr != m_EndFunction && m_EndFunctionUser != nullptr)
		{
			//�������������Լ��Ķ�������
			(m_EndFunctionUser->*m_EndFunction)();
		}
	}), NULL);

	this->runAction(seq);
}

/*
	@�����ص�����
*/
void Ui_Notice_Text::EndFunctionSet(SEL_CallFunc_Void callback, Ref * register_user)
{
	m_EndFunction = callback;
	m_EndFunctionUser = register_user;
}
