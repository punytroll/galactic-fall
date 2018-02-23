#include "point.h"
#include "triangle_point.h"

std::vector< Triangle * > Point::GetTriangles(void) const
{
	std::vector< Triangle * > Result;
	
	for(auto TrianglePoint : _TrianglePoints)
	{
		for(auto Triangle : TrianglePoint->GetTriangles())
		{
			Result.push_back(Triangle);
		}
	}
	
	return Result;
}
