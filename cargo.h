#ifndef CARGO_H
#define CARGO_H

#include <math3d/vector2f.h>
#include <math3d/vector3f.h>

#include "position.h"

class Commodity;
class Model;

class Cargo : public Position
{
public:
	Cargo(Model * Model, const Commodity * Commodity, const math3d::vector2f & Velocity);
	void Draw(void) const;
	void Move(float Seconds);
	const Commodity * GetCommodity(void) const;
	float GetRadialSize(void) const;
private:
	Model * m_Model;
	const Commodity * m_Commodity;
	math3d::vector2f m_Velocity;
	float m_AngularPosition;
	math3d::vector3f m_RotationAxis;
	float m_AngularVelocity;
};

inline const Commodity * Cargo::GetCommodity(void) const
{
	return m_Commodity;
}

inline float Cargo::GetRadialSize(void) const
{
	return 0.867f;  // sqrt(sqr(0.5) + sqr(0.5) + sqr(0.5))
}

#endif
