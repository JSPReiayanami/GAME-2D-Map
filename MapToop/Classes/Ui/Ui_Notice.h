#ifndef __Ui_Notice_H__
#define __Ui_Notice_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Ui_Notice : public Interface_Entity
{
public:
	Ui_Notice();
	~Ui_Notice();
	static Ui_Notice * CreateWithId(const int & _id);
	CREATE_FUNC(Ui_Notice);
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
		@ˢ����ʾ
	*/
	void RefreshView();
	/*
	@��ť�Ļص��¼�
	*/
	void TouchCloseEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@
	*/
	virtual void onEnter();
};
#endif // !__Ui_BagItem_H__