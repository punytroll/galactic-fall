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

#ifndef UI_EVENT_H
#define UI_EVENT_H

#include "../connection.h"

namespace UI
{
	class Widget;
	
	class Event
	{
	public:
		enum class Phase
		{
			Unspecified,
			Capturing,
			Target,
			Bubbling
		};
		
		Event(void);
		virtual ~Event(void);
		// getters
		UI::Widget * GetCurrentTarget(void);
		UI::Event::Phase GetPhase(void);
		bool GetStopCallbacks(void);
		bool GetStopPropagation(void);
		UI::Widget * GetTarget(void);
		// setters
		void SetCurrentTarget(UI::Widget * CurrentTarget);
		void SetPhase(UI::Event::Phase Phase);
		void SetTarget(UI::Widget * Target);
		// modifiers
		void ResumeCallbacks(void);
		void StopCallbacks(void);
		void StopPropagation(void);
	private:
		UI::Widget * _CurrentTarget;
		Connection _CurrentTargetDestroyingConnection;
		UI::Event::Phase _Phase;
		bool _StopCallbacks;
		bool _StopPropagation;
		UI::Widget * _Target;
		Connection _TargetDestroyingConnection;
	};
}

inline UI::Widget * UI::Event::GetCurrentTarget(void)
{
	return _CurrentTarget;
}

inline UI::Event::Phase UI::Event::GetPhase(void)
{
	return _Phase;
}

inline bool UI::Event::GetStopCallbacks(void)
{
	return _StopCallbacks;
}

inline bool UI::Event::GetStopPropagation(void)
{
	return _StopPropagation;
}

inline UI::Widget * UI::Event::GetTarget(void)
{
	return _Target;
}

inline void UI::Event::SetPhase(UI::Event::Phase Phase)
{
	_Phase = Phase;
}

inline void UI::Event::ResumeCallbacks(void)
{
	_StopCallbacks = false;
}

inline void UI::Event::StopCallbacks(void)
{
	_StopCallbacks = true;
}

inline void UI::Event::StopPropagation(void)
{
	_StopPropagation = true;
}

#endif
