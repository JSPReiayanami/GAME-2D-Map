#ifndef __NoticeManager_H_
#define __NoticeManager_H_
#include "Head.h"

#if (__HeadFileIncludeNum == __HeadFileHavePath)

#else

#endif//判断路径的引用
class NoticeManager :
	public cocos2d::Node
{
public:
	NoticeManager();
	~NoticeManager();
	/*
	@固定创建方法
	*/
	static cocos2d::Scene * createScene();
	virtual bool init();
	CREATE_FUNC(NoticeManager);
	/*
	@获取单例场景管理者
	*/
	static NoticeManager * getInstance();
    /*
        @弹出一个提示框,一段时间后自动消失
     */
    void PopStringNoticeAndWillRemove(const string & content,Node * add_node,const float time = 1);
    /*
        @弹出一个提示框,一段时间后自动消失
     */
    void PopStringNoticeAndWillRemoveWithId(const int & id,Node * add_node);
    /*
        @弹出过关粒子效果
     */
    void PopPassGameParticle();
private:
	/*
	@单例的指针
	*/
	static NoticeManager * m_NoticeManager;
	
};
#endif

