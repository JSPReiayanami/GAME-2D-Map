#ifndef __Ui_Notice_Text_H__
#define __Ui_Notice_Text_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Ui_Notice_Text : public Interface_Entity
{
public:
	Ui_Notice_Text();
	~Ui_Notice_Text();
	CREATE_FUNC(Ui_Notice_Text);
	/*
		@�̶���������
	*/
	virtual bool init();

	/*
		@���ڵ�
	*/
	Layout * m_Root;//���ڵ�
	string   m_LayerSrc;//������Դ
	/*
		@�����Լ�������
	*/
	void SetInterfaceName();
	/*
		��ʼ�Լ��Ķ���Ч��
	*/
	void StartRun();
	/*
		@�����ص�����
	*/
	void EndFunctionSet(SEL_CallFunc_Void callback, Ref * register_user);
	/*
		@����
	*/
	SEL_CallFunc_Void m_EndFunction;
	Ref * m_EndFunctionUser;
};
#endif // !__Ui_Notice_Text_H__