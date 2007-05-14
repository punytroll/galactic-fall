#ifndef SHOT_H
#define SHOT_H

#include <math3d/vector2f.h>

#include "physical_object.h"

class Model;

class Shot : public PhysicalObject
{
public:
	Shot(float AngularPosition, const math3d::vector2f & Velocity);
	virtual ~Shot(void);
	virtual void Draw(void) const;
	bool Update(float Seconds);
	const math3d::vector2f & GetVelocity(void) const;
private:
	double m_TimeOfDeath;
	math3d::vector2f m_Velocity;
	float m_AngularPosition;
};

inline const math3d::vector2f & Shot::GetVelocity(void) const
{
	return m_Velocity;
}

#endif

