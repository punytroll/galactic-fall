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
	Point * GetPoint(void)
	{
		static_assert((Slot >= 0) && (Slot < 3), "Slot must be between 0 and 2 inclusive.");
		
		return GetTrianglePoint<Slot>()->GetPoint();
	}
	
	template< int Slot >
	const Point * GetPoint() const
	{
		static_assert((Slot >= 0) && (Slot < 3), "Slot must be between 0 and 2 inclusive.");
		
		return GetTrianglePoint<Slot>()->GetPoint();
	}
	
	template< int Slot >
	TrianglePoint * GetTrianglePoint(void)
	{
		static_assert((Slot >= 0) && (Slot < 3), "Slot must be between 0 and 2 inclusive.");
		
		return m_ppTrianglePoints[Slot];
	}
	
	template< int Slot >
	const TrianglePoint * GetTrianglePoint(void) const
	{
		static_assert((Slot >= 0) && (Slot < 3), "Slot must be between 0 and 2 inclusive.");
		
		return m_ppTrianglePoints[Slot];
	}
	
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
	TrianglePoint * GetTrianglePoint(Point * Point);
	const TrianglePoint * GetTrianglePoint(Point * Point) const;
private:
	void _SetTrianglePoint(int Slot, TrianglePoint * TrianglePoint);
	TrianglePoint * m_ppTrianglePoints[3];
};

#endif
