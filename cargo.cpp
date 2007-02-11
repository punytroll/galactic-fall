#include <GL/gl.h>

#include "cargo.h"
#include "color.h"
#include "commodity.h"
#include "model.h"

Cargo::Cargo(Model * Model, const Commodity * Commodity, const math3d::vector2f & Velocity) :
	m_Model(Model),
	m_Commodity(Commodity),
	m_Velocity(Velocity)
{
	SetRadialSize(Model->GetRadialSize());
	SetName(m_Commodity->GetName());
	m_RotationAxis.set(static_cast< float >(random()) / RAND_MAX, static_cast< float >(random()) / RAND_MAX, static_cast< float >(random()) / RAND_MAX);
	m_RotationAxis.normalize();
	m_AngularPosition = static_cast< float >(random()) / RAND_MAX;
	m_AngularVelocity = static_cast< float >(random()) / RAND_MAX;
}

Cargo::~Cargo(void)
{
}

void Cargo::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, m_RotationAxis.m_V.m_A[0], m_RotationAxis.m_V.m_A[1], m_RotationAxis.m_V.m_A[2]);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, math3d::vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Commodity->GetColor()->GetColor().m_V.m_A);
	m_Model->Draw();
	glPopMatrix();
}

void Cargo::Move(float Seconds)
{
	m_Position += m_Velocity * Seconds;
	m_AngularPosition += m_AngularVelocity * Seconds;
}
