#include "SoundManager.h"
#include "Head.h"
//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Config/SceneManagerCofing.h"
#include "DataManager.h"
#else
#include "DataManager.h"
#include "SceneManagerCofing.h"
#endif//判断路径的引用
USING_NS_CC;
SoundManager::SoundManager():
m_SoundIsOpen(true)
{
	//m_SimleAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
}


SoundManager::~SoundManager()
{
}

//===== get the manger ====
SoundManager * SoundManager::s_SoundManager = nullptr;
SoundManager * SoundManager::getInstance(){
	if (s_SoundManager == nullptr)
	{
		s_SoundManager = new SoundManager();
	}
	return s_SoundManager;
}
/*
	@预加载音效
*/
void SoundManager::PreloadAllEffect()
{
	/*for (auto & music_info : DataManager::getInstance()->m_MusicData)
	{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(music_info.second->getMyMusicPath().c_str());
	}*/
}

void SoundManager::PlayBackGroundMusic(const int & _id)
{
	if (m_SoundIsOpen)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("", true);
	}
}
/*
@播放音效
*/
void SoundManager::PlayEffectMusic(const int & _id)
{
	if (m_SoundIsOpen)
	{
		
		if (false)
		{
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(music_data->getMyMusicPath().c_str());
		}
	}
}

/*
@关闭声音
*/
void SoundManager::CloseSound()
{
	m_BackSoundVolume = CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
	//CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	m_SoundIsOpen = false;
}
/*
@打开声音
*/
void SoundManager::OpenSound()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	m_SoundIsOpen = true;
}