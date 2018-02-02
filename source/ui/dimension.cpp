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
#include <iostream>

#include "../globals.h"
#include "dimension.h"
#include "event.h"
#include "user_interface.h"

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
	assert(_Owner != nullptr);
	switch(_Type)
	{
	case UI::Dimension::Type::Height:
		{
			UI::Event HeightChangedEvent;
			
			HeightChangedEvent.SetTarget(_Owner);
			g_UserInterface->DispatchHeightChangedEvent(HeightChangedEvent);
			
			break;
		}
	case UI::Dimension::Type::Left:
		{
			UI::Event LeftChangedEvent;
			
			LeftChangedEvent.SetTarget(_Owner);
			g_UserInterface->DispatchLeftChangedEvent(LeftChangedEvent);
			
			break;
		}
	case UI::Dimension::Type::Top:
		{
			UI::Event TopChangedEvent;
			
			TopChangedEvent.SetTarget(_Owner);
			g_UserInterface->DispatchTopChangedEvent(TopChangedEvent);
			
			break;
		}
	case UI::Dimension::Type::Width:
		{
			UI::Event WidthChangedEvent;
			
			WidthChangedEvent.SetTarget(_Owner);
			g_UserInterface->DispatchWidthChangedEvent(WidthChangedEvent);
			
			break;
		}
	}
	Expressions::Variable::InvalidateValue();
}
