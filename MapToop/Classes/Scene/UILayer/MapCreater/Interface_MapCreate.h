#ifndef __Interface_MapCreate_H__
#define __Interface_MapCreate_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class MapEditor;
class Interface_MapCreate : public Interface_Entity
{
public:
	Interface_MapCreate();
	~Interface_MapCreate();
	CREATE_FUNC(Interface_MapCreate);
	CREATEPush_FUNC(Interface_MapCreate,"Interface_MapCreate");
	bool init();
	/*
	@�����Լ�������
	*/
	void SetInterfaceName(const string & name = "Interface_MapCreate");
	/*
	@��ʼ����Դ
	*/
	void InitSrcLayout();
	/*
	@��ʼ���ؼ�
	*/
	void InitWidget();
	/*
	@��ʼ����ť�ص�
	*/
	void InitButtonEvent();

	void OnIntoScene();
	void OnExitScene();
	void OnClose();

private:
	MapEditor * m_MapEditor;
};


#endif //!__Interface_MapCreate_H__