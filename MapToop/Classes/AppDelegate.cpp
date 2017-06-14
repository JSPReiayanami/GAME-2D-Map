#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Config/Head.h"
USING_NS_CC;
#include "Test/PhysicsScene.h"
#include "Scene/GameScene.h"
#include "Manager/FileManager.h"
#include "Scene/UILayer/Common/Interface_Main.h"
#include "Scene/UILayer/LittleGame/Interface_LittleGame_1.h"
#include "Scene/UiLayer/MapCreater/Interface_MapCreate.h"
AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}
//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);

}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("MapTool", Rect(0, 0, __DesignResolutionWidth, __DesignResolutionHight));
        director->setOpenGLView(glview);
    }
    director->getOpenGLView()->setDesignResolutionSize(__DesignResolutionWidth, __DesignResolutionHight, ResolutionPolicy::FIXED_WIDTH);
	director->getOpenGLView()->setFrameSize(__DesignResolutionWidth, __DesignResolutionHight);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("res");
    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
	//auto scene = PhysicsScene::createScene();
    // run
 	FileManager::GetInstance()->LoadFileData();
 	//GameScene::RunGame();
	GameScene::RunPhysicsGame();
 //	
	//Interface_Main::CreateToScene();
	Interface_MapCreate::CreateToScene();
	//Interface_LittleGame_1::CreateToScene();
	//GameScene::GetInstance()->PushIntoMap(60001001);
	//director->runWithScene(HelloWorld::createScene());
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
