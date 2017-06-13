#include "UpdateLayer.h"
#include "HelloWorldScene.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

bool UpdateLayer::init(){
	if (CCLayer::init())
	{
		
		// 设置资源包下载目录
		m_downloadDir = CCFileUtils::sharedFileUtils()->getWritablePath();	
		m_downloadDir += "download";

		// 设置代理
		getAssetsManager()->setDelegate(this);

		// 添加资源包下载路径到搜索路径，优先搜索更新的资源	
		std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
		searchPaths.insert(searchPaths.begin(), m_downloadDir);
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);	
		
		// 提示
		m_label = CCLabelTTF::create("", "Arial", 18);
		m_label->setAnchorPoint(ccp(1,0.5));
		m_label->setPosition(ccp(465,20));
		addChild(m_label);

		// 菜单
		CCMenu* menu = CCMenu::create();
		menu->setPosition(CCPointZero);
		addChild(menu);

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

		// 重置
		CCMenuItemFont* itemReset = CCMenuItemFont::create("reset",this,menu_selector(UpdateLayer::reset));
		itemReset->setPosition(ccp(visibleSize.width/2, 50));
		menu->addChild(itemReset);
		// 获取当前版本号
		CCMenuItemFont* itemGetClientVersion = CCMenuItemFont::create("getClientVersion",this,menu_selector(UpdateLayer::getClientVersion));
		itemGetClientVersion->setPosition(ccp(visibleSize.width/2, 100));
		menu->addChild(itemGetClientVersion);
		// 获取服务器最新版本
		CCMenuItemFont* itemGetServerVersion = CCMenuItemFont::create("checkUpdate",this,menu_selector(UpdateLayer::checkUpdate));
		itemGetServerVersion->setPosition(ccp(visibleSize.width/2, 150));
		menu->addChild(itemGetServerVersion);
		// 更新版本
		CCMenuItemFont* itemUpdateVersion = CCMenuItemFont::create("updateVersion",this,menu_selector(UpdateLayer::update));
		itemUpdateVersion->setPosition(ccp(visibleSize.width/2, 200));
		menu->addChild(itemUpdateVersion);
		// 进入场景
		CCMenuItemFont* itemEnterScene = CCMenuItemFont::create("enterScene",this,menu_selector(UpdateLayer::enterScene));
		itemEnterScene->setPosition(ccp(visibleSize.width/2, 250));
		menu->addChild(itemEnterScene);
		return true;
	}
	return false;
}


AssetsManager* UpdateLayer::getAssetsManager(){
	static AssetsManager* s_assetsManager = NULL;

	if (s_assetsManager ==NULL)
	{		
		s_assetsManager = new AssetsManager("https://coding.net/u/linchaolong/p/Cocos2d-x_HotUpdate/git/raw/master/test.zip",  //下载资源包的url
			"https://coding.net/u/linchaolong/p/Cocos2d-x_HotUpdate/git/raw/master/version", // 获取服务端版本号的url
			m_downloadDir.c_str()); // 资源保存路径
		s_assetsManager->setDelegate(this);
		s_assetsManager->setConnectionTimeout(3);
	}
	CCLOG("save path : %s",s_assetsManager->getStoragePath());
	return s_assetsManager;
}

void UpdateLayer::initDownloadDir(){

	// 如果下载目录不存在，则创建下载目录
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;

	pDir = opendir (m_downloadDir.c_str());
	if (! pDir)
	{
		mkdir(m_downloadDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
	if ((GetFileAttributesA(m_downloadDir.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(m_downloadDir.c_str(), 0);
	}
#endif
}

void UpdateLayer::deleteDir(std::string dir){
	// Remove downloaded files
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	std::string command = "rm -r ";
	// Path may include space.
	command += "\"" + dir + "\"";
	system(command.c_str());
#else
	std::string command = "rd /s /q ";
	// Path may include space.
	command += "\"" + dir + "\"";
	system(command.c_str());
#endif
}

void UpdateLayer::onError(cocos2d::extension::AssetsManager::ErrorCode errorCode){
	switch (errorCode)
	{
	case cocos2d::extension::AssetsManager::kCreateFile:
		CCLOG("error : create file failure");
		m_label->setString("error : create file failure");
		break;
	case cocos2d::extension::AssetsManager::kNetwork:
		CCLOG("error : no network");
		m_label->setString("error : no network");
		break;
	case cocos2d::extension::AssetsManager::kNoNewVersion:
		CCLOG("error : no new version");
		m_label->setString("error : no new version");
		break;
	case cocos2d::extension::AssetsManager::kUncompress:
		CCLOG("error : uncompress file error");
		m_label->setString("error : uncompress file error");
		break;
	default:
		break;
	}
}

void UpdateLayer::onProgress(int percent){
	char progress[80];
	memset( progress, '\0', sizeof(progress) );
	snprintf(progress, sizeof(progress), "hotupdate downloading %d%%", percent);	
	CCLOG("percent=%d %s",percent,progress);
	m_label->setString(progress);
}

void UpdateLayer::onSuccess(){
	CCLOG("download success.");	
	m_label->setString("download success.");
}

void UpdateLayer::update(CCObject* pSender){
	// 初始化下载目录
	initDownloadDir();
	// 下载更新包
	getAssetsManager()->update();
}

void UpdateLayer::reset(CCObject* pSender){
	if ("" != m_downloadDir)
	{
		// 删除下载目录
		deleteDir(m_downloadDir);
	}
	// 删除版本号
	getAssetsManager()->deleteVersion();
}

void  UpdateLayer::getClientVersion(CCObject* pSender){
	CCString* msg = CCString::createWithFormat("current client version : %s", getAssetsManager()->getVersion().c_str());
	CCLOG("%s",msg->getCString());
	m_label->setString(msg->getCString());
}

void UpdateLayer::checkUpdate(CCObject* pSender){	
	if (getAssetsManager()->checkUpdate())
	{
		CCLOG("has new version");
		m_label->setString("has new version");
	}else{
		CCLOG("has not new version");
		m_label->setString("has not new version");
	}
}

void UpdateLayer::enterScene(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}
