#include "PhysicsScene.h"
PhysicsScene::PhysicsScene(){
	m_WorldBox = nullptr;
	m_PhysicsWorld = nullptr;
}
PhysicsScene::~PhysicsScene(){

}

cocos2d::Scene* PhysicsScene::createScene()
{
	Scene * scene = Scene::createWithPhysics();

	PhysicsScene * p_s = PhysicsScene::create();
	p_s->m_PhysicsWorld = scene->getPhysicsWorld();
	p_s->m_PhysicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	p_s->m_PhysicsWorld->setGravity(Vec2(1,0));
	scene->addChild(p_s);
	return scene;
}

bool PhysicsScene::init()
{
	if (!Layer::init())
		return false;
	IsOpenWorldBox(true);
	PhysicsBody * cir = MakeCircle(50, PhysicsMaterial(0.1f, 0.0f, 0.0f));
	return true;
}

void PhysicsScene::IsOpenWorldBox(bool is)
{
	if (m_WorldBox != nullptr)m_WorldBox->removeFromWorld();
	if (is)
	{
		m_WorldBox = PhysicsBody::createEdgeBox(__VisibleSize, PhysicsMaterial(1, 0, 0), __VisibleSize_Width*0.005);
		m_WorldBox->setDynamic(false);
		this->addComponent(m_WorldBox);
	}
}

PhysicsBody * PhysicsScene::MakeCircle(float dis, PhysicsMaterial material){
	PhysicsBody * circle = PhysicsBody::createCircle(dis, material);
	Layer * lay = Layer::create();
	lay->setContentSize(Size(dis,dis));
	lay->addComponent(circle);
	this->addChild(lay);
	lay->setPosition(__VisibleSize / 2);
	return circle;
}