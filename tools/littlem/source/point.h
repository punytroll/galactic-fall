#ifndef LITTLEM_POINT_H
#define LITTLEM_POINT_H

#include <vector>

#include <algebra/vector3f.h>

class Triangle;
class TrianglePoint;

class Point
{
public:
	const Vector3f & GetPosition(void) const
	{
		return _Position;
	}
	
	const std::vector< TrianglePoint * > & GetTrianglePoints(void) const
	{
		return _TrianglePoints;
	}
	
	std::vector< Triangle * > GetTriangles(void) const;
	
	void SetPosition(const Vector3f & Position)
	{
		_Position = Position;
	}
	
	std::vector< TrianglePoint * > _TrianglePoints;
private:
	Vector3f _Position;
};

#endif
