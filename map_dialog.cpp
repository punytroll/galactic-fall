/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include "button.h"
#include "character.h"
#include "color.h"
#include "draw_text.h"
#include "galaxy.h"
#include "globals.h"
#include "label.h"
#include "map_dialog.h"
#include "map_knowledge.h"
#include "system.h"
#include "user_interface.h"

MapDialog::MapDialog(Widget * SupWidget, System * System, Character * Character) :
	WWindow(SupWidget, "Map: " + System->GetName()),
	m_System(System),
	m_Character(Character),
	m_Scale(5.0f),
	m_SelectedSystem(0)
{
	SetPosition(Vector2f(70.0f, 400.0f));
	SetSize(Vector2f(500.0f, 530.0f));
	AddKeyListener(this);
	AddMouseButtonListener(this);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(Vector2f(390.0f, 500.0f));
	m_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	m_OKButton->AddClickedListener(this);
	m_OKButtonLabel = new Label(m_OKButton, "OK");
	m_OKButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_OKButtonLabel->SetSize(m_OKButton->GetSize());
	m_OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
}

void MapDialog::Draw(void) const
{
	WWindow::Draw();
	
	Vector2f Middle(GetSize() / 2);
	
	Middle.m_V.m_A[1] += 15.0f;
	
	float SystemSize(5.0f);
	
	glPushMatrix();
	glTranslatef(Middle.m_V.m_A[0], Middle.m_V.m_A[1], 0.0f);
	glScalef(1.0f, -1.0f, 1.0f);
	
	const std::set< System * > & UnexploredSystems(m_Character->GetMapKnowledge()->GetUnexploredSystems());
	
	for(std::set< System * >::const_iterator UnexploredSystemIterator = UnexploredSystems.begin(); UnexploredSystemIterator != UnexploredSystems.end(); ++UnexploredSystemIterator)
	{
		Vector2f Position((*UnexploredSystemIterator)->GetPosition() - m_System->GetPosition());
		
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
		Vector2f Position((*ExploredSystemIterator)->GetPosition() - m_System->GetPosition());
		
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
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, -12.0f - SystemSize * 2.0f, 0.0f);
		DrawTextWithoutTranslation((*ExploredSystemIterator)->GetName());
		glPopMatrix();
	}
	glPopMatrix();
}

bool MapDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		Destroy();
		
		return true;
	}
	
	return false;
}

bool MapDialog::OnKey(Widget * EventSource, int KeyCode, int State)
{
	if(((KeyCode == 9 /* ESCAPE */) || (KeyCode == 36 /* RETURN */) || (KeyCode == 58 /* M */)) && (State == EV_DOWN))
	{
		Destroy();
	}
	// eat all other input
	return true;
}

bool MapDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if(WWindow::OnMouseButton(EventSource, Button, State, X, Y) == true)
	{
		return true;
	}
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
			Y -= 15.0f + GetSize().m_V.m_A[1] / 2;
			
			const std::map< std::string, System * > & Systems(g_Galaxy->GetSystems());
			
			for(std::map< std::string, System * >::const_iterator SystemIterator = Systems.begin(); SystemIterator != Systems.end(); ++SystemIterator)
			{
				Vector2f Position(SystemIterator->second->GetPosition() - m_System->GetPosition());
				
				Position *= m_Scale;
				Position.m_V.m_A[0] -= X;
				Position.m_V.m_A[1] += Y;
				if(Position.SquaredLength() < 40.0f)
				{
					m_SelectedSystem = SystemIterator->second;
					
					break;
				}
			}
			
			return true;
		}
	}
	
	return false;
}
