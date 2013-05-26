#ifndef LITTLEM_TRIANGLE_POINT_H
#define LITTLEM_TRIANGLE_POINT_H

#include <vector>

#include <algebra/vector4f.h>

class Point;
class Triangle;

class TrianglePoint
{
public:
	TrianglePoint(void);
	TrianglePoint(Point * pPoint);
	void vDraw(void);
	void vDrawSelection(void);
	Point * m_pPoint;
	Vector4f m_Normal;
	std::vector< Triangle * > m_Triangles;
};

#endif
