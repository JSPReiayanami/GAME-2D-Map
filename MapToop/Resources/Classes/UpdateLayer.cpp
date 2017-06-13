#include "UpdateLayer.h"
#include "HelloWorldScene.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

bool UpdateLayer::init(){
	if (CCLayer::init())
	{
		
		// ������Դ������Ŀ¼
		m_downloadDir = CCFileUtils::sharedFileUtils()->getWritablePath();	
		m_downloadDir += "download";

		// ���ô���
		getAssetsManager()->setDelegate(this);

		// �����Դ������·��������·���������������µ���Դ	
		std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
		searchPaths.insert(searchPaths.begin(), m_downloadDir);
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);	
		
		// ��ʾ
		m_label = CCLabelTTF::create("", "Arial", 18);
		m_label->setAnchorPoint(ccp(1,0.5));
		m_label->setPosition(ccp(465,20));
		addChild(m_label);

		// �˵�
		CCMenu* menu = CCMenu::create();
		menu->setPosition(CCPointZero);
		addChild(menu);

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

		// ����
		CCMenuItemFont* itemReset = CCMenuItemFont::create("reset",this,menu_selector(UpdateLayer::reset));
		itemReset->setPosition(ccp(visibleSize.width/2, 50));
		menu->addChild(itemReset);
		// ��ȡ��ǰ�汾��
		CCMenuItemFont* itemGetClientVersion = CCMenuItemFont::create("getClientVersion",this,menu_selector(UpdateLayer::getClientVersion));
		itemGetClientVersion->setPosition(ccp(visibleSize.width/2, 100));
		menu->addChild(itemGetClientVersion);
		// ��ȡ���������°汾
		CCMenuItemFont* itemGetServerVersion = CCMenuItemFont::create("checkUpdate",this,menu_selector(UpdateLayer::checkUpdate));
		itemGetServerVersion->setPosition(ccp(visibleSize.width/2, 150));
		menu->addChild(itemGetServerVersion);
		// ���°汾
		CCMenuItemFont* itemUpdateVersion = CCMenuItemFont::create("updateVersion",this,menu_selector(UpdateLayer::update));
		itemUpdateVersion->setPosition(ccp(visibleSize.width/2, 200));
		menu->addChild(itemUpdateVersion);
		// ���볡��
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
		s_assetsManager = new AssetsManager("https://coding.net/u/linchaolong/p/Cocos2d-x_HotUpdate/git/raw/master/test.zip",  //������Դ����url
			"https://coding.net/u/linchaolong/p/Cocos2d-x_HotUpdate/git/raw/master/version", // ��ȡ����˰汾�ŵ�url
			m_downloadDir.c_str()); // ��Դ����·��
		s_assetsManager->setDelegate(this);
		s_assetsManager->setConnectionTimeout(3);
	}
	CCLOG("save path : %s",s_assetsManager->getStoragePath());
	return s_assetsManager;
}

void UpdateLayer::initDownloadDir(){

	// �������Ŀ¼�����ڣ��򴴽�����Ŀ¼
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
	// ��ʼ������Ŀ¼
	initDownloadDir();
	// ���ظ��°�
	getAssetsManager()->update();
}

void UpdateLayer::reset(CCObject* pSender){
	if ("" != m_downloadDir)
	{
		// ɾ������Ŀ¼
		deleteDir(m_downloadDir);
	}
	// ɾ���汾��
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
