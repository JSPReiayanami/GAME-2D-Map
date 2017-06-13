#ifndef __Interface_Temp_H__
#define __Interface_Temp_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"

class Interface_Temp : public Interface_Entity
{
public:
	Interface_Temp();
	~Interface_Temp();
	CREATE_FUNC(Interface_Temp);
	CREATEPush_FUNC(Interface_Temp,"Interface_Temp");
	bool init();
	/*
	@�����Լ�������
	*/
	void SetInterfaceName(const string & name = "Interface_Temp");
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
};


#endif //!__Interface_Temp_H__