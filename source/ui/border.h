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

#ifndef UI_BORDER_H
#define UI_BORDER_H

#include "../color.h"
#include "widget.h"

namespace UI
{
	class Border : public Widget
	{
	public:
		Border(Widget * SupWidget);
		virtual ~Border(void);
		// getters
		Color & GetForegroundColor(void);
		const Color & GetForegroundColor(void) const;
		float GetWidth(void) const;
		// setters
		void SetForegroundColor(const Color & ForegroundColor);
		void SetWidth(float Width);
		// draw
		virtual void Draw(void) const;
	private:
		Color m_ForegroundColor;
		float m_Width;
	};

	inline Color & Border::GetForegroundColor(void)
	{
		return m_ForegroundColor;
	}

	inline const Color & Border::GetForegroundColor(void) const
	{
		return m_ForegroundColor;
	}

	inline float Border::GetWidth(void) const
	{
		return m_Width;
	}

	inline void Border::SetForegroundColor(const Color & ForegroundColor)
	{
		m_ForegroundColor = ForegroundColor;
	}

	inline void Border::SetWidth(float Width)
	{
		m_Width = Width;
	}
}

#endif
