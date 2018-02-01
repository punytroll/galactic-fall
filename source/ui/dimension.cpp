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

#include <iostream>

#include "dimension.h"

UI::Dimension::Dimension(UI::Widget * Owner, UI::Dimension::Type Type) :
	_Owner(Owner),
	_Type(Type)
{
}

UI::Dimension::~Dimension(void)
{
}

void UI::Dimension::operator=(Expressions::Expression && Expression)
{
	SetExpression(std::move(Expression));
}

void UI::Dimension::InvalidateValue(void)
{
	Expressions::Variable::InvalidateValue();
	switch(_Type)
	{
	case UI::Dimension::Type::Height:
		{
			std::cout << "Height changed: " << GetValue() << std::endl;
			
			break;
		}
	case UI::Dimension::Type::Left:
		{
			std::cout << "Left changed: " << GetValue() << std::endl;
			
			break;
		}
	case UI::Dimension::Type::Top:
		{
			std::cout << "Top changed: " << GetValue() << std::endl;
			
			break;
		}
	case UI::Dimension::Type::Width:
		{
			std::cout << "Width changed: " << GetValue() << std::endl;
			
			break;
		}
	}
}
