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

#include "globals.h"
#include "label.h"
#include "string_cast.h"
#include "system_statistics.h"
#include "timing_dialog.h"

TimingDialog::TimingDialog(Widget * SupWidget) :
	WWindow(SupWidget, "Timing Information")
{
	SetPosition(Vector2f(300.0f, 300.0f));
	SetSize(Vector2f(320.0f, 300.0f));
	
	Label * FramesPerSecondCaptionLabel(new Label(this, "Frames per Second:"));
	
	FramesPerSecondCaptionLabel->SetPosition(Vector2f(10.0f, 40.0f));
	FramesPerSecondCaptionLabel->SetSize(Vector2f(220.0f, 20.0f));
	m_FramesPerSecondLabel = new Label(this, "0");
	m_FramesPerSecondLabel->SetPosition(Vector2f(240.0f, 40.0f));
	m_FramesPerSecondLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_FramesPerSecondLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * FrameToFrameSecondsThisFrameCaptionLabel(new Label(this, "Seconds from Frame to Frame:"));
	
	FrameToFrameSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 60.0f));
	FrameToFrameSecondsThisFrameCaptionLabel->SetSize(Vector2f(220.0f, 20.0f));
	m_FrameToFrameSecondsThisFrameLabel = new Label(this, "0");
	m_FrameToFrameSecondsThisFrameLabel->SetPosition(Vector2f(240.0f, 60.0f));
	m_FrameToFrameSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_FrameToFrameSecondsThisFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * ProcessingSecondsThisFrameCaptionLabel(new Label(this, "Processing Seconds this Frame:"));
	
	ProcessingSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 80.0f));
	ProcessingSecondsThisFrameCaptionLabel->SetSize(Vector2f(220.0f, 20.0f));
	m_ProcessingSecondsThisFrameLabel = new Label(this, "0");
	m_ProcessingSecondsThisFrameLabel->SetPosition(Vector2f(240.0f, 80.0f));
	m_ProcessingSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_ProcessingSecondsThisFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * AISecondsThisFrameCaptionLabel(new Label(this, "AI Seconds this Frame:"));
	
	AISecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 100.0f));
	AISecondsThisFrameCaptionLabel->SetSize(Vector2f(220.0f, 20.0f));
	m_AISecondsThisFrameLabel = new Label(this, "0");
	m_AISecondsThisFrameLabel->SetPosition(Vector2f(240.0f, 100.0f));
	m_AISecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_AISecondsThisFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * PhysicsSecondsThisFrameCaptionLabel(new Label(this, "Physics Seconds this Frame:"));
	
	PhysicsSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 120.0f));
	PhysicsSecondsThisFrameCaptionLabel->SetSize(Vector2f(220.0f, 20.0f));
	m_PhysicsSecondsThisFrameLabel = new Label(this, "0");
	m_PhysicsSecondsThisFrameLabel->SetPosition(Vector2f(240.0f, 120.0f));
	m_PhysicsSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_PhysicsSecondsThisFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * GraphicsSecondsThisFrameCaptionLabel(new Label(this, "Graphics Seconds this Frame:"));
	
	GraphicsSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 140.0f));
	GraphicsSecondsThisFrameCaptionLabel->SetSize(Vector2f(220.0f, 20.0f));
	m_GraphicsSecondsThisFrameLabel = new Label(this, "0");
	m_GraphicsSecondsThisFrameLabel->SetPosition(Vector2f(240.0f, 140.0f));
	m_GraphicsSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_GraphicsSecondsThisFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * MessagingSecondsThisFrameCaptionLabel(new Label(this, "Messaging Seconds this Frame:"));
	
	MessagingSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 160.0f));
	MessagingSecondsThisFrameCaptionLabel->SetSize(Vector2f(220.0f, 20.0f));
	m_MessagingSecondsThisFrameLabel = new Label(this, "0");
	m_MessagingSecondsThisFrameLabel->SetPosition(Vector2f(240.0f, 160.0f));
	m_MessagingSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_MessagingSecondsThisFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * DispatchedMessagesThisFrameCaptionLabel(new Label(this, "Dispatched messages this Frame:"));
	
	DispatchedMessagesThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 180.0f));
	DispatchedMessagesThisFrameCaptionLabel->SetSize(Vector2f(220.0f, 20.0f));
	m_DispatchedMessagesThisFrameLabel = new Label(this, "0");
	m_DispatchedMessagesThisFrameLabel->SetPosition(Vector2f(240.0f, 180.0f));
	m_DispatchedMessagesThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_DispatchedMessagesThisFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
}

void TimingDialog::Update(void)
{
	m_AISecondsThisFrameLabel->SetString(to_string_cast(g_SystemStatistics->GetAISecondsThisFrame() * 1000, 2) + " ms");
	m_DispatchedMessagesThisFrameLabel->SetString(to_string_cast(g_SystemStatistics->GetDispatchedMessagesThisFrame()));
	m_FramesPerSecondLabel->SetString(to_string_cast(g_SystemStatistics->GetFramesPerSecond(), 2));
	m_FrameToFrameSecondsThisFrameLabel->SetString(to_string_cast(g_SystemStatistics->GetFrameToFrameSecondsThisFrame() * 1000, 2) + " ms");
	m_GraphicsSecondsThisFrameLabel->SetString(to_string_cast(g_SystemStatistics->GetGraphicsSecondsThisFrame() * 1000, 2) + " ms");
	m_MessagingSecondsThisFrameLabel->SetString(to_string_cast(g_SystemStatistics->GetMessagingSecondsThisFrame() * 1000, 2) + " ms");
	m_PhysicsSecondsThisFrameLabel->SetString(to_string_cast(g_SystemStatistics->GetPhysicsSecondsThisFrame() * 1000, 2) + " ms");
	m_ProcessingSecondsThisFrameLabel->SetString(to_string_cast(g_SystemStatistics->GetProcessingSecondsThisFrame() * 1000, 2) + " ms");
}
