/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#ifndef UI_VIEW_DISPLAY_H
#define UI_VIEW_DISPLAY_H

#include <string>

#include "widget.h"

namespace Graphics
{
	class RenderContext;
	class View;
}

namespace UI
{
	class ViewDisplay : public Widget
	{
	public:
		ViewDisplay(void);
		virtual ~ViewDisplay(void);
		// getters
		Graphics::View * GetView(void);
		// setters
		void SetView(Graphics::View * View);
		// draw
		virtual void Draw(Graphics::RenderContext * RenderContext) override;
	private:
		Graphics::View * _View;
	};
}

#endif
 
