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

void Triangle::vRealignNormal(void)
{
	Vector3f Delta1(pGetPoint(2)->GetPosition() - pGetPoint(1)->GetPosition());
	Vector3f Delta2(pGetPoint(3)->GetPosition() - pGetPoint(1)->GetPosition());
	Vector3f Normal(Delta1.Cross(Delta2));
	
	Normal.Normalize();
	pGetTrianglePoint(1)->_Normal = pGetTrianglePoint(2)->_Normal = pGetTrianglePoint(3)->_Normal = Normal;
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
	glNormal3fv(m_ppTrianglePoints[0]->_Normal.GetPointer());
	glVertex3fv(m_ppTrianglePoints[0]->GetPoint()->GetPosition().GetPointer());
	glNormal3fv(m_ppTrianglePoints[1]->_Normal.GetPointer());
	glVertex3fv(m_ppTrianglePoints[1]->GetPoint()->GetPosition().GetPointer());
	glNormal3fv(m_ppTrianglePoints[2]->_Normal.GetPointer());
	glVertex3fv(m_ppTrianglePoints[2]->GetPoint()->GetPosition().GetPointer());
}

void Triangle::vDrawSelection(void)
{
	glVertex3fv(m_ppTrianglePoints[0]->GetPoint()->GetPosition().GetPointer());
	glVertex3fv(m_ppTrianglePoints[1]->GetPoint()->GetPosition().GetPointer());
	glVertex3fv(m_ppTrianglePoints[2]->GetPoint()->GetPosition().GetPointer());
}

void Triangle::vInvert(void)
{
	std::swap(m_ppTrianglePoints[1], m_ppTrianglePoints[2]);
}

Point * Triangle::pGetPoint(int iPoint)
{
	if((iPoint >= 1) && (iPoint <= 3))
	{
		return pGetTrianglePoint(iPoint)->GetPoint();
	}
	
	return 0;
}

const Point * Triangle::pGetPoint(int iPoint) const
{
	if((iPoint >= 1) && (iPoint <= 3))
	{
		return pGetTrianglePoint(iPoint)->GetPoint();
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

void Triangle::_SetTrianglePoint(int Slot, TrianglePoint * TrianglePoint)
{
	assert(TrianglePoint != nullptr);
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
