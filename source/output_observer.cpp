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

#include "command_mind.h"
#include "globals.h"
#include "message.h"
#include "output_observer.h"
#include "planet.h"
#include "planet_dialog.h"
#include "user_interface.h"

/// @todo evil hack
class CommandMind;

extern Reference< CommandMind > g_InputMind;

OutputObserver::OutputObserver(void) :
	m_PlanetDialog(0)
{
}

void OutputObserver::HandleMessage(Message * Message)
{
	if((g_InputMind.IsValid() == true) && (GetObservedCharacter().Get() == g_InputMind->GetCharacter()))
	{
		if(Message->GetTypeIdentifier() == "landed")
		{
			assert(m_PlanetDialog == 0);
			assert(Message->GetSender()->GetTypeIdentifier() == "planet");
			m_PlanetDialog = new PlanetDialog(g_UserInterface->GetRootWidget(), dynamic_cast< Planet * >(Message->GetSender().Get()), GetObservedCharacter().Get());
			m_PlanetDialog->GrabKeyFocus();
		}
		else if(Message->GetTypeIdentifier() == "taken_off")
		{
			assert(m_PlanetDialog != 0);
			m_PlanetDialog->Destroy();
			m_PlanetDialog = 0;
		}
	}
}
