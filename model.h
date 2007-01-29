#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include <math3d/vector4f.h>

class Model
{
public:
	struct Triangle
	{
		std::vector< math3d::vector4f >::size_type Points[3];
		math3d::vector4f Normals[3];
	};
	
	Model(const std::string & Identifier);
	void Clear(void);
	void Draw(void) const;
	const std::string & GetIdentifier(void) const;
	float GetRadialSize(void) const;
	std::vector< math3d::vector4f >::size_type AddPoint(const math3d::vector4f & Point);
	std::vector< Model::Triangle >::size_type AddTriangle(const Model::Triangle & Triangle);
	std::vector< Model::Triangle >::size_type AddTriangle(std::vector< math3d::vector4f >::size_type Point1Index, const math3d::vector4f & Point1Normal, std::vector< math3d::vector4f >::size_type Point2Index, const math3d::vector4f & Point2Normal, std::vector< math3d::vector4f >::size_type Point3Index, const math3d::vector4f & Point3Normal);
private:
	std::string m_Identifier;
	std::vector< math3d::vector4f > m_Points;
	std::vector< Model::Triangle > m_Triangles;
	mutable float m_RadialSize;
};

inline const std::string & Model::GetIdentifier(void) const
{
	return m_Identifier;
}

#endif
