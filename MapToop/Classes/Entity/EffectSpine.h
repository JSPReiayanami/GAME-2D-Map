#ifndef _EffectSpine_H_
#define _EffectSpine_H_
#include "Config/Head.h"
#include "EffectSprite.h"
#include "spine/SkeletonAnimation.h"
using namespace spine;
//
// EffectSprite
//
class EffectSpineFuncData {
public:
	EffectSpineFuncData(const string & animName, int whereIndex, const CallFunc_Function_StringIntAndNode & callback) {
		name = animName;
		index = whereIndex;
		activate = false;
		callFunc = callback;
	};
	EffectSpineFuncData() {
		activate = false;
		callFunc = nullptr;
		funcUser = nullptr;
	}
	~EffectSpineFuncData() {};

	void ActivateFunc(Node * who = nullptr) {
		if (callFunc != nullptr) {
			callFunc(name, index, who);
		}
	}
	string name;
	int index;
	bool activate;
	CallFunc_Function_StringIntAndNode callFunc;
	Ref * funcUser;
};


class EffectSpine : public Layout
{
public:
	static EffectSpine *create(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1);
	void PlayWithActionName(std::string name);
protected:
	EffectSpine();
	~EffectSpine();
	void OnStartListener(int trackIndex);
	void OnEndListener(int trackIndex);
	void OnCompleteListener(int trackIndex, int loopCount);
	void OnEventListener(int trackIndex, spEvent* event);
private:
	bool init(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1);
	void update(float delta);
	vector<EffectSpineFuncData> m_CallFunc;
	SkeletonAnimation * m_Spine;
	std::string m_CurActionName;
};


#endif //!_EffectSpine_H_