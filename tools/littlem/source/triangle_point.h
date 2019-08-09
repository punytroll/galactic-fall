#ifndef LITTLEM_TRIANGLE_POINT_H
#define LITTLEM_TRIANGLE_POINT_H

#include <vector>

#include <algebra/vector3f.h>

class Point;
class Triangle;

class TrianglePoint
{
public:
	friend Triangle;
	
	// constructor & destructor
	TrianglePoint(void);
	~TrianglePoint(void);
	// getters
	Point * GetPoint(void)
	{
		return _Point;
	}
	
	const Point * GetPoint(void) const
	{
		return _Point;
	}
	
	const Vector3f & GetNormal(void) const
	{
		return _Normal;
	}
	
	const std::vector< Triangle * > & GetTriangles(void) const
	{
		return _Triangles;
	}
	
	// setters
	void SetNormal(const Vector3f & Normal)
	{
		_Normal = Normal;
	}
	
	void SetPoint(Point * Point);
private:
	Vector3f _Normal;
	Point * _Point;
	std::vector< Triangle * > _Triangles;
};

#endif
