#include <algorithm>
#include <cassert>

#include <GL/gl.h>

#include "point.h"
#include "triangle_point.h"

extern std::vector< TrianglePoint * > g_TrianglePoints;

TrianglePoint::TrianglePoint(void) :
	_Point(nullptr)
{
	g_TrianglePoints.push_back(this);
}

TrianglePoint::~TrianglePoint(void)
{
	g_TrianglePoints.erase(std::find(g_TrianglePoints.begin(), g_TrianglePoints.end(), this));
	if(_Point != nullptr)
	{
		_Point->m_TrianglePoints.erase(std::find(_Point->m_TrianglePoints.begin(), _Point->m_TrianglePoints.end(), this));
	}
}

Point * TrianglePoint::GetPoint(void)
{
	return _Point;
}

const Point * TrianglePoint::GetPoint(void) const
{
	return _Point;
}

const std::vector< Triangle * > & TrianglePoint::GetTriangles(void) const
{
	return _Triangles;
}

void TrianglePoint::SetPoint(Point * Point)
{
	if(_Point != nullptr)
	{
		assert(false);
	}
	else
	{
		_Point = Point;
		_Point->m_TrianglePoints.push_back(this);
	}
}
