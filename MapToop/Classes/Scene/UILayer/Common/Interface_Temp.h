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
	@设置自己的名字
	*/
	void SetInterfaceName(const string & name = "Interface_Temp");
	/*
	@初始化资源
	*/
	void InitSrcLayout();
	/*
	@初始化控件
	*/
	void InitWidget();
	/*
	@初始化按钮回调
	*/
	void InitButtonEvent();

	void OnIntoScene();
	void OnExitScene();
	void OnClose();
};


#endif //!__Interface_Temp_H__