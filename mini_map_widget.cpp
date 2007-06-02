#include <GL/gl.h>

#include "cargo.h"
#include "mini_map_widget.h"
#include "planet.h"
#include "ship.h"
#include "system.h"

MiniMapWidget::MiniMapWidget(void) :
	Widget(),
	m_Focus(0)
{
	m_Camera.SetFieldOfView(0.392699082f);
	m_Camera.SetPosition(0.0f, 0.0f, 1500.0f);
	m_Perspective.SetAspect(1.0f);
	m_Perspective.SetFieldOfView(0.392699082f);
	m_Perspective.SetNearClippingPlane(1.0f);
	m_Perspective.SetFarClippingPlane(10000.0f);
}

void MiniMapWidget::SetFocus(Ship * Focus)
{
	m_Focus = Focus;
	m_Camera.SetFocus(Focus);
}

void MiniMapWidget::Draw(void) const
{
	Widget::Draw();
	// mini map
	if((m_Focus != 0) && (m_Focus->GetCurrentSystem() != 0))
	{
		glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		// TODO: 0.0f is not the real value
		glViewport(static_cast< GLint >(GetGlobalPosition().m_V.m_A[0]), static_cast< GLint >(0.0f), static_cast< GLint >(GetSize().m_V.m_A[0]), static_cast< GLint >(220.0f));
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		m_Perspective.Draw();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		m_Camera.Draw();
		
		const std::list< Planet * > & Planets(m_Focus->GetCurrentSystem()->GetPlanets());
		const std::list< Ship * > & Ships(m_Focus->GetCurrentSystem()->GetShips());
		const std::list< Cargo * > & Cargos(m_Focus->GetCurrentSystem()->GetCargos());
		
		glBegin(GL_POINTS);
		glColor3f(0.8f, 0.8f, 0.8f);
		for(std::list< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
		{
			if(*PlanetIterator == m_Focus->GetTarget())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*PlanetIterator)->GetPosition().m_V.m_A[0], (*PlanetIterator)->GetPosition().m_V.m_A[1]);
			if(*PlanetIterator == m_Focus->GetTarget())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
		{
			if(*ShipIterator == m_Focus->GetTarget())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*ShipIterator)->GetPosition().m_V.m_A[0], (*ShipIterator)->GetPosition().m_V.m_A[1]);
			if(*ShipIterator == m_Focus->GetTarget())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Cargo * >::const_iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
		{
			if(*CargoIterator == m_Focus->GetTarget())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*CargoIterator)->GetPosition().m_V.m_A[0], (*CargoIterator)->GetPosition().m_V.m_A[1]);
			if(*CargoIterator == m_Focus->GetTarget())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		glEnd();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
	}
}
