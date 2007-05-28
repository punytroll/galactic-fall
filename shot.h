#ifndef SHOT_H
#define SHOT_H

#include <math3d/vector2f.h>

#include "physical_object.h"

class Model;

class Shot : public PhysicalObject
{
public:
	Shot(PhysicalObject * Shooter, float AngularPosition, const math3d::vector2f & Velocity);
	virtual ~Shot(void);
	virtual void Draw(void) const;
	bool Update(float Seconds);
	const PhysicalObject * GetShooter(void) const;
	const math3d::vector2f & GetVelocity(void) const;
	float GetDamage(void) const;
private:
	PhysicalObject * m_Shooter;
	double m_TimeOfDeath;
	math3d::vector2f m_Velocity;
	float m_AngularPosition;
	float m_Damage;
};

inline const PhysicalObject * Shot::GetShooter(void) const
{
	return m_Shooter;
}

inline const math3d::vector2f & Shot::GetVelocity(void) const
{
	return m_Velocity;
}

inline float Shot::GetDamage(void) const
{
	return m_Damage;
}

#endif

