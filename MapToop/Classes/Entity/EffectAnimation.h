#ifndef _EffectArmature_H_
#define _EffectArmature_H_
#include "Config/Head.h"
#include "EffectSprite.h"
//
// EffectSprite
//

class EffectArmatureFuncData{
public:
	EffectArmatureFuncData(const string & animName, int whereIndex,const CallFunc_Function_StringIntAndNode & callback){
		name = animName;
		index = whereIndex;
		activate = false;
		callFunc = callback;
	};
	EffectArmatureFuncData(){
		activate = false;
		callFunc = nullptr;
		funcUser = nullptr;
	}
	~EffectArmatureFuncData(){};

	void ActivateFunc(Node * who = nullptr){
		if (callFunc != nullptr){
			callFunc(name, index, who);
		}
	}
	string name;
	int index;
	bool activate;
	CallFunc_Function_StringIntAndNode callFunc;
	Ref * funcUser;
};

class EffectArmature : public EffectSprite
{
public:
	static EffectArmature *create(const std::string& filename);
	bool AddAnimate(Animate * animate,const string & name);
	bool RemoveAnimate(const string & name);
	void PlayAnimation(const string & name);
	void StopPlay();
	void ContinuePlay();
	void AddAnimateCallFunc(EffectArmatureFuncData & callFunc);
protected:
	EffectArmature();
	~EffectArmature();
	map<string, Animate *> m_Animates;
	vector<EffectArmatureFuncData> m_CallFunc;
	string m_CurPlayName;
	Action * m_CurAction;
private:
	void update(float delta);
};


#endif //!_EffectArmature_H_