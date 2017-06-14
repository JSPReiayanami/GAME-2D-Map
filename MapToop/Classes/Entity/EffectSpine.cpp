#include "EffectSpine.h"

EffectSpine::EffectSpine()
{
	m_Spine = nullptr;
	m_CurActionName = "";
}

EffectSpine::~EffectSpine()
{
	this->unscheduleUpdate();
}
EffectSpine * EffectSpine::create(const std::string& skeletonDataFile, const std::string& atlasFile, float scale/* = 1*/)
{
	auto ret = new (std::nothrow) EffectSpine;
	if (ret && ret->init(skeletonDataFile, atlasFile, scale)) {
		ret->scheduleUpdate();
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void EffectSpine::PlayWithActionName(std::string name)
{
	spTrackEntry * ani = m_Spine->setAnimation(0, name, true);
	if (m_CurActionName != "" && nullptr == ani)
	{
		m_Spine->setAnimation(0, m_CurActionName, true);
		CCLOG("[%s]Have no this action!", name.c_str());
	}
	else
	{
		m_CurActionName = name;
	}
}

bool EffectSpine::init(const std::string& skeletonDataFile, const std::string& atlasFile, float scale /*= 1*/)
{
	m_Spine = SkeletonAnimation::createWithFile(skeletonDataFile, atlasFile, scale);
	if (!Layout::init() || nullptr == m_Spine)
	{
		return false;
	}
	m_Spine->setStartListener(std::bind(&EffectSpine::OnStartListener, this,std::placeholders::_1));
	m_Spine->setEndListener(std::bind(&EffectSpine::OnEndListener, this, std::placeholders::_1));
	m_Spine->setCompleteListener(std::bind(&EffectSpine::OnCompleteListener, this, std::placeholders::_1, std::placeholders::_2));
	m_Spine->setEventListener(std::bind(&EffectSpine::OnEventListener, this, std::placeholders::_1, std::placeholders::_2));

	this->addChild(m_Spine);
	this->setContentSize(m_Spine->getContentSize());
	return true;
}

void EffectSpine::update(float delta) {

}

void EffectSpine::OnStartListener(int trackIndex)
{

}
void EffectSpine::OnEndListener(int trackIndex)
{

}
void EffectSpine::OnCompleteListener(int trackIndex, int loopCount)
{

}
void EffectSpine::OnEventListener(int trackIndex, spEvent* event)
{
	CCLOG("%d event: %s, %d, %f, %s", trackIndex, event->data->name, event->intValue, event->floatValue, event->stringValue);
	std::string curName = event->data->name;
	for (auto & funcData : m_CallFunc) {
		if (funcData.name == curName) {

		}
	}
}
