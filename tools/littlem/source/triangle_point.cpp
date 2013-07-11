#include <GL/gl.h>

#include "point.h"
#include "triangle_point.h"

TrianglePoint::TrianglePoint(void) :
	m_pPoint(0)
{
}

TrianglePoint::TrianglePoint(Point * pPoint) :
	m_pPoint(pPoint)
{
}

void TrianglePoint::vDraw(void)
{
	glNormal3fv(m_Normal.GetPointer());
	m_pPoint->Draw();
}

void TrianglePoint::vDrawSelection(void)
{
	m_pPoint->DrawSelection();
}
