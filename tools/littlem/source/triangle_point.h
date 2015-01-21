#ifndef LITTLEM_TRIANGLE_POINT_H
#define LITTLEM_TRIANGLE_POINT_H

#include <vector>

#include <algebra/vector3f.h>

class Point;
class Triangle;

class TrianglePoint
{
public:
	// constructor & destructor
	TrianglePoint(void);
	~TrianglePoint(void);
	// getters
	Point * GetPoint(void);
	const Point * GetPoint(void) const;
	const std::vector< Triangle * > & GetTriangles(void) const;
	// setters
	void SetPoint(Point * Point);
	Vector3f _Normal;
	std::vector< Triangle * > _Triangles;
private:
	Point * _Point;
};

#endif
