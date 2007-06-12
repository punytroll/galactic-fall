#include <GL/gl.h>

#include "cargo.h"
#include "color.h"
#include "commodity.h"
#include "model.h"

Cargo::Cargo(const Commodity * Commodity) :
	m_Commodity(Commodity),
	m_Hull(20.0f),
	m_Velocity(true)
{
	SetRadialSize(m_Commodity->GetModel()->GetRadialSize());
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
	glMaterialfv(GL_FRONT, GL_SPECULAR, math3d::vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Commodity->GetColor()->GetColor().m_V.m_A);
	m_Commodity->GetModel()->Draw();
	glPopMatrix();
}

void Cargo::Move(float Seconds)
{
	m_Position += m_Velocity * Seconds;
	m_AngularPosition += m_AngularVelocity * Seconds;
}
