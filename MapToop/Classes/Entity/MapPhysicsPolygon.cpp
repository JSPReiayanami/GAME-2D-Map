#include "MapPhysicsPolygon.h"
MapPhysicsPolygon::MapPhysicsPolygon()
{
	m_Body = nullptr;
}
MapPhysicsPolygon::~MapPhysicsPolygon()
{

}

MapPhysicsPolygon *MapPhysicsPolygon::create(const std::string & filename) {
	auto ret = new (std::nothrow) MapPhysicsPolygon;
	if (ret && ret->initWithFile(filename)) {
		ret->m_ImagePath = filename;
		ret->autorelease();
		ret->InitPhysics();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}
MapPhysicsPolygon * MapPhysicsPolygon::createWithId(int _id)
{
	auto ret = new (std::nothrow) MapPhysicsPolygon;
	if (ret && ret->InitWithId(_id)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

bool MapPhysicsPolygon::InitWithId(int _id)
{
	if (!this->initWithFile(""))
	{
		return false;
	}

	InitPhysics();
	return true;
}

void MapPhysicsPolygon::InitPhysics()
{
	/*auto plinfo = AutoPolygon::generatePolygon(m_ImagePath, Rect::ZERO,2);
	this->setPolygonInfo(plinfo);*/
	//DrawNode * drawnode = DrawNode::create(3);
	//drawnode->clear();
	//auto count = plinfo.triangles.indexCount / 3;
	//auto indices = plinfo.triangles.indices;
	//auto verts = plinfo.triangles.verts;
	//for (ssize_t i = 0; i < count; i++)
	//{
	//	//draw 3 lines
	//	Vec3 from = verts[indices[i * 3]].vertices;
	//	Vec3 to = verts[indices[i * 3 + 1]].vertices;
	//	drawnode->drawLine(Vec2(from.x, from.y), Vec2(to.x, to.y), Color4F::GREEN);

	//	from = verts[indices[i * 3 + 1]].vertices;
	//	to = verts[indices[i * 3 + 2]].vertices;
	//	drawnode->drawLine(Vec2(from.x, from.y), Vec2(to.x, to.y), Color4F::GREEN);

	//	from = verts[indices[i * 3 + 2]].vertices;
	//	to = verts[indices[i * 3]].vertices;
	//	drawnode->drawLine(Vec2(from.x, from.y), Vec2(to.x, to.y), Color4F::GREEN);
	//}
	//this->addChild(drawnode);
	//for (ssize_t i = 0; i < count; i++)
	//{
	//	//draw 3 lines
	//	Vec2 * vecc = new Vec2[3];
	//	Vec3 from = verts[indices[i * 3]].vertices;
	//	Vec3 to = verts[indices[i * 3 + 1]].vertices;
	//	vecc[0].x = from.x;
	//	vecc[0].y = from.y;
	//	vecc[1].x = to.x;
	//	vecc[1].y = to.y;
	//	to = verts[indices[i * 3 + 2]].vertices;
	//	vecc[2].x = to.x;
	//	vecc[2].y = to.y;
	//	PhysicsBody * body = PhysicsBody::createEdgePolygon(vecc, 3);
	//	this->addComponent(body);
	//	CC_SAFE_DELETE_ARRAY(vecc);
	//}
	//vec[count] = vec[0];
	/*vec[0] = Vec2(0, 0);
	vec[1] = Vec2(0, 100);
	vec[2] = Vec2(100, 100);*/

	PhysicsBody * body = nullptr;
	body = PhysicsBody::createBox(this->getContentSize());
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);

	this->addComponent(body);
}

