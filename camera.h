#ifndef CAMERA_H
#define CAMERA_H

#include <math3d/vector3f.h>

class Position;

class Camera
{
public:
	Camera(void);
	void Draw(void);
	void SetFocus(Position * Focus);
	const Position * GetFocus(void) const;
	math3d::vector3f m_Position;
private:
	const Position * m_Focus;
};

inline const Position * Camera::GetFocus(void) const
{
	return m_Focus;
}

#endif
