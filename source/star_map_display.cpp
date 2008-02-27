/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include <map>

#include <GL/gl.h>

#include "character.h"
#include "draw_text.h"
#include "galaxy.h"
#include "globals.h"
#include "map_knowledge.h"
#include "object_aspect_name.h"
#include "star_map_display.h"
#include "system.h"
#include "user_interface.h"

StarMapDisplay::StarMapDisplay(Widget * SupWidget, System * System, Character * Character) :
	Widget(SupWidget),
	m_Character(Character),
	m_System(System),
	m_SelectedSystem(0),
	m_Scale(5.0f),
	m_OffsetPosition(true)
{
	AddMouseButtonListener(this);
	AddMouseMotionListener(this);
}

void StarMapDisplay::Draw(void) const
{
	Widget::Draw();
	
	Vector2f Middle(GetSize() / 2);
	
	Middle += m_OffsetPosition;
	
	float SystemSize(5.0f);
	
	glPushAttrib(GL_ENABLE_BIT);
	glPushMatrix();
	glTranslatef(Middle.m_V.m_A[0], Middle.m_V.m_A[1], 0.0f);
	glScalef(1.0f, -1.0f, 1.0f);
	
	const std::set< System * > & UnexploredSystems(m_Character->GetMapKnowledge()->GetUnexploredSystems());
	
	for(std::set< System * >::const_iterator UnexploredSystemIterator = UnexploredSystems.begin(); UnexploredSystemIterator != UnexploredSystems.end(); ++UnexploredSystemIterator)
	{
		Vector3f Position((*UnexploredSystemIterator)->GetPosition() - m_System->GetPosition());
		
		Position *= m_Scale;
		glPushMatrix();
		glTranslatef(Position.m_V.m_A[0], Position.m_V.m_A[1], 0.0f);
		if(*UnexploredSystemIterator == m_SelectedSystem)
		{
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		else if(*UnexploredSystemIterator == m_System)
		{
			glColor3f(0.8f, 0.8f, 0.0f);
		}
		else
		{
			glColor3f(0.5f, 0.5f, 0.5f);
		}
		glBegin(GL_LINE_LOOP);
		glVertex2f(SystemSize, 0.0f);
		glVertex2f(SystemSize * 0.866f, SystemSize * 0.5f);
		glVertex2f(SystemSize * 0.5f, SystemSize * 0.866f);
		glVertex2f(0.0f, SystemSize);
		glVertex2f(-SystemSize * 0.5f, SystemSize * 0.866f);
		glVertex2f(-SystemSize * 0.866f, SystemSize * 0.5f);
		glVertex2f(-SystemSize, 0.0f);
		glVertex2f(-SystemSize * 0.866f, -SystemSize * 0.5f);
		glVertex2f(-SystemSize * 0.5f, -SystemSize * 0.866f);
		glVertex2f(0.0f, -SystemSize);
		glVertex2f(SystemSize * 0.5f, -SystemSize * 0.866f);
		glVertex2f(SystemSize * 0.866f, -SystemSize * 0.5f);
		glEnd();
		glPopMatrix();
	}
	
	const std::set< System * > & ExploredSystems(m_Character->GetMapKnowledge()->GetExploredSystems());
	
	for(std::set< System * >::const_iterator ExploredSystemIterator = ExploredSystems.begin(); ExploredSystemIterator != ExploredSystems.end(); ++ExploredSystemIterator)
	{
		Vector3f Position((*ExploredSystemIterator)->GetPosition() - m_System->GetPosition());
		
		Position *= m_Scale;
		glPushMatrix();
		glTranslatef(Position.m_V.m_A[0], Position.m_V.m_A[1], 0.0f);
		
		const std::list< System * > & LinkedSystems((*ExploredSystemIterator)->GetLinkedSystems());
		
		for(std::list< System * >::const_iterator LinkedSystemIterator = LinkedSystems.begin(); LinkedSystemIterator != LinkedSystems.end(); ++LinkedSystemIterator)
		{
			if(UnexploredSystems.find(*LinkedSystemIterator) == UnexploredSystems.end())
			{
				// system already explored so make it blue
				glColor3f(0.0f, 0.0f, 0.8f);
			}
			else
			{
				// system still unexplored so make it grey
				glColor3f(0.5f, 0.5f, 0.5f);
			}
			glBegin(GL_LINES);
			glVertex2f(0.0f, 0.0f);
			glVertex2fv((((*LinkedSystemIterator)->GetPosition() - (*ExploredSystemIterator)->GetPosition()) * m_Scale).m_V.m_A);
			glEnd();
		}
		if(*ExploredSystemIterator == m_SelectedSystem)
		{
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		else if(*ExploredSystemIterator == m_System)
		{
			glColor3f(0.8f, 0.8f, 0.0f);
		}
		else
		{
			glColor3f(0.0f, 0.0f, 0.8f);
		}
		glBegin(GL_LINE_LOOP);
		glVertex2f(SystemSize, 0.0f);
		glVertex2f(SystemSize * 0.866f, SystemSize * 0.5f);
		glVertex2f(SystemSize * 0.5f, SystemSize * 0.866f);
		glVertex2f(0.0f, SystemSize);
		glVertex2f(-SystemSize * 0.5f, SystemSize * 0.866f);
		glVertex2f(-SystemSize * 0.866f, SystemSize * 0.5f);
		glVertex2f(-SystemSize, 0.0f);
		glVertex2f(-SystemSize * 0.866f, -SystemSize * 0.5f);
		glVertex2f(-SystemSize * 0.5f, -SystemSize * 0.866f);
		glVertex2f(0.0f, -SystemSize);
		glVertex2f(SystemSize * 0.5f, -SystemSize * 0.866f);
		glVertex2f(SystemSize * 0.866f, -SystemSize * 0.5f);
		glEnd();
		glPushMatrix();
		glScalef(1.0f, -1.0f, 1.0f);
		glTranslatef(0.0f, 12.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		DrawText((*ExploredSystemIterator)->GetAspectName()->GetName());
		glPopMatrix();
		glPopMatrix();
	}
	glPopMatrix();
	glPopAttrib();
}

bool StarMapDisplay::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if(EventSource == this)
	{
		if((Button == 4 /* WHEEL_UP */) && (State == EV_DOWN))
		{
			m_Scale *= 1.1f;
			
			return true;
		}
		else if((Button == 5 /* WHEEL_DOWN */) && (State == EV_DOWN))
		{
			m_Scale *= 0.9f;
			
			return true;
		}
		else if((Button == 1 /* LEFT */) && (State == EV_UP))
		{
			X -= GetSize().m_V.m_A[0] / 2;
			Y -= GetSize().m_V.m_A[1] / 2;
			
			const std::map< std::string, System * > & Systems(g_Galaxy->GetSystems());
			
			for(std::map< std::string, System * >::const_iterator SystemIterator = Systems.begin(); SystemIterator != Systems.end(); ++SystemIterator)
			{
				Vector3f Position(SystemIterator->second->GetPosition() - m_System->GetPosition());
				
				Position *= m_Scale;
				Position.m_V.m_A[0] -= X - m_OffsetPosition[0];
				Position.m_V.m_A[1] += Y - m_OffsetPosition[1];
				if(Position.SquaredLength() < 40.0f)
				{
					m_SelectedSystem = SystemIterator->second;
					
					break;
				}
			}
			
			return true;
		}
		else if(Button == 2 /* MIDDLE */)
		{
			if(State == EV_DOWN)
			{
				m_GrabPosition.Set(X, Y);
				g_UserInterface->SetCaptureWidget(this);
			}
			else
			{
				g_UserInterface->ReleaseCaptureWidget();
			}
			
			return true;
		}
	}
	
	return false;
}

void StarMapDisplay::OnMouseMotion(Widget * EventSource, float X, float Y)
{
	if((EventSource == this) && (g_UserInterface->GetCaptureWidget() == this))
	{
		m_OffsetPosition += (Vector2f(X, Y) - m_GrabPosition);
		m_GrabPosition = Vector2f(X, Y);
	}
}
