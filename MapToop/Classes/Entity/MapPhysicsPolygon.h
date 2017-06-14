#ifndef _MapPhysicsPolygon_H_
#define _MapPhysicsPolygon_H_
#include "Config/Head.h"

class MapPhysicsPolygon : public Sprite
{
public:
	enum BodyType
	{

	};
public:
	MapPhysicsPolygon();
	~MapPhysicsPolygon();
	static MapPhysicsPolygon * create(const std::string & filename);
	static MapPhysicsPolygon * createWithId(int _id);
	bool InitWithId(int _id);
	void InitPhysics();
private:
	PhysicsBody * m_Body;
	string  m_ImagePath;
};


#endif //!MapPhysicsPolygon