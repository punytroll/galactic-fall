#include <algorithm>
#include <cassert>

#include <GL/gl.h>

#include "point.h"
#include "triangle_point.h"

TrianglePoint::TrianglePoint(void) :
	_Point(nullptr)
{
}

TrianglePoint::~TrianglePoint(void)
{
	if(_Point != nullptr)
	{
		_Point->_TrianglePoints.erase(std::find(_Point->_TrianglePoints.begin(), _Point->_TrianglePoints.end(), this));
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
		_Point->_TrianglePoints.push_back(this);
	}
}
