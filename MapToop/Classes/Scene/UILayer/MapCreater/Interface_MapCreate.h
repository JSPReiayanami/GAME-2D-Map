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
	@设置自己的名字
	*/
	void SetInterfaceName(const string & name = "Interface_MapCreate");
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

private:
	MapEditor * m_MapEditor;
};


#endif //!__Interface_MapCreate_H__