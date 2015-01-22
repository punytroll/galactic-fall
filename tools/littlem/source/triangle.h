#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <algebra/vector4f.h>

class Point;
class TrianglePoint;

class Triangle
{
public:
	Triangle(void);
	
	template< int Slot >
	void SetTrianglePoint(TrianglePoint * TrianglePoint)
	{
		static_assert((Slot >= 0) && (Slot < 3), "Slot must be between 0 and 2 inclusive.");
		_SetTrianglePoint(Slot, TrianglePoint);
	}
	
	void vRealignNormal(void);
	Vector3f GetTriangleCenter(void) const;
	Vector3f GetTriangleNormal(void) const;
	void vInvert(void);
	Point * pGetPoint(int iPoint);
	const Point * pGetPoint(int iPoint) const;
	TrianglePoint * pGetTrianglePoint(int iTrianglePoint);
	const TrianglePoint * pGetTrianglePoint(int iTrianglePoint) const;
	TrianglePoint * pGetTrianglePoint(Point * pPoint);
	const TrianglePoint * pGetTrianglePoint(Point * pPoint) const;
	TrianglePoint * m_ppTrianglePoints[3];
private:
	void _SetTrianglePoint(int Slot, TrianglePoint * TrianglePoint);
};

#endif
