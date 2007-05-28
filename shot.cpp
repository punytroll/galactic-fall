#include <GL/gl.h>

#include <math3d/vector4f.h>

#include "game_time.h"
#include "shot.h"

Shot::Shot(PhysicalObject * Shooter, float AngularPosition, const math3d::vector2f & Velocity) :
	m_Shooter(Shooter),
	m_TimeOfDeath(GameTime::Get() + 2.5),
	m_AngularPosition(AngularPosition),
	m_Velocity(Velocity)
{
	SetRadialSize(0.54f);
}

Shot::~Shot(void)
{
}

void Shot::Draw(void) const
{
	glPushAttrib(GL_ENABLE_BIT);
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
	glColor4fv(math3d::vector4f(0.8f, 0.2f, 0.2f, 0.8f).m_V.m_A);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -0.2f);
	glVertex2f(0.5f, -0.2f);
	glVertex2f(0.5f, 0.2f);
	glVertex2f(-0.5f, 0.2f);
	glEnd();
	glPopMatrix();
	glPopAttrib();
}

bool Shot::Update(float Seconds)
{
	if(m_TimeOfDeath < GameTime::Get())
	{
		return false;
	}
	m_Position += m_Velocity * Seconds;
	
	return true;
}
