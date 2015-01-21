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

#include <iostream>

#include "globals.h"
#include "message.h"
#include "object.h"
#include "output_observer.h"
#include "planet.h"
#include "ui/planet_window.h"
#include "ui/user_interface.h"

OutputObserver::OutputObserver(void) :
	_PlanetWindow(nullptr)
{
}

void OutputObserver::HandleMessage(Message * Message)
{
	if(Message->GetTypeIdentifier() == "landed")
	{
		assert(_PlanetWindow == nullptr);
		assert(Message->GetSender()->GetTypeIdentifier() == "planet");
		_PlanetWindow = new UI::PlanetWindow(g_UserInterface->GetRootWidget(), dynamic_cast< Planet * >(Message->GetSender().Get()), GetObservedCharacter());
		_PlanetWindow->GrabKeyFocus();
	}
	else if(Message->GetTypeIdentifier() == "taken_off")
	{
		assert(_PlanetWindow != nullptr);
		_PlanetWindow->Destroy();
		_PlanetWindow = nullptr;
	}
}
