#ifndef __Physics_Scene_H__
#define __Physics_Scene_H__
#include "Config/Head.h"
#include "cocos2d.h"

class PhysicsScene : public cocos2d::Layer
{
public:
	PhysicsScene();
	~PhysicsScene();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(PhysicsScene);


	cocos2d::PhysicsWorld * m_PhysicsWorld;
	PhysicsBody * m_WorldBox;
	void IsOpenWorldBox(bool is);
	PhysicsBody * MakeCircle(float dis, PhysicsMaterial material);
};

#endif // __Physics_Scene_H__