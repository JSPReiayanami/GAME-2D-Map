#ifndef __Ui_PassGame_H__
#define __Ui_PassGame_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Ui_PassGame : public Interface_Entity
{
public:
	Ui_PassGame();
	~Ui_PassGame();
	CREATE_FUNC(Ui_PassGame);
	/*
	@�̶���������
	*/
	virtual bool init();

	/*
	@���ڵ�
	*/
	Layout * m_Root;//���ڵ�
	string   m_LayerSrc;//������Դ
	CC_SYNTHESIZE(int, m_Id, MyId);//��Ʒ��id
	ListView * m_ListText;
	/*
	@�����Լ�������
	*/
	void SetInterfaceName();
	/*
	@��ʼ����Դ
	*/
	void InitSrcLayout();
	/*
	@��ʼ���ؼ�
	*/
	void InitWidget();
	/*
	@��ʼ����ť�Ļص��¼�
	*/
	void InitButtonEvent();
	/*
	@��ť�Ļص��¼�
	*/
	void TouchCloseEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
};
#endif // !__Ui_BagItem_H__