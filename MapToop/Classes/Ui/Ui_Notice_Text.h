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
		@固定创建方法
	*/
	virtual bool init();

	/*
		@根节点
	*/
	Layout * m_Root;//根节点
	string   m_LayerSrc;//界面资源
	/*
		@设置自己的名字
	*/
	void SetInterfaceName();
	/*
		开始自己的动画效果
	*/
	void StartRun();
	/*
		@结束回调设置
	*/
	void EndFunctionSet(SEL_CallFunc_Void callback, Ref * register_user);
	/*
		@属性
	*/
	SEL_CallFunc_Void m_EndFunction;
	Ref * m_EndFunctionUser;
};
#endif // !__Ui_Notice_Text_H__