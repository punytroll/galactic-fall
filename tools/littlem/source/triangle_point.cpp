#include <algorithm>
#include <cassert>

#include "point.h"
#include "triangle_point.h"

TrianglePoint::TrianglePoint(void) :
	_Normal(Vector3f::CreateFromComponents(1.0f, 0.0f, 0.0f)),
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
