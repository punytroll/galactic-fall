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

#ifndef UI_SCROLL_BAR_H
#define UI_SCROLL_BAR_H

#include "widget.h"

namespace UI
{
	class Button;
	class Event;
	
	class ScrollBar : public UI::Widget
	{
	public:
		enum class Alignment
		{
			UNDEFINED,
			HORIZONTAL,
			VERTICAL
		};
		
		ScrollBar(ScrollBar::Alignment Alignment);
		virtual ~ScrollBar(void);
		// events
		Connection ConnectScrollPositionChangedCallback(std::function< void (void) > Callback);
		void DisconnectScrollPositionChangedCallback(Connection & Connection);
		// getters
		UI::ScrollBar::Alignment GetAlignment(void) const;
		float GetCurrentPosition(void) const;
		float GetMinimumPosition(void) const;
		float GetMaximumPosition(void) const;
		float GetStepSize(void) const;
		// setters
		void SetAlignment(UI::ScrollBar::Alignment Alignment);
		void SetCurrentPosition(float CurrentPosition);
		void SetMinimumPosition(float MinimumPosition);
		void SetMaximumPosition(float MaximumPosition);
		void SetStepSize(float StepSize);
		// modifiers
		void StepLess(void);
		void StepMore(void);
	private:
		// callbacks
		void _OnLessClicked(void);
		void _OnMoreClicked(void);
		void _OnSizeChanged(UI::Event & SizeChangedEvent);
		void _OnTrackerMouseButton(UI::MouseButtonEvent & MouseButtonEvent);
		void _OnTrackerMouseEnter(void);
		void _OnTrackerMouseLeave(void);
		void _OnTrackerMouseMove(UI::MouseMoveEvent & MouseMoveEvent);
		// helper functions and actions
		void _AdjustTrackerPosition(void);
		float _GetAvailableRange(void);
		// member variables
		Vector2f _GrabPosition;
		UI::Button * _LessButton;
		UI::Button * _MoreButton;
		UI::Widget * _Tracker;
		UI::ScrollBar::Alignment _Alignment;
		float _CurrentPosition;
		float _MaximumPosition;
		float _MinimumPosition;
		float _StepSize;
		// events
		::Event< void > _ScrollPositionChangedEvent;
	};

	inline ScrollBar::Alignment ScrollBar::GetAlignment(void) const
	{
		return _Alignment;
	}

	inline float ScrollBar::GetCurrentPosition(void) const
	{
		return _CurrentPosition;
	}

	inline float ScrollBar::GetMinimumPosition(void) const
	{
		return _MinimumPosition;
	}

	inline float ScrollBar::GetMaximumPosition(void) const
	{
		return _MaximumPosition;
	}

	inline float ScrollBar::GetStepSize(void) const
	{
		return _StepSize;
	}

	inline void ScrollBar::SetStepSize(float StepSize)
	{
		_StepSize = StepSize;
	}

	inline void ScrollBar::StepLess(void)
	{
		SetCurrentPosition(GetCurrentPosition() - GetStepSize());
	}

	inline void ScrollBar::StepMore(void)
	{
		SetCurrentPosition(GetCurrentPosition() + GetStepSize());
	}
}

#endif
