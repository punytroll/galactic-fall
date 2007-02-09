#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include "position.h"

class PhysicalObject : public Position
{
public:
	virtual ~PhysicalObject(void);
	virtual void Draw(void) const = 0;
	const std::string & GetName(void) const;
	void SetName(const std::string & Name);
	void SetRadialSize(float RadialSize);
	float GetRadialSize(void) const;
private:
	std::string m_Name;
	float m_RadialSize;
};

inline const std::string & PhysicalObject::GetName(void) const
{
	return m_Name;
}

inline void PhysicalObject::SetName(const std::string & Name)
{
	m_Name = Name;
}

inline float PhysicalObject::GetRadialSize(void) const
{
	return m_RadialSize;
}

#endif
