#ifndef LITTLEM_POINT_H
#define LITTLEM_POINT_H

#include <vector>

#include "position.h"

class TrianglePoint;

class Point : public Position
{
public:
	Point(float fX, float fY, float fZ);
	
	const std::vector< TrianglePoint * > & GetTrianglePoints(void) const
	{
		return m_TrianglePoints;
	}
	
	std::vector< TrianglePoint * > m_TrianglePoints;
};

#endif
