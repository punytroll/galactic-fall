#ifndef LITTLEM_POINT_H
#define LITTLEM_POINT_H

#include <vector>

#include "position.h"

class TrianglePoint;

class Point : public Position
{
public:
	Point(float fX, float fY, float fZ);
	std::vector< TrianglePoint * > m_TrianglePoints;
};

#endif
