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

#include "window.h"

namespace UI
{
	class Label;
	
	class TimingDialog : public UI::Window
	{
	public:
		TimingDialog(UI::Widget * SupWidget);
	private:
		// callbacks
		bool _OnKey(const KeyEventInformation & KeyEventInformation);
		void _OnUpdating(float RealTimeSeconds, float GameTimeSeconds);
		// member variables
		UI::Label * _AISecondsThisFrameLabel;
		UI::Label * _CommoditiesInCurrentSystemThisFrameLabel;
		UI::Label * _DispatchedMessagesThisFrameLabel;
		UI::Label * _FramesPerSecondLabel;
		UI::Label * _FrameToFrameSecondsThisFrameLabel;
		UI::Label * _GraphicsSecondsThisFrameLabel;
		UI::Label * _MessagingSecondsThisFrameLabel;
		UI::Label * _ParticleSystemsThisFrameLabel;
		UI::Label * _ParticlesThisFrameLabel;
		UI::Label * _PhysicsSecondsThisFrameLabel;
		UI::Label * _ProcessingSecondsThisFrameLabel;
		UI::Label * _ShipsInCurrentSystemThisFrameLabel;
		UI::Label * _ShotsInCurrentSystemThisFrameLabel;
	};
}

#endif
