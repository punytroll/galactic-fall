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
	
	class ScrollBar : public UI::Widget
	{
	public:
		enum class Alignment
		{
			UNDEFINED,
			HORIZONTAL,
			VERTICAL
		};
		
		ScrollBar(UI::Widget * SupWidget, ScrollBar::Alignment Alignment);
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
		void OnLessClicked(void);
		void OnMoreClicked(void);
		void OnSizeChanged(void);
		bool OnTrackerMouseButton(int Button, int State, float X, float Y);
		void OnTrackerMouseEnter(void);
		void OnTrackerMouseLeave(void);
		void OnTrackerMouseMoved(float X, float Y);
		// helper functions and actions
		void AdjustTrackerPosition(void);
		// member variables
		Vector2f m_GrabPosition;
		UI::Button * m_LessButton;
		UI::Button * m_MoreButton;
		UI::Widget * m_Tracker;
		UI::ScrollBar::Alignment m_Alignment;
		float m_CurrentPosition;
		float m_MinimumPosition;
		float m_MaximumPosition;
		float m_StepSize;
		// events
		::Event< void > _ScrollPositionChangedEvent;
	};

	inline ScrollBar::Alignment ScrollBar::GetAlignment(void) const
	{
		return m_Alignment;
	}

	inline float ScrollBar::GetCurrentPosition(void) const
	{
		return m_CurrentPosition;
	}

	inline float ScrollBar::GetMinimumPosition(void) const
	{
		return m_MinimumPosition;
	}

	inline float ScrollBar::GetMaximumPosition(void) const
	{
		return m_MaximumPosition;
	}

	inline float ScrollBar::GetStepSize(void) const
	{
		return m_StepSize;
	}

	inline void ScrollBar::SetStepSize(float StepSize)
	{
		m_StepSize = StepSize;
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
