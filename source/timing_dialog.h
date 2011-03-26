/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#ifndef TIMING_DIALOG_H
#define TIMING_DIALOG_H

#include "type_definitions.h"
#include "window.h"

namespace UI
{
	class Label;
}

class TimingDialog : public WWindow
{
public:
	TimingDialog(Widget * SupWidget);
	void Update(void);
private:
	// callbacks
	bool OnKey(const KeyEventInformation & KeyEventInformation);
	// member variables
	UI::Label * m_AISecondsThisFrameLabel;
	UI::Label * m_CommoditiesInCurrentSystemThisFrameLabel;
	UI::Label * m_DispatchedMessagesThisFrameLabel;
	UI::Label * m_FramesPerSecondLabel;
	UI::Label * m_FrameToFrameSecondsThisFrameLabel;
	UI::Label * m_GraphicsSecondsThisFrameLabel;
	UI::Label * m_MessagingSecondsThisFrameLabel;
	UI::Label * m_ParticleSystemsThisFrameLabel;
	UI::Label * m_ParticlesThisFrameLabel;
	UI::Label * m_PhysicsSecondsThisFrameLabel;
	UI::Label * m_ProcessingSecondsThisFrameLabel;
	UI::Label * m_ShipsInCurrentSystemThisFrameLabel;
	UI::Label * m_ShotsInCurrentSystemThisFrameLabel;
};

#endif
