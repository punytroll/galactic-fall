#include <GL/gl.h>

#include <algorithm>
#include <cassert>

#include "point.h"
#include "triangle.h"
#include "triangle_point.h"

Triangle::Triangle(void)
{
	m_ppTrianglePoints[0] = 0;
	m_ppTrianglePoints[1] = 0;
	m_ppTrianglePoints[2] = 0;
}

void Triangle::RealignNormal(void)
{
	Vector3f Delta1(GetPoint<1>()->GetPosition() - GetPoint<0>()->GetPosition());
	Vector3f Delta2(GetPoint<2>()->GetPosition() - GetPoint<0>()->GetPosition());
	Vector3f Normal(Delta1.Cross(Delta2));
	
	Normal.Normalize();
	GetTrianglePoint<0>()->_Normal = GetTrianglePoint<1>()->_Normal = GetTrianglePoint<2>()->_Normal = Normal;
}

Vector3f Triangle::GetTriangleCenter(void) const
{
	return Vector3f(GetPoint<0>()->GetPosition() + GetPoint<1>()->GetPosition() + GetPoint<2>()->GetPosition()) / 3.0f;
}

Vector3f Triangle::GetTriangleNormal(void) const
{
	return Vector3f((GetPoint<1>()->GetPosition() - GetPoint<0>()->GetPosition()).Cross(GetPoint<2>()->GetPosition() - GetPoint<0>()->GetPosition())).Normalized();
}

void Triangle::Invert(void)
{
	std::swap(m_ppTrianglePoints[1], m_ppTrianglePoints[2]);
}

TrianglePoint * Triangle::GetTrianglePoint(Point * Point)
{
	if(Point == GetPoint<0>())
	{
		return m_ppTrianglePoints[0];
	}
	else if(Point == GetPoint<1>())
	{
		return m_ppTrianglePoints[1];
	}
	else if(Point == GetPoint<2>())
	{
		return m_ppTrianglePoints[2];
	}
	
	return 0;
}

const TrianglePoint * Triangle::GetTrianglePoint(Point * Point) const
{
	if(Point == GetPoint<0>())
	{
		return m_ppTrianglePoints[0];
	}
	else if(Point == GetPoint<1>())
	{
		return m_ppTrianglePoints[1];
	}
	else if(Point == GetPoint<2>())
	{
		return m_ppTrianglePoints[2];
	}
	
	return 0;
}

void Triangle::_SetTrianglePoint(int Slot, TrianglePoint * TrianglePoint)
{
	if(m_ppTrianglePoints[Slot] != nullptr)
	{
		m_ppTrianglePoints[Slot]->_Triangles.erase(std::find(m_ppTrianglePoints[Slot]->_Triangles.begin(), m_ppTrianglePoints[Slot]->_Triangles.end(), this));
		if(m_ppTrianglePoints[Slot]->_Triangles.size() == 0)
		{
			delete m_ppTrianglePoints[Slot];
			m_ppTrianglePoints[Slot] = nullptr;
		}
	}
	m_ppTrianglePoints[Slot] = TrianglePoint;
	if(TrianglePoint != nullptr)
	{
		TrianglePoint->_Triangles.push_back(this);
	}
}
