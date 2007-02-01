#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include "position.h"

class PhysicalObject : public Position
{
public:
	virtual ~PhysicalObject(void);
	virtual void Draw(void) const = 0;
	void SetRadialSize(float RadialSize);
	float GetRadialSize(void) const;
private:
	float m_RadialSize;
};

inline float PhysicalObject::GetRadialSize(void) const
{
	return m_RadialSize;
}

#endif
