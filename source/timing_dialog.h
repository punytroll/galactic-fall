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
	void UpdateFramesPerSecond(float FramesPerSecond);
	void UpdateMessagingSecondsPerFrame(float MessagingSecondsPerFrame);
	void UpdateDispatchedMessagesPerFrame(u4byte DispatchedMessagesPerFrame);
	void UpdateAISecondsPerFrame(float AISecondsPerFrame);
	void UpdateGraphicsSecondsPerFrame(float GraphicsSecondsPerFrame);
	void UpdatePhysicsSecondsPerFrame(float PhysicsSecondsPerFrame);
	void UpdateTotalSecondsPerFrame(float TotalSecondsPerFrame);
	void UpdateTotalSecondsPerFrameProcessing(float TotalSecondsPerFrameProcessing);
private:
	Label * m_AISecondsPerFrameLabel;
	Label * m_DispatchedMessagesPerFrameLabel;
	Label * m_FramesPerSecondLabel;
	Label * m_GraphicsSecondsPerFrameLabel;
	Label * m_MessagingSecondsPerFrameLabel;
	Label * m_PhysicsSecondsPerFrameLabel;
	Label * m_TotalSecondsPerFrameLabel;
	Label * m_TotalSecondsPerFrameProcessingLabel;
};

#endif
