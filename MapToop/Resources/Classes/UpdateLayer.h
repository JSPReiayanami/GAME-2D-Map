#ifndef __HOTUPDATER_H__
#define __HOTUPDATER_H__

#include "cocos2d.h"
USING_NS_CC;
#include "cocos-ext.h"
USING_NS_CC_EXT;
#include "AssetsManager/AssetsManager.h"

// �ȸ���ʵ��ʾ��
class UpdateLayer:public CCLayer, public AssetsManagerDelegateProtocol
{
public:
	static CCScene* scene(){
		CCScene* scene = CCScene::create();
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
	// ��ʼ��
	bool init();

	// ���ػص�����
	virtual void onError(cocos2d::extension::AssetsManager::ErrorCode errorCode);
	virtual void onProgress(int percent);
	virtual void onSuccess();

	// �˵��ص�����
	void reset(CCObject* pSender);							// ���ð汾
	void getClientVersion(CCObject* pSender);	    // ��ȡ��ǰ�ͻ��˰汾��
	void checkUpdate(CCObject* pSender);		    // ����Ƿ��а汾����
	void update(CCObject* pSender);					    // ���°汾
	void enterScene(CCObject* pSender);				// ���볡�������δ������Ļ�м����ʾ̾�ŵ�ͼƬ�����º����ʾ��һ��ͼƬ
protected:
	// ��ʼ������Ŀ¼
	void initDownloadDir();
	// ɾ��Ŀ¼
	void deleteDir(std::string dir);
private:
	CCLabelTTF* m_label;
	std::string m_downloadDir;
	AssetsManager* getAssetsManager();	
};

#endif