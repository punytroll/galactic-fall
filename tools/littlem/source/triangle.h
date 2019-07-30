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
	
	void RealignNormal(void);
	Vector3f GetTriangleCenter(void) const;
	Vector3f GetTriangleNormal(void) const;
	void Invert(void);
	Point * GetPoint(int Point);
	const Point * GetPoint(int Point) const;
	TrianglePoint * GetTrianglePoint(int TrianglePoint);
	const TrianglePoint * GetTrianglePoint(int TrianglePoint) const;
	TrianglePoint * GetTrianglePoint(Point * Point);
	const TrianglePoint * GetTrianglePoint(Point * Point) const;
	TrianglePoint * m_ppTrianglePoints[3];
private:
	void _SetTrianglePoint(int Slot, TrianglePoint * TrianglePoint);
};

#endif
