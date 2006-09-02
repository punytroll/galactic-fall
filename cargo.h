#ifndef CARGO_H
#define CARGO_H

#include <math3d/vector2f.h>
#include <math3d/vector3f.h>

#include "object.h"

class Commodity;
class Model;

class Cargo : public Object
{
public:
	Cargo(Model * Model, const Commodity * Commodity, const math3d::vector2f & Velocity);
	virtual ~Cargo(void);
	virtual void Draw(void) const;
	void Move(float Seconds);
	const Commodity * GetCommodity(void) const;
	const math3d::vector2f & GetVelocity(void);
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

inline const math3d::vector2f & Cargo::GetVelocity(void)
{
	return m_Velocity;
}

#endif
