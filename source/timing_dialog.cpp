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

#include "callbacks/callbacks.h"
#include "globals.h"
#include "key_event_information.h"
#include "string_cast.h"
#include "system_statistics.h"
#include "timing_dialog.h"
#include "ui/label.h"

TimingDialog::TimingDialog(Widget * SupWidget) :
	WWindow(SupWidget, "Timing Information")
{
	SetPosition(Vector2f(300.0f, 300.0f));
	SetSize(Vector2f(350.0f, 400.0f));
	ConnectKeyCallback(Callback(this, &TimingDialog::OnKey));
	
	UI::Label * FramesPerSecondCaptionLabel(new UI::Label(this, "Frames per Second:"));
	
	FramesPerSecondCaptionLabel->SetPosition(Vector2f(10.0f, 40.0f));
	FramesPerSecondCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	FramesPerSecondCaptionLabel->SetAnchorRight(true);
	m_FramesPerSecondLabel = new UI::Label(this, "0");
	m_FramesPerSecondLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 40.0f));
	m_FramesPerSecondLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_FramesPerSecondLabel->SetAnchorLeft(false);
	m_FramesPerSecondLabel->SetAnchorRight(true);
	m_FramesPerSecondLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * FrameToFrameSecondsThisFrameCaptionLabel(new UI::Label(this, "Seconds from Frame to Frame:"));
	
	FrameToFrameSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 60.0f));
	FrameToFrameSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	FrameToFrameSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	m_FrameToFrameSecondsThisFrameLabel = new UI::Label(this, "0");
	m_FrameToFrameSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 60.0f));
	m_FrameToFrameSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_FrameToFrameSecondsThisFrameLabel->SetAnchorLeft(false);
	m_FrameToFrameSecondsThisFrameLabel->SetAnchorRight(true);
	m_FrameToFrameSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ProcessingSecondsThisFrameCaptionLabel(new UI::Label(this, "Processing Seconds this Frame:"));
	
	ProcessingSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 80.0f));
	ProcessingSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ProcessingSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	m_ProcessingSecondsThisFrameLabel = new UI::Label(this, "0");
	m_ProcessingSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 80.0f));
	m_ProcessingSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_ProcessingSecondsThisFrameLabel->SetAnchorLeft(false);
	m_ProcessingSecondsThisFrameLabel->SetAnchorRight(true);
	m_ProcessingSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * AISecondsThisFrameCaptionLabel(new UI::Label(this, "AI Seconds this Frame:"));
	
	AISecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 100.0f));
	AISecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	AISecondsThisFrameCaptionLabel->SetAnchorRight(true);
	m_AISecondsThisFrameLabel = new UI::Label(this, "0");
	m_AISecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 100.0f));
	m_AISecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_AISecondsThisFrameLabel->SetAnchorLeft(false);
	m_AISecondsThisFrameLabel->SetAnchorRight(true);
	m_AISecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * PhysicsSecondsThisFrameCaptionLabel(new UI::Label(this, "Physics Seconds this Frame:"));
	
	PhysicsSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 120.0f));
	PhysicsSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	PhysicsSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	m_PhysicsSecondsThisFrameLabel = new UI::Label(this, "0");
	m_PhysicsSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 120.0f));
	m_PhysicsSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_PhysicsSecondsThisFrameLabel->SetAnchorLeft(false);
	m_PhysicsSecondsThisFrameLabel->SetAnchorRight(true);
	m_PhysicsSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * GraphicsSecondsThisFrameCaptionLabel(new UI::Label(this, "Graphics Seconds this Frame:"));
	
	GraphicsSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 140.0f));
	GraphicsSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	GraphicsSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	m_GraphicsSecondsThisFrameLabel = new UI::Label(this, "0");
	m_GraphicsSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 140.0f));
	m_GraphicsSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_GraphicsSecondsThisFrameLabel->SetAnchorLeft(false);
	m_GraphicsSecondsThisFrameLabel->SetAnchorRight(true);
	m_GraphicsSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * MessagingSecondsThisFrameCaptionLabel(new UI::Label(this, "Messaging Seconds this Frame:"));
	
	MessagingSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 160.0f));
	MessagingSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	MessagingSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	m_MessagingSecondsThisFrameLabel = new UI::Label(this, "0");
	m_MessagingSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 160.0f));
	m_MessagingSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_MessagingSecondsThisFrameLabel->SetAnchorLeft(false);
	m_MessagingSecondsThisFrameLabel->SetAnchorRight(true);
	m_MessagingSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * DispatchedMessagesThisFrameCaptionLabel(new UI::Label(this, "Dispatched messages this Frame:"));
	
	DispatchedMessagesThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 180.0f));
	DispatchedMessagesThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	DispatchedMessagesThisFrameCaptionLabel->SetAnchorRight(true);
	m_DispatchedMessagesThisFrameLabel = new UI::Label(this, "0");
	m_DispatchedMessagesThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 180.0f));
	m_DispatchedMessagesThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_DispatchedMessagesThisFrameLabel->SetAnchorLeft(false);
	m_DispatchedMessagesThisFrameLabel->SetAnchorRight(true);
	m_DispatchedMessagesThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * CommoditiesInCurrentSystemThisFrameCaptionLabel(new UI::Label(this, "Commodities in current System this Frame:"));
	
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 200.0f));
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetAnchorRight(true);
	m_CommoditiesInCurrentSystemThisFrameLabel = new UI::Label(this, "0");
	m_CommoditiesInCurrentSystemThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 200.0f));
	m_CommoditiesInCurrentSystemThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_CommoditiesInCurrentSystemThisFrameLabel->SetAnchorLeft(false);
	m_CommoditiesInCurrentSystemThisFrameLabel->SetAnchorRight(true);
	m_CommoditiesInCurrentSystemThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ShipsInCurrentSystemThisFrameCaptionLabel(new UI::Label(this, "Ships in current System this Frame:"));
	
	ShipsInCurrentSystemThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 220.0f));
	ShipsInCurrentSystemThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ShipsInCurrentSystemThisFrameCaptionLabel->SetAnchorRight(true);
	m_ShipsInCurrentSystemThisFrameLabel = new UI::Label(this, "0");
	m_ShipsInCurrentSystemThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 220.0f));
	m_ShipsInCurrentSystemThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_ShipsInCurrentSystemThisFrameLabel->SetAnchorLeft(false);
	m_ShipsInCurrentSystemThisFrameLabel->SetAnchorRight(true);
	m_ShipsInCurrentSystemThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ShotsInCurrentSystemThisFrameCaptionLabel(new UI::Label(this, "Shots in current System this Frame:"));
	
	ShotsInCurrentSystemThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 240.0f));
	ShotsInCurrentSystemThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ShotsInCurrentSystemThisFrameCaptionLabel->SetAnchorRight(true);
	m_ShotsInCurrentSystemThisFrameLabel = new UI::Label(this, "0");
	m_ShotsInCurrentSystemThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 240.0f));
	m_ShotsInCurrentSystemThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_ShotsInCurrentSystemThisFrameLabel->SetAnchorLeft(false);
	m_ShotsInCurrentSystemThisFrameLabel->SetAnchorRight(true);
	m_ShotsInCurrentSystemThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ParticleSystemsThisFrameCaptionLabel(new UI::Label(this, "Particle systems this Frame:"));
	
	ParticleSystemsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 260.0f));
	ParticleSystemsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ParticleSystemsThisFrameCaptionLabel->SetAnchorRight(true);
	m_ParticleSystemsThisFrameLabel = new UI::Label(this, "0");
	m_ParticleSystemsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 260.0f));
	m_ParticleSystemsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_ParticleSystemsThisFrameLabel->SetAnchorLeft(false);
	m_ParticleSystemsThisFrameLabel->SetAnchorRight(true);
	m_ParticleSystemsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ParticlesThisFrameCaptionLabel(new UI::Label(this, "Particles this Frame:"));
	
	ParticlesThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 280.0f));
	ParticlesThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ParticlesThisFrameCaptionLabel->SetAnchorRight(true);
	m_ParticlesThisFrameLabel = new UI::Label(this, "0");
	m_ParticlesThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 280.0f));
	m_ParticlesThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_ParticlesThisFrameLabel->SetAnchorLeft(false);
	m_ParticlesThisFrameLabel->SetAnchorRight(true);
	m_ParticlesThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
}

bool TimingDialog::OnKey(const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) && (KeyEventInformation.IsDown() == true))
	{
		Destroy();
		
		return true;
	}
	
	return false;
}

void TimingDialog::Update(void)
{
	m_AISecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetAISecondsThisFrame() * 1000, 2) + " ms");
	m_CommoditiesInCurrentSystemThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetCommoditiesInCurrentSystemThisFrame()));
	m_DispatchedMessagesThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetDispatchedMessagesThisFrame()));
	m_FramesPerSecondLabel->SetText(to_string_cast(g_SystemStatistics->GetFramesPerSecond(), 2));
	m_FrameToFrameSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetFrameToFrameSecondsThisFrame() * 1000, 2) + " ms");
	m_GraphicsSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetGraphicsSecondsThisFrame() * 1000, 2) + " ms");
	m_MessagingSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetMessagingSecondsThisFrame() * 1000, 2) + " ms");
	m_ParticleSystemsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetParticleSystemsThisFrame()));
	m_ParticlesThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetParticlesThisFrame()));
	m_PhysicsSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetPhysicsSecondsThisFrame() * 1000, 2) + " ms");
	m_ProcessingSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetProcessingSecondsThisFrame() * 1000, 2) + " ms");
	m_ShipsInCurrentSystemThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetShipsInCurrentSystemThisFrame()));
	m_ShotsInCurrentSystemThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetShotsInCurrentSystemThisFrame()));
}
