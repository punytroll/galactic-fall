/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
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

#include <cassert>

#include "event.h"
#include "widget.h"

UI::Event::Event(void) :
	_CurrentTarget(nullptr),
	_Phase(UI::Event::Phase::Unspecified),
	_StopPropagation(false),
	_Target(nullptr)
{
}

UI::Event::~Event(void)
{
	SetCurrentTarget(nullptr);
	SetTarget(nullptr);
}

void UI::Event::SetCurrentTarget(UI::Widget * CurrentTarget)
{
	if(_CurrentTarget != nullptr)
	{
		_CurrentTarget = nullptr;
		assert(m_CurrentTargetDestroyingConnection.IsValid() == true);
		m_CurrentTargetDestroyingConnection.Disconnect();
	}
	assert(m_CurrentTargetDestroyingConnection.IsValid() == false);
	if(CurrentTarget != nullptr)
	{
		_CurrentTarget = CurrentTarget;
		m_CurrentTargetDestroyingConnection = _CurrentTarget->ConnectDestroyingCallback(std::bind(&UI::Event::SetCurrentTarget, this, nullptr));
		assert(m_CurrentTargetDestroyingConnection.IsValid() == true);
	}
}

void UI::Event::SetTarget(UI::Widget * Target)
{
	if(_Target != nullptr)
	{
		_Target = nullptr;
		assert(m_TargetDestroyingConnection.IsValid() == true);
		m_TargetDestroyingConnection.Disconnect();
	}
	assert(m_TargetDestroyingConnection.IsValid() == false);
	if(Target != nullptr)
	{
		_Target = Target;
		m_TargetDestroyingConnection = _Target->ConnectDestroyingCallback(std::bind(&UI::Event::SetTarget, this, nullptr));
		assert(m_TargetDestroyingConnection.IsValid() == true);
	}
}
