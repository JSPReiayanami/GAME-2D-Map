#include "NoticeManager.h"


#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/SceneManager.h"
#include "Config/UiConfig.h"
#include "Manager/SDataManager.h"
#else
#include "SceneManager.h"
#include "UiConfig.h"
#include "DataManager.h"
#endif//判断路径的引用
USING_NS_CC;
NoticeManager::NoticeManager()
{
}


NoticeManager::~NoticeManager()
{
}

//===== get the manger ====
NoticeManager * NoticeManager::m_NoticeManager = nullptr;
NoticeManager * NoticeManager::getInstance(){
	if (m_NoticeManager == nullptr)
	{
		m_NoticeManager = new NoticeManager();
	}
	return m_NoticeManager;
}

//---
Scene * NoticeManager::createScene(){
	auto scene = Scene::create();
	auto manger = NoticeManager::create();
	scene->addChild(manger);
	return scene;
}
bool NoticeManager::init(){
	if (!Node::init())
	{
		return false;
	}
	return true;
}

/*
 @弹出一个提示框,一段时间后自动消失
 */
void NoticeManager::PopStringNoticeAndWillRemove(const string & content,Node * add_node,const float time /*= 1*/)
{
    auto ui_s_n = Ui_StringNotice::CreateWithContentWithTime(content);
    add_node->addChild(ui_s_n,99);
    ui_s_n->setPosition(__VisibleSize/2);
    ui_s_n->setPosition(Vec2(__VisibleSize_Width/2, -1000));
    auto move_to_1 = MoveTo::create(1, add_node->getContentSize()/2);
    auto move_to_2 = MoveTo::create(1, Vec2(ui_s_n->getPositionX(),-1000));
    auto seq = Sequence::create(move_to_1,DelayTime::create(3),move_to_2,CallFunc::create([=](){
        ui_s_n->removeFromParent();
    }),NULL);
    ui_s_n->runAction(seq);
}
/*
 @弹出一个提示框,一段时间后自动消失
 */
void NoticeManager::PopStringNoticeAndWillRemoveWithId(const int & id,Node * add_node)
{
    auto notice_data = DataManager::getInstance()->GetNoticeStringDataWithId(id);
    if(nullptr != notice_data)
    {
        auto ui_s_n = Ui_StringNotice::CreateWithContentWithTime(notice_data->getMyValue());
        add_node->addChild(ui_s_n,99);
        ui_s_n->setPosition(__VisibleSize/2);
        ui_s_n->setPosition(Vec2(__VisibleSize_Width/2, -1000));
        auto move_to_1 = MoveTo::create(1, add_node->getContentSize()/2);
        auto move_to_2 = MoveTo::create(1, Vec2(ui_s_n->getPositionX(),-1000));
        auto seq = Sequence::create(move_to_1,DelayTime::create(3),move_to_2,CallFunc::create([=](){
            ui_s_n->removeFromParent();
        }),NULL);
        ui_s_n->runAction(seq);
    }
    
}

/*
 @弹出过关粒子效果
 */
void NoticeManager::PopPassGameParticle()
{
    auto  par = ParticleSystemQuad::create("Particle/Pass_1.plist");
    auto o_secen = Director::getInstance()->getRunningScene();
    o_secen->addChild(par);
    par->setPosition(__VisibleSize/2);
    auto seq = Sequence::create(DelayTime::create(2),CallFunc::create([=](){
        par->removeFromParent();
    }), NULL);
    par->runAction(seq);
}
