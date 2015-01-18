#include <assert.h>

#include "point.h"
#include "triangle.h"
#include "triangle_point.h"

Triangle::Triangle(void)
{
	m_ppTrianglePoints[0] = 0;
	m_ppTrianglePoints[1] = 0;
	m_ppTrianglePoints[2] = 0;
}

void Triangle::vRealignNormal(void)
{
	Vector3f Delta1(pGetPoint(2)->GetPosition() - pGetPoint(1)->GetPosition());
	Vector3f Delta2(pGetPoint(3)->GetPosition() - pGetPoint(1)->GetPosition());
	Vector3f Normal(Delta1.Cross(Delta2));
	
	Normal.Normalize();
	pGetTrianglePoint(1)->m_Normal = pGetTrianglePoint(2)->m_Normal = pGetTrianglePoint(3)->m_Normal = Normal;
}

Vector3f Triangle::GetTriangleCenter(void) const
{
	return Vector3f(pGetPoint(1)->GetPosition() + pGetPoint(2)->GetPosition() + pGetPoint(3)->GetPosition()) / 3.0f;
}

Vector3f Triangle::GetTriangleNormal(void) const
{
	return Vector3f((pGetPoint(2)->GetPosition() - pGetPoint(1)->GetPosition()).Cross(pGetPoint(3)->GetPosition() - pGetPoint(1)->GetPosition())).Normalized();
}

void Triangle::vDraw(void)
{
	pGetTrianglePoint(1)->vDraw();
	pGetTrianglePoint(2)->vDraw();
	pGetTrianglePoint(3)->vDraw();
}

void Triangle::vDrawSelection(void)
{
	pGetTrianglePoint(1)->vDrawSelection();
	pGetTrianglePoint(2)->vDrawSelection();
	pGetTrianglePoint(3)->vDrawSelection();
}

void Triangle::vInvert(void)
{
	std::swap(m_ppTrianglePoints[1], m_ppTrianglePoints[2]);
}

Point * Triangle::pGetPoint(int iPoint)
{
	if((iPoint >= 1) && (iPoint <= 3))
	{
		return pGetTrianglePoint(iPoint)->m_Point;
	}
	
	return 0;
}

const Point * Triangle::pGetPoint(int iPoint) const
{
	if((iPoint >= 1) && (iPoint <= 3))
	{
		return pGetTrianglePoint(iPoint)->m_Point;
	}
	
	return 0;
}

TrianglePoint * Triangle::pGetTrianglePoint(int iTrianglePoint)
{
	assert((iTrianglePoint >= 1) && (iTrianglePoint <= 3));
	
	return m_ppTrianglePoints[iTrianglePoint - 1];
}

const TrianglePoint * Triangle::pGetTrianglePoint(int iTrianglePoint) const
{
	assert((iTrianglePoint >= 1) && (iTrianglePoint <= 3));
	
	return m_ppTrianglePoints[iTrianglePoint - 1];
}

TrianglePoint * Triangle::pGetTrianglePoint(Point * pPoint)
{
	if(pPoint == pGetPoint(1))
	{
		return m_ppTrianglePoints[0];
	}
	else if(pPoint == pGetPoint(2))
	{
		return m_ppTrianglePoints[1];
	}
	else if(pPoint == pGetPoint(3))
	{
		return m_ppTrianglePoints[2];
	}
	
	return 0;
}

const TrianglePoint * Triangle::pGetTrianglePoint(Point * pPoint) const
{
	if(pPoint == pGetPoint(1))
	{
		return m_ppTrianglePoints[0];
	}
	else if(pPoint == pGetPoint(2))
	{
		return m_ppTrianglePoints[1];
	}
	else if(pPoint == pGetPoint(3))
	{
		return m_ppTrianglePoints[2];
	}
	
	return 0;
}

void Triangle::vSetTrianglePoint(int iTrianglePoint, TrianglePoint * pTrianglePoint)
{
	assert((iTrianglePoint >= 1) && (iTrianglePoint <= 3));
	
	m_ppTrianglePoints[iTrianglePoint - 1] = pTrianglePoint;;
}
