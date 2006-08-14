#ifndef OBJECT_H
#define OBJECT_H

#include "position.h"

class Object : public Position
{
public:
	virtual void Draw(void) const = 0;
	void SetRadialSize(float RadialSize);
	float GetRadialSize(void) const;
private:
	float m_RadialSize;
};

inline float Object::GetRadialSize(void) const
{
	return m_RadialSize;
}

#endif
