#ifndef __SOUNDMANAGER_H_
#define __SOUNDMANAGER_H_
#include "cocos2d.h"
#include <iostream>
class SoundManager 
{
public:
	SoundManager();
	~SoundManager();
	/*
		@获取单例音效管理者
	*/
	static SoundManager * getInstance();
	/*
		@单例的指针
	*/
	static SoundManager * s_SoundManager;
	//CocosDenshion::SimpleAudioEngine * m_SimleAudioEngine;
	float m_BackSoundVolume;
	bool m_SoundIsOpen;
	/*
		@预加载音效
	*/
	void PreloadAllEffect();
	/*
		@播放背景音乐
	*/
	void PlayBackGroundMusic(const int & _id);
	/*
		@播放音效
	*/
	void PlayEffectMusic(const int & _id);
	/*
		@关闭声音
	*/
	void CloseSound();
	/*
		@打开声音
	*/
	void OpenSound();
};
#endif

