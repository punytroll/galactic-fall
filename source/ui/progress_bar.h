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

#ifndef UI_PROGRESS_BAR_H
#define UI_PROGRESS_BAR_H

#include "widget.h"

namespace Graphics
{
	class ColorRGBO;
}

namespace UI
{
	class Event;
	class Label;
	
	class ProgressBar : public UI::Widget
	{
	public:
		ProgressBar(UI::Widget * SupWidget);
		// setters
		void SetColor(const Graphics::ColorRGBO & Color);
		void SetFillLevel(float FillLevel);
		void SetText(const std::string & Text);
	private:
		void _OnSizeChanged(UI::Event & SizeChangedEvent);
		void _ResizeFill(void);
		// variables
		UI::Widget * _Fill;
		float _FillLevel;
		UI::Label * _Label;
	};
}

#endif
