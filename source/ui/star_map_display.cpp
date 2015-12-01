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
#include "../galaxy.h"
#include "../globals.h"
#include "../graphics/color_rgbo.h"
#include "../graphics/drawing.h"
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
	_Character(Character),
	_OffsetPosition(true),
	_Scale(5.0f),
	_SelectedSystem(nullptr),
	_System(System)
{
	ConnectMouseButtonCallback(std::bind(&UI::StarMapDisplay::_OnMouseButton, this, std::placeholders::_1));
	ConnectMouseMoveCallback(std::bind(&UI::StarMapDisplay::_OnMouseMove, this, std::placeholders::_1));
}

void UI::StarMapDisplay::Draw(Graphics::RenderContext * RenderContext)
{
	Widget::Draw(RenderContext);
	
	Vector2f Middle(GetSize() / 2);
	
	Middle += GetGlobalPosition();
	Middle += _OffsetPosition;
	
	auto SystemSize{5.0f};
	
	GLPushAttrib(GL_ENABLE_BIT);
	GLPushMatrix();
	GLTranslatef(Middle[0], Middle[1], 0.0f);
	GLScalef(1.0f, -1.0f, 1.0f);
	
	auto & UnexploredSystems{_Character->GetMapKnowledge()->GetUnexploredSystems()};
	
	for(auto UnexploredSystem : UnexploredSystems)
	{
		auto Position{UnexploredSystem->GetAspectPosition()->GetPosition() - _System->GetAspectPosition()->GetPosition()};
		
		Position *= _Scale;
		GLPushMatrix();
		GLTranslatef(Position[0], Position[1], 0.0f);
		if(UnexploredSystem == _SelectedSystem)
		{
			GLColor3f(0.0f, 1.0f, 0.0f);
		}
		else if(UnexploredSystem == _System)
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
	
	auto & ExploredSystems{_Character->GetMapKnowledge()->GetExploredSystems()};
	
	for(auto ExploredSystem : ExploredSystems)
	{
		assert(ExploredSystem != nullptr);
		assert(ExploredSystem->GetAspectPosition() != nullptr);
		
		auto Position{ExploredSystem->GetAspectPosition()->GetPosition() - _System->GetAspectPosition()->GetPosition()};
		
		Position *= _Scale;
		GLPushMatrix();
		GLTranslatef(Position[0], Position[1], 0.0f);
		
		auto & LinkedSystems{ExploredSystem->GetLinkedSystems()};
		
		for(auto LinkedSystem : LinkedSystems)
		{
			assert(LinkedSystem != nullptr);
			if(UnexploredSystems.find(LinkedSystem) == UnexploredSystems.end())
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
			assert(LinkedSystem->GetAspectPosition() != nullptr);
			
			auto To{(LinkedSystem->GetAspectPosition()->GetPosition() - ExploredSystem->GetAspectPosition()->GetPosition()).Scale(_Scale)};
			
			GLVertex2f(To[0], To[1]);
			GLEnd();
		}
		if(ExploredSystem == _SelectedSystem)
		{
			GLColor3f(0.0f, 1.0f, 0.0f);
		}
		else if(ExploredSystem == _System)
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
		assert(ExploredSystem->GetAspectName() != nullptr);
		Graphics::Drawing::DrawText(RenderContext, Middle + Vector2f(Position[0], 1.2f * SystemSize - Position[1]), ExploredSystem->GetAspectName()->GetName(), Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 1.0f));
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
			_Scale *= 1.1f;
			MouseButtonEvent.StopPropagation();
		}
		else if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::WheelDown) && (MouseButtonEvent.IsDown() == true))
		{
			_Scale *= 0.9f;
			MouseButtonEvent.StopPropagation();
		}
		else if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
		{
			auto NormalizedPosition{MouseButtonEvent.GetPosition() - GetSize() / 2.0f};
			
			for(auto SystemPair : g_Galaxy->GetSystems())
			{
				assert(SystemPair.second != nullptr);
				assert(SystemPair.second->GetAspectPosition() != nullptr);
				
				auto Position{SystemPair.second->GetAspectPosition()->GetPosition() - _System->GetAspectPosition()->GetPosition()};
				
				Position *= _Scale;
				Position[0] -= NormalizedPosition[0] - _OffsetPosition[0];
				Position[1] += NormalizedPosition[1] - _OffsetPosition[1];
				if(Position.SquaredLength() < 40.0f)
				{
					_SelectedSystem = SystemPair.second;
					
					break;
				}
			}
		}
		else if(MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Middle)
		{
			if(MouseButtonEvent.IsDown() == true)
			{
				_GrabPosition = MouseButtonEvent.GetPosition();
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
		_OffsetPosition += (MouseMoveEvent.GetPosition() - _GrabPosition);
		_GrabPosition = MouseMoveEvent.GetPosition();
	}
}
