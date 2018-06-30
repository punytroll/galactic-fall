/**
 * galactic-fall
 * Copyright (C) 2007, 2014  Hagen Möbius
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

#include "widget.h"

namespace Graphics
{
	class ColorRGBO;
	class RenderContext;
}

namespace UI
{
	class Border : public Widget
	{
	public:
		Border(void);
		virtual ~Border(void);
		// getters
		const Graphics::ColorRGBO * GetColor(void) const;
		float GetLineWidth(void) const;
		// setters
		void SetColor(const Graphics::ColorRGBO & Color);
		void SetLineWidth(float LineWidth);
		// draw
		virtual void Draw(Graphics::RenderContext * RenderContext) override;
	private:
		Graphics::ColorRGBO * _Color;
		float _LineWidth;
	};

	inline const Graphics::ColorRGBO * Border::GetColor(void) const
	{
		return _Color;
	}

	inline float Border::GetLineWidth(void) const
	{
		return _LineWidth;
	}

	inline void Border::SetLineWidth(float LineWidth)
	{
		_LineWidth = LineWidth;
	}
}

#endif
