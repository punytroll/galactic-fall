#include <GL/gl.h>

#include "cargo.h"
#include "model.h"

Cargo::Cargo(Model * Model, const Commodity * Commodity, const math3d::vector2f & Velocity) :
	m_Model(Model),
	m_Commodity(Commodity),
	m_Velocity(Velocity)
{
	m_RotationAxis.set(static_cast< float >(random()) / RAND_MAX, static_cast< float >(random()) / RAND_MAX, static_cast< float >(random()) / RAND_MAX);
	m_RotationAxis.normalize();
	m_AngularPosition = static_cast< float >(random()) / RAND_MAX;
	m_AngularVelocity = static_cast< float >(random()) / RAND_MAX;
}

void Cargo::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, m_RotationAxis.m_V.m_A[0], m_RotationAxis.m_V.m_A[1], m_RotationAxis.m_V.m_A[2]);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	m_Model->Draw();
	glPopMatrix();
}

void Cargo::Move(float Seconds)
{
	m_Position += m_Velocity * Seconds;
	m_AngularPosition += m_AngularVelocity * Seconds;
}
