#ifndef CARGO_H
#define CARGO_H

#include <math3d/vector2f.h>
#include <math3d/vector3f.h>

#include "physical_object.h"

class Commodity;
class Model;

class Cargo : public PhysicalObject
{
public:
	Cargo(Model * Model, const Commodity * Commodity, const math3d::vector2f & Velocity);
	virtual ~Cargo(void);
	// modifiers
	virtual void Draw(void) const;
	void Move(float Seconds);
	// getters
	const Commodity * GetCommodity(void) const;
	float GetHull(void) const;
	const math3d::vector2f & GetVelocity(void) const;
	// setters
	void SetHull(float Hull);
private:
	Model * m_Model;
	const Commodity * m_Commodity;
	float m_Hull;
	math3d::vector2f m_Velocity;
	float m_AngularPosition;
	math3d::vector3f m_RotationAxis;
	float m_AngularVelocity;
};

inline const Commodity * Cargo::GetCommodity(void) const
{
	return m_Commodity;
}

inline float Cargo::GetHull(void) const
{
	return m_Hull;
}

inline const math3d::vector2f & Cargo::GetVelocity(void) const
{
	return m_Velocity;
}

inline void Cargo::SetHull(float Hull)
{
	m_Hull = Hull;
}

#endif
