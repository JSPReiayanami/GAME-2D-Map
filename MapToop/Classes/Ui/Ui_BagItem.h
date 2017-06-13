#ifndef __Ui_BagItem_H__
#define __Ui_BagItem_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Ui_BagItem : public Interface_Entity
{
public:
	Ui_BagItem();
	~Ui_BagItem();
	static Ui_BagItem * CreateWithId(const int & _id);
	CREATE_FUNC(Ui_BagItem);
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
	CC_SYNTHESIZE(int, m_Value, MyValue);//��Ʒ��ѡ��״̬
	CC_SYNTHESIZE(bool, m_IsChoice, MyIsChoice);//��Ʒ��ѡ��״̬
	Button * m_ButtonItem;
	Text   * m_TextName;
	ImageView * m_ImageChoice;
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
	void TouchEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@�����Ƿ�ѡ��
	*/
	void SetIsChoice(bool _is);
};
#endif // !__Ui_BagItem_H__