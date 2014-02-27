/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include <assert.h>

#include "color.h"
#include "ship_class.h"
#include "slot.h"
#include "visualization_prototype.h"

ShipClass::ShipClass(const std::string & Identifier) :
	m_ExhaustOffset(true),
	m_Identifier(Identifier),
	m_MaximumAvailableSpace(0),
	m_VisualizationPrototype(0)
{
}

ShipClass::~ShipClass(void)
{
	while(m_Slots.empty() == false)
	{
		delete m_Slots.begin()->second;
		m_Slots.erase(m_Slots.begin());
	}
	delete m_VisualizationPrototype;
	m_VisualizationPrototype = 0;
}

bool ShipClass::AddSlot(const std::string & SlotIdentifier, Slot * Slot)
{
	if(m_Slots.find(SlotIdentifier) == m_Slots.end())
	{
		m_Slots[SlotIdentifier] = Slot;
		
		return true;
	}
	else
	{
		return false;
	}
}

void ShipClass::AddVisualizationPrototype(void)
{
	assert(m_VisualizationPrototype == 0);
	m_VisualizationPrototype = new VisualizationPrototype();
}
