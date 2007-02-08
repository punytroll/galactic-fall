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
	void SetFieldOfView(float FieldOfView);
	Position * GetFocus(void);
	const Position * GetFocus(void) const;
	const math3d::vector3f & GetPosition(void) const;
	float GetFieldOfView(void) const;
	void ZoomIn(void);
	void ZoomOut(void);
private:
	Position * m_Focus;
	math3d::vector3f m_Position;
	float m_FieldOfView;
};

inline void Camera::SetFieldOfView(float FieldOfView)
{
	m_FieldOfView = FieldOfView;
}

inline Position * Camera::GetFocus(void)
{
	return m_Focus;
}

inline const Position * Camera::GetFocus(void) const
{
	return m_Focus;
}

inline const math3d::vector3f & Camera::GetPosition(void) const
{
	return m_Position;
}

inline float Camera::GetFieldOfView(void) const
{
	return m_FieldOfView;
}

#endif
