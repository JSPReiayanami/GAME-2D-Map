#include "EffectAnimation.h"
#include "Effect.h"

EffectArmature::EffectArmature()
{
	m_Animates.clear();
	m_CurAction = nullptr;
	m_CurPlayName = "";
	m_CallFunc.clear();
}

EffectArmature::~EffectArmature()
{
	this->unscheduleUpdate();
}
EffectArmature * EffectArmature::create(const std::string& filename)
{
	auto ret = new (std::nothrow) EffectArmature;
	if (ret && ret->initWithFile(filename)) {
		ret->scheduleUpdate();
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}
bool EffectArmature::AddAnimate(Animate * animate, const string & name)
{
	//龟儿子没用命名
	if (name == ""){
		return false;
	}
	if (m_Animates.find(name) == m_Animates.end()){
		m_Animates.insert(make_pair(name, animate));
		animate->retain();
		return true;
	}
	else
	{
		return false;
	}
}

bool EffectArmature::RemoveAnimate(const string & name)
{
	if (name == m_CurPlayName && m_CurAction != nullptr){
		this->stopAction(m_CurAction);
	}
	if (m_Animates.find(name) != m_Animates.end()){
		m_Animates.at(name)->release();
		m_Animates.erase(m_Animates.find(name));
	}
	return true;
}

void EffectArmature::PlayAnimation(const string & name)
{
	if (m_Animates.find(name) == m_Animates.end()) return;
	m_CurPlayName = name;
	if (m_CurAction != nullptr)
		this->stopAction(m_CurAction);

	m_CurAction = this->runAction(m_Animates[name]);
}

void EffectArmature::StopPlay()
{
	this->getActionManager()->pauseTarget(this);
}

void EffectArmature::ContinuePlay()
{
	this->getActionManager()->resumeTarget(this);
}

void EffectArmature::AddAnimateCallFunc(EffectArmatureFuncData & callFunc)
{
	m_CallFunc.push_back(callFunc);
}

void EffectArmature::update(float delta)
{
	vector<EffectArmatureFuncData>::iterator ite = m_CallFunc.begin();
	while (ite != m_CallFunc.end())
	{
		EffectArmatureFuncData func = (*ite);
		if (m_CurAction != nullptr && m_CurPlayName != "" && 
			m_Animates.find(func.name) != m_Animates.end() && 
			func.activate == false &&
			m_Animates[func.name]->getCurrentFrameIndex() == func.index 
		){
			(*ite).activate = true;
			func.ActivateFunc(this);
		}
		else if (m_CurAction != nullptr && m_CurPlayName != "" &&
			m_Animates.find(func.name) != m_Animates.end() &&
			func.activate == true &&
			m_Animates[func.name]->getCurrentFrameIndex() != func.index
		)
		{
			(*ite).activate = false;
		}
		ite++;
	}
}



