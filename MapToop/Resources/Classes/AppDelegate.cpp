#include "AppDelegate.h"
#include "UpdateLayer.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}



bool AppDelegate::applicationDidFinishLaunching() {
	
	/*
	// ������Դ������Ŀ¼
	std::string m_downloadDir = CCFileUtils::sharedFileUtils()->getWritablePath();	
	m_downloadDir += "download";
	// �ȸ���
	AssetsManager* s_assetsManager = new AssetsManager("https://coding.net/u/linchaolong/p/Cocos2d-x_HotUpdate/git/raw/master/test.zip",  //������Դ����url
		"https://coding.net/u/linchaolong/p/Cocos2d-x_HotUpdate/git/raw/master/version", // ��ȡ����˰汾�ŵ�url
		m_downloadDir.c_str()); // ��Դ����·��
	s_assetsManager->setDelegate(this);
	s_assetsManager->setConnectionTimeout(3);
	CCLOG("save path : %s",s_assetsManager->getStoragePath());
	*/

    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

	pEGLView->setDesignResolutionSize(480, 320, kResolutionShowAll);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	//CCScene *pScene = HotUpdaterTest::scene();
	CCScene *pScene = UpdateLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
