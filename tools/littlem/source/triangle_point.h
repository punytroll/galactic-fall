#ifndef LITTLEM_TRIANGLE_POINT_H
#define LITTLEM_TRIANGLE_POINT_H

#include <vector>

#include <algebra/vector3f.h>

class Point;
class Triangle;

class TrianglePoint
{
public:
	TrianglePoint(void);
	TrianglePoint(Point * pPoint);

	const std::vector< Triangle * > & GetTriangles(void) const
	{
		return m_Triangles;
	}
	
	void vDraw(void);
	void vDrawSelection(void);
	Point * m_pPoint;
	Vector3f m_Normal;
	std::vector< Triangle * > m_Triangles;
};

#endif
