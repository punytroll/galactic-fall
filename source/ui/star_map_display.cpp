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

#include "../character.h"
#include "../draw_text.h"
#include "../galaxy.h"
#include "../globals.h"
#include "../graphics/gl.h"
#include "../map_knowledge.h"
#include "../object_aspect_name.h"
#include "../object_aspect_position.h"
#include "../system.h"
#include "mouse_button_event.h"
#include "mouse_move_event.h"
#include "star_map_display.h"
#include "user_interface.h"

UI::StarMapDisplay::StarMapDisplay(Widget * SupWidget, System * System, Character * Character) :
	Widget(SupWidget),
	m_Character(Character),
	m_System(System),
	m_SelectedSystem(0),
	m_Scale(5.0f),
	m_OffsetPosition(true)
{
	ConnectMouseButtonCallback(std::bind(&UI::StarMapDisplay::_OnMouseButton, this, std::placeholders::_1));
	ConnectMouseMoveCallback(std::bind(&UI::StarMapDisplay::_OnMouseMove, this, std::placeholders::_1));
}

void UI::StarMapDisplay::Draw(Graphics::RenderContext * RenderContext)
{
	Widget::Draw(RenderContext);
	
	Vector2f Middle(GetSize() / 2);
	
	Middle += m_OffsetPosition;
	
	float SystemSize(5.0f);
	
	GLPushAttrib(GL_ENABLE_BIT);
	GLPushMatrix();
	GLTranslatef(Middle[0], Middle[1], 0.0f);
	GLScalef(1.0f, -1.0f, 1.0f);
	
	const std::set< System * > & UnexploredSystems(m_Character->GetMapKnowledge()->GetUnexploredSystems());
	
	for(std::set< System * >::const_iterator UnexploredSystemIterator = UnexploredSystems.begin(); UnexploredSystemIterator != UnexploredSystems.end(); ++UnexploredSystemIterator)
	{
		Vector3f Position((*UnexploredSystemIterator)->GetAspectPosition()->GetPosition() - m_System->GetAspectPosition()->GetPosition());
		
		Position *= m_Scale;
		GLPushMatrix();
		GLTranslatef(Position[0], Position[1], 0.0f);
		if(*UnexploredSystemIterator == m_SelectedSystem)
		{
			GLColor3f(0.0f, 1.0f, 0.0f);
		}
		else if(*UnexploredSystemIterator == m_System)
		{
			GLColor3f(0.8f, 0.8f, 0.0f);
		}
		else
		{
			GLColor3f(0.5f, 0.5f, 0.5f);
		}
		GLBegin(GL_LINE_LOOP);
		GLVertex2f(SystemSize, 0.0f);
		GLVertex2f(SystemSize * 0.866f, SystemSize * 0.5f);
		GLVertex2f(SystemSize * 0.5f, SystemSize * 0.866f);
		GLVertex2f(0.0f, SystemSize);
		GLVertex2f(-SystemSize * 0.5f, SystemSize * 0.866f);
		GLVertex2f(-SystemSize * 0.866f, SystemSize * 0.5f);
		GLVertex2f(-SystemSize, 0.0f);
		GLVertex2f(-SystemSize * 0.866f, -SystemSize * 0.5f);
		GLVertex2f(-SystemSize * 0.5f, -SystemSize * 0.866f);
		GLVertex2f(0.0f, -SystemSize);
		GLVertex2f(SystemSize * 0.5f, -SystemSize * 0.866f);
		GLVertex2f(SystemSize * 0.866f, -SystemSize * 0.5f);
		GLEnd();
		GLPopMatrix();
	}
	
	const std::set< System * > & ExploredSystems(m_Character->GetMapKnowledge()->GetExploredSystems());
	
	for(std::set< System * >::const_iterator ExploredSystemIterator = ExploredSystems.begin(); ExploredSystemIterator != ExploredSystems.end(); ++ExploredSystemIterator)
	{
		Vector3f Position((*ExploredSystemIterator)->GetAspectPosition()->GetPosition() - m_System->GetAspectPosition()->GetPosition());
		
		Position *= m_Scale;
		GLPushMatrix();
		GLTranslatef(Position[0], Position[1], 0.0f);
		
		const std::list< System * > & LinkedSystems((*ExploredSystemIterator)->GetLinkedSystems());
		
		for(std::list< System * >::const_iterator LinkedSystemIterator = LinkedSystems.begin(); LinkedSystemIterator != LinkedSystems.end(); ++LinkedSystemIterator)
		{
			if(UnexploredSystems.find(*LinkedSystemIterator) == UnexploredSystems.end())
			{
				// system already explored so make it blue
				GLColor3f(0.0f, 0.0f, 0.8f);
			}
			else
			{
				// system still unexplored so make it grey
				GLColor3f(0.5f, 0.5f, 0.5f);
			}
			GLBegin(GL_LINES);
			GLVertex2f(0.0f, 0.0f);
			
			Vector3f To(((*LinkedSystemIterator)->GetAspectPosition()->GetPosition() - (*ExploredSystemIterator)->GetAspectPosition()->GetPosition()).Scale(m_Scale));
			
			GLVertex2f(To[0], To[1]);
			GLEnd();
		}
		if(*ExploredSystemIterator == m_SelectedSystem)
		{
			GLColor3f(0.0f, 1.0f, 0.0f);
		}
		else if(*ExploredSystemIterator == m_System)
		{
			GLColor3f(0.8f, 0.8f, 0.0f);
		}
		else
		{
			GLColor3f(0.0f, 0.0f, 0.8f);
		}
		GLBegin(GL_LINE_LOOP);
		GLVertex2f(SystemSize, 0.0f);
		GLVertex2f(SystemSize * 0.866f, SystemSize * 0.5f);
		GLVertex2f(SystemSize * 0.5f, SystemSize * 0.866f);
		GLVertex2f(0.0f, SystemSize);
		GLVertex2f(-SystemSize * 0.5f, SystemSize * 0.866f);
		GLVertex2f(-SystemSize * 0.866f, SystemSize * 0.5f);
		GLVertex2f(-SystemSize, 0.0f);
		GLVertex2f(-SystemSize * 0.866f, -SystemSize * 0.5f);
		GLVertex2f(-SystemSize * 0.5f, -SystemSize * 0.866f);
		GLVertex2f(0.0f, -SystemSize);
		GLVertex2f(SystemSize * 0.5f, -SystemSize * 0.866f);
		GLVertex2f(SystemSize * 0.866f, -SystemSize * 0.5f);
		GLEnd();
		GLPushMatrix();
		GLScalef(1.0f, -1.0f, 1.0f);
		GLTranslatef(0.0f, 12.0f, 0.0f);
		GLColor3f(1.0f, 1.0f, 1.0f);
		DrawText((*ExploredSystemIterator)->GetAspectName()->GetName());
		GLPopMatrix();
		GLPopMatrix();
	}
	GLPopMatrix();
	GLPopAttrib();
}

void UI::StarMapDisplay::_OnMouseButton(UI::MouseButtonEvent & MouseButtonEvent)
{
	if(MouseButtonEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::WheelUp) && (MouseButtonEvent.IsDown() == true))
		{
			m_Scale *= 1.1f;
			MouseButtonEvent.StopPropagation();
		}
		else if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::WheelDown) && (MouseButtonEvent.IsDown() == true))
		{
			m_Scale *= 0.9f;
			MouseButtonEvent.StopPropagation();
		}
		else if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
		{
			Vector2f NormalizedPosition(MouseButtonEvent.GetPosition() - GetSize() / 2.0f);
			const std::map< std::string, System * > & Systems(g_Galaxy->GetSystems());
			
			for(std::map< std::string, System * >::const_iterator SystemIterator = Systems.begin(); SystemIterator != Systems.end(); ++SystemIterator)
			{
				Vector3f Position(SystemIterator->second->GetAspectPosition()->GetPosition() - m_System->GetAspectPosition()->GetPosition());
				
				Position *= m_Scale;
				Position[0] -= NormalizedPosition[0] - m_OffsetPosition[0];
				Position[1] += NormalizedPosition[1] - m_OffsetPosition[1];
				if(Position.SquaredLength() < 40.0f)
				{
					m_SelectedSystem = SystemIterator->second;
					
					break;
				}
			}
		}
		else if(MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Middle)
		{
			if(MouseButtonEvent.IsDown() == true)
			{
				m_GrabPosition = MouseButtonEvent.GetPosition();
				g_UserInterface->SetCaptureWidget(this);
			}
			else if(MouseButtonEvent.IsUp() == true)
			{
				g_UserInterface->ReleaseCaptureWidget();
			}
		}
	}
}

void UI::StarMapDisplay::_OnMouseMove(UI::MouseMoveEvent & MouseMoveEvent)
{
	if((MouseMoveEvent.GetPhase() == UI::Event::Phase::Target) && (g_UserInterface->GetCaptureWidget() == this))
	{
		m_OffsetPosition += (MouseMoveEvent.GetPosition() - m_GrabPosition);
		m_GrabPosition = MouseMoveEvent.GetPosition();
	}
}
