/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#include <expressions/operators.h>

#include "dimension_operators.h"
#include "widget.h"

using namespace Expressions::Operators;

Expressions::Expression UI::bottom(UI::Widget * Widget)
{
	assert(Widget != nullptr);
	
	return Widget->_Top + Widget->_Height;
}

Expressions::Expression UI::height(UI::Widget * Widget)
{
	assert(Widget != nullptr);
	
	return Widget->_Height;
}

Expressions::Expression UI::left(UI::Widget * Widget)
{
	assert(Widget != nullptr);
	
	return Widget->_Left;
}

Expressions::Expression UI::right(UI::Widget * Widget)
{
	assert(Widget != nullptr);
	
	return Widget->_Left + Widget->_Width;
}

Expressions::Expression UI::top(UI::Widget * Widget)
{
	assert(Widget != nullptr);
	
	return Widget->_Top;
}

Expressions::Expression UI::width(UI::Widget * Widget)
{
	assert(Widget != nullptr);
	
	return Widget->_Width;
}
