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

#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H

#include "clicked_listener.h"
#include "widget.h"

class Button;
class ScrollPositionChangedListener;

class ScrollBar : public ClickedListener, public Widget
{
public:
	enum Alignment
	{
		HORIZONTAL,
		VERTICAL
	};
	
	ScrollBar(Widget * SupWidget, ScrollBar::Alignment Alignment);
	virtual ~ScrollBar(void);
	void AddScrollPositionChangedListener(ScrollPositionChangedListener * ScrollPositionChangedListener);
	// getters
	float GetCurrentPosition(void) const;
	float GetMinimumPosition(void) const;
	float GetMaximumPosition(void) const;
	float GetStepSize(void) const;
	// setters
	void SetAlignment(ScrollBar::Alignment Alignment);
	void SetCurrentPosition(float CurrentPosition);
	void SetMinimumPosition(float MinimumPosition);
	void SetMaximumPosition(float MaximumPosition);
	void SetStepSize(float StepSize);
	// modifiers
	void StepLess(void);
	void StepMore(void);
protected:
	virtual bool OnClicked(Widget * EventSource);
private:
	std::list< ScrollPositionChangedListener * > m_ScrollPositionChangedListeners;
	Button * m_LessButton;
	Button * m_MoreButton;
	float m_CurrentPosition;
	float m_MinimumPosition;
	float m_MaximumPosition;
	float m_StepSize;
};

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

inline void ScrollBar::SetMinimumPosition(float MinimumPosition)
{
	m_MinimumPosition = MinimumPosition;
}

inline void ScrollBar::SetMaximumPosition(float MaximumPosition)
{
	m_MaximumPosition = MaximumPosition;
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

#endif
