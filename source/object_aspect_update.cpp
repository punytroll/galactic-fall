/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#include "object_aspect_update.h"

ObjectAspectUpdate::ObjectAspectUpdate(void) :
	m_Callback(0)
{
}

ObjectAspectUpdate::~ObjectAspectUpdate(void)
{
	delete m_Callback;
	m_Callback = 0;
}

bool ObjectAspectUpdate::Update(float Seconds)
{
	if(m_Callback != 0)
	{
		return (*m_Callback)(Seconds);
	}
	else
	{
		return true;
	}
}

void ObjectAspectUpdate::SetCallback(Callback1< bool, float > * Callback)
{
	assert(m_Callback == 0);
	m_Callback = Callback;
}
