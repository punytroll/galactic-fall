#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <algebra/vector4f.h>

class Point;
class TrianglePoint;

class Triangle
{
public:
	Triangle(void);
	void vRealignNormal(void);
	Vector3f GetTriangleCenter(void) const;
	Vector3f GetTriangleNormal(void) const;
	void vDraw(void);
	void vDrawSelection(void);
	void vInvert(void);
	Point * pGetPoint(int iPoint);
	const Point * pGetPoint(int iPoint) const;
	TrianglePoint * pGetTrianglePoint(int iTrianglePoint);
	const TrianglePoint * pGetTrianglePoint(int iTrianglePoint) const;
	TrianglePoint * pGetTrianglePoint(Point * pPoint);
	const TrianglePoint * pGetTrianglePoint(Point * pPoint) const;
	void vSetTrianglePoint(int iTrianglePoint, TrianglePoint * pTrianglePoint);
	TrianglePoint * m_ppTrianglePoints[3];
};

#endif
