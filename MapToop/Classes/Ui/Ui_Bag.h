#ifndef __Ui_Bag_H__
#define __Ui_Bag_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Ui_BagItem;
class Ui_Bag : public Interface_Entity
{
public:
	Ui_Bag();
	~Ui_Bag();
	CREATE_FUNC(Ui_Bag);
	/*
	@�̶���������
	*/
	virtual bool init();

	/*
	@���ڵ�
	*/
	Layout * m_Root;//���ڵ�
	string   m_LayerSrc;//������Դ
	map<int, ImageView *> m_BagSpace;//��Ʒ�ĸ���
	map<int, int> m_BagData;//��Ʒ������
	map<int, Ui_BagItem*> m_BagItem;//��Ʒ��ʵ��
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
		@��ʼ����������
	*/
	void InitBagSpace();
	/*
		@��ӵ���
	*/
	void AddPropToBag(PropData * p_data);
	void AddItemToBagWithId(const int & _id);
	/*
		@ɾ������
	*/
	void DeletePropFromBag(int _id);
	/*
		@��ձ���
	*/
	void ClearBag();
	/*
		@������и����ĵ���
	*/
	void ClearAllPropHightLight();
	/*
		@�Ƿ񱳰��и�Id����Ʒ
	*/
	bool IsHavePropWithId(const int & _id);
};
#endif // !__Ui_Notice_Text_H__