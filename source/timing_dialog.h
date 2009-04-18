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

class Label;

class TimingDialog : public WWindow
{
public:
	TimingDialog(Widget * SupWidget);
	void Update(void);
private:
	// callbacks
	bool OnKey(const KeyEventInformation & KeyEventInformation);
	// member variables
	Label * m_AISecondsThisFrameLabel;
	Label * m_CommoditiesInCurrentSystemThisFrameLabel;
	Label * m_DispatchedMessagesThisFrameLabel;
	Label * m_FramesPerSecondLabel;
	Label * m_FrameToFrameSecondsThisFrameLabel;
	Label * m_GraphicsSecondsThisFrameLabel;
	Label * m_MessagingSecondsThisFrameLabel;
	Label * m_ParticleSystemsThisFrameLabel;
	Label * m_ParticlesThisFrameLabel;
	Label * m_PhysicsSecondsThisFrameLabel;
	Label * m_ProcessingSecondsThisFrameLabel;
	Label * m_ShipsInCurrentSystemThisFrameLabel;
	Label * m_ShotsInCurrentSystemThisFrameLabel;
};

#endif
