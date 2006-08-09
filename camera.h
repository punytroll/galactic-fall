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
	void SetPosition(float X, float Y);
	void SetPosition(float X, float Y, float Z);
	const Position * GetFocus(void) const;
	const math3d::vector3f & GetPosition(void) const;
	void ZoomIn(void);
	void ZoomOut(void);
private:
	const Position * m_Focus;
	math3d::vector3f m_Position;
};

inline const Position * Camera::GetFocus(void) const
{
	return m_Focus;
}

inline const math3d::vector3f & Camera::GetPosition(void) const
{
	return m_Position;
}

#endif
