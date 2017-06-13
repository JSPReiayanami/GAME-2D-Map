#ifndef __Interface_Entity_H__
#define __Interface_Entity_H__
#include "Config/Head.h"

#define CREATEPush_FUNC(__TYPE__,__Name__) \
	static __TYPE__* CreateToScene(string name = __Name__) \
{ \
	Interface_Entity * inter = Interface_Entity::PushIntoSceneWithName(name);\
	if (inter != nullptr)\
	{\
		return (__TYPE__ *)inter; \
	}\
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
	if (pRet && pRet->init()) \
	{ \
	pRet->autorelease(); \
	pRet->SetInterfaceName(__Name__);\
	pRet->PushIntoScene();\
	return pRet; \
	} \
	else \
	{ \
	delete pRet; \
	pRet = nullptr; \
	return nullptr; \
	} \
}


class Interface_Entity : public Layout
{
public:
	Interface_Entity();
	~Interface_Entity();
	CREATE_FUNC(Interface_Entity);
	CREATEPush_FUNC(Interface_Entity,"Interface_Entity");
	static Interface_Entity * PushIntoSceneWithName(const string & name);
	/*
		@固定创建方法
	*/
	virtual bool init();
	
	/*
		@根节点
	*/
	Layout * m_Root;//根节点
	string   m_LayerSrc;//界面资源
	bool m_InScene;//是否在界面中显示
	/*
		@设置自己的名字
	*/
	virtual void SetInterfaceName(const string & name = "Interface_Entity");
	/*
		@初始化资源
	*/
	virtual void InitSrcLayout();
	/*
		@初始化控件
	*/
	virtual void InitWidget();
	/*
		@初始化按钮回调
	*/
	virtual void InitButtonEvent();

	virtual void OnIntoScene();
	virtual void OnExitScene();
	virtual void OnClose();
	virtual void PushIntoScene();
	virtual void PopToScene(bool isRemove = true);
	
protected:

private:
	

};
#endif // !__MAINSCENE__