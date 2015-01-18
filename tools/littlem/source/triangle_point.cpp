#include <GL/gl.h>

#include "point.h"
#include "triangle_point.h"

TrianglePoint::TrianglePoint(void) :
	m_Point(nullptr)
{
}

TrianglePoint::TrianglePoint(Point * Point) :
	m_Point(Point)
{
}

void TrianglePoint::vDraw(void)
{
	glNormal3fv(m_Normal.GetPointer());
	glVertex3fv(m_Point->GetPosition().GetPointer());
}

void TrianglePoint::vDrawSelection(void)
{
	glVertex3fv(m_Point->GetPosition().GetPointer());
}
