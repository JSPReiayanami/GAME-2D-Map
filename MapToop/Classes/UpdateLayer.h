#ifndef __HOTUPDATER_H__
#define __HOTUPDATER_H__
#include "Config/Head.h"
#include "AssetsManager.h"
using namespace extension;
// 热更新实现示例
class UpdateLayer:public Layer, public AssetsManagerDelegateProtocol
{
public:
	static Scene* scene(){
		Scene* scene = Scene::create();
		scene->addChild(UpdateLayer::create());
		return scene;
	};
	static UpdateLayer* create(){
		UpdateLayer* pLayer = new UpdateLayer;
		if (pLayer && pLayer->init())
		{
			pLayer->autorelease();
			return pLayer;
		}
		delete pLayer;
		return NULL;
	};
	// 初始化
	bool init();
	// 下载回调函数
	virtual void onError(AssetsManager::ErrorCode errorCode);
	virtual void onProgress(int percent);
	virtual void onSuccess();

	// 菜单回调函数
	void reset(CCObject* pSender);							// 重置版本
	void getClientVersion(CCObject* pSender);	    // 获取当前客户端版本号
	void checkUpdate(CCObject* pSender);		    // 检查是否有版本更新
	void update(CCObject* pSender);					    // 更新版本
	void enterScene(CCObject* pSender);				// 进入场景，如果未更新屏幕中间会显示叹号的图片，更新后会显示另一张图片
protected:
	// 初始化下载目录
	void initDownloadDir();
	// 删除目录
	void deleteDir(std::string dir);
private:
	CCLabelTTF* m_label;
	std::string m_downloadDir;
	cocos2d::extension::AssetsManager* getAssetsManager();
};

#endif