#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#include <math3d/matrix4f.h>

class Perspective
{
public:
	Perspective(void);
	void Draw(void) const;
	// setters
	void SetAspect(float Aspect);
	void SetFieldOfView(float FieldOfView);
	void SetNearClippingPlane(float NearClippingPlane);
	void SetFarClippingPlane(float FarClippingPlane);
private:
	mutable bool m_Initialized;
	float m_Aspect;
	float m_FieldOfView;
	float m_NearClippingPlane;
	float m_FarClippingPlane;
	mutable math3d::matrix4f m_Matrix;
};

inline void Perspective::SetAspect(float Aspect)
{
	m_Aspect = Aspect;
	m_Initialized = false;
}

inline void Perspective::SetFieldOfView(float FieldOfView)
{
	m_FieldOfView = FieldOfView;
	m_Initialized = false;
}

inline void Perspective::SetNearClippingPlane(float NearClippingPlane)
{
	m_NearClippingPlane = NearClippingPlane;
	m_Initialized = false;
}

inline void Perspective::SetFarClippingPlane(float FarClippingPlane)
{
	m_FarClippingPlane = FarClippingPlane;
	m_Initialized = false;
}


#endif
