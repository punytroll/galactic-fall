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

#include "../callbacks/callbacks.h"
#include "../globals.h"
#include "../key_event_information.h"
#include "../string_cast.h"
#include "../system_statistics.h"
#include "label.h"
#include "timing_dialog.h"

UI::TimingDialog::TimingDialog(UI::Widget * SupWidget) :
	UI::Window(SupWidget, "Timing Information")
{
	SetPosition(Vector2f(300.0f, 300.0f));
	SetSize(Vector2f(350.0f, 400.0f));
	ConnectKeyCallback(Callback(this, &UI::TimingDialog::_OnKey));
	ConnectUpdatingCallback(Callback(this, &UI::TimingDialog::_OnUpdating));
	
	UI::Label * FramesPerSecondCaptionLabel(new UI::Label(this, "Frames per Second:"));
	
	FramesPerSecondCaptionLabel->SetPosition(Vector2f(10.0f, 40.0f));
	FramesPerSecondCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	FramesPerSecondCaptionLabel->SetAnchorRight(true);
	_FramesPerSecondLabel = new UI::Label(this, "0");
	_FramesPerSecondLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 40.0f));
	_FramesPerSecondLabel->SetSize(Vector2f(70.0f, 20.0f));
	_FramesPerSecondLabel->SetAnchorLeft(false);
	_FramesPerSecondLabel->SetAnchorRight(true);
	_FramesPerSecondLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * FrameToFrameSecondsThisFrameCaptionLabel(new UI::Label(this, "Seconds from Frame to Frame:"));
	
	FrameToFrameSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 60.0f));
	FrameToFrameSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	FrameToFrameSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_FrameToFrameSecondsThisFrameLabel = new UI::Label(this, "0");
	_FrameToFrameSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 60.0f));
	_FrameToFrameSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_FrameToFrameSecondsThisFrameLabel->SetAnchorLeft(false);
	_FrameToFrameSecondsThisFrameLabel->SetAnchorRight(true);
	_FrameToFrameSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ProcessingSecondsThisFrameCaptionLabel(new UI::Label(this, "Processing Seconds this Frame:"));
	
	ProcessingSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 80.0f));
	ProcessingSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ProcessingSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_ProcessingSecondsThisFrameLabel = new UI::Label(this, "0");
	_ProcessingSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 80.0f));
	_ProcessingSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_ProcessingSecondsThisFrameLabel->SetAnchorLeft(false);
	_ProcessingSecondsThisFrameLabel->SetAnchorRight(true);
	_ProcessingSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * AISecondsThisFrameCaptionLabel(new UI::Label(this, "AI Seconds this Frame:"));
	
	AISecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 100.0f));
	AISecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	AISecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_AISecondsThisFrameLabel = new UI::Label(this, "0");
	_AISecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 100.0f));
	_AISecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_AISecondsThisFrameLabel->SetAnchorLeft(false);
	_AISecondsThisFrameLabel->SetAnchorRight(true);
	_AISecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * PhysicsSecondsThisFrameCaptionLabel(new UI::Label(this, "Physics Seconds this Frame:"));
	
	PhysicsSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 120.0f));
	PhysicsSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	PhysicsSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_PhysicsSecondsThisFrameLabel = new UI::Label(this, "0");
	_PhysicsSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 120.0f));
	_PhysicsSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_PhysicsSecondsThisFrameLabel->SetAnchorLeft(false);
	_PhysicsSecondsThisFrameLabel->SetAnchorRight(true);
	_PhysicsSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * GraphicsSecondsThisFrameCaptionLabel(new UI::Label(this, "Graphics Seconds this Frame:"));
	
	GraphicsSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 140.0f));
	GraphicsSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	GraphicsSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_GraphicsSecondsThisFrameLabel = new UI::Label(this, "0");
	_GraphicsSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 140.0f));
	_GraphicsSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_GraphicsSecondsThisFrameLabel->SetAnchorLeft(false);
	_GraphicsSecondsThisFrameLabel->SetAnchorRight(true);
	_GraphicsSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * MessagingSecondsThisFrameCaptionLabel(new UI::Label(this, "Messaging Seconds this Frame:"));
	
	MessagingSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 160.0f));
	MessagingSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	MessagingSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_MessagingSecondsThisFrameLabel = new UI::Label(this, "0");
	_MessagingSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 160.0f));
	_MessagingSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_MessagingSecondsThisFrameLabel->SetAnchorLeft(false);
	_MessagingSecondsThisFrameLabel->SetAnchorRight(true);
	_MessagingSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * DispatchedMessagesThisFrameCaptionLabel(new UI::Label(this, "Dispatched messages this Frame:"));
	
	DispatchedMessagesThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 180.0f));
	DispatchedMessagesThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	DispatchedMessagesThisFrameCaptionLabel->SetAnchorRight(true);
	_DispatchedMessagesThisFrameLabel = new UI::Label(this, "0");
	_DispatchedMessagesThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 180.0f));
	_DispatchedMessagesThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_DispatchedMessagesThisFrameLabel->SetAnchorLeft(false);
	_DispatchedMessagesThisFrameLabel->SetAnchorRight(true);
	_DispatchedMessagesThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * CommoditiesInCurrentSystemThisFrameCaptionLabel(new UI::Label(this, "Commodities in current System this Frame:"));
	
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 200.0f));
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetAnchorRight(true);
	_CommoditiesInCurrentSystemThisFrameLabel = new UI::Label(this, "0");
	_CommoditiesInCurrentSystemThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 200.0f));
	_CommoditiesInCurrentSystemThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_CommoditiesInCurrentSystemThisFrameLabel->SetAnchorLeft(false);
	_CommoditiesInCurrentSystemThisFrameLabel->SetAnchorRight(true);
	_CommoditiesInCurrentSystemThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ShipsInCurrentSystemThisFrameCaptionLabel(new UI::Label(this, "Ships in current System this Frame:"));
	
	ShipsInCurrentSystemThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 220.0f));
	ShipsInCurrentSystemThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ShipsInCurrentSystemThisFrameCaptionLabel->SetAnchorRight(true);
	_ShipsInCurrentSystemThisFrameLabel = new UI::Label(this, "0");
	_ShipsInCurrentSystemThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 220.0f));
	_ShipsInCurrentSystemThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_ShipsInCurrentSystemThisFrameLabel->SetAnchorLeft(false);
	_ShipsInCurrentSystemThisFrameLabel->SetAnchorRight(true);
	_ShipsInCurrentSystemThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ShotsInCurrentSystemThisFrameCaptionLabel(new UI::Label(this, "Shots in current System this Frame:"));
	
	ShotsInCurrentSystemThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 240.0f));
	ShotsInCurrentSystemThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ShotsInCurrentSystemThisFrameCaptionLabel->SetAnchorRight(true);
	_ShotsInCurrentSystemThisFrameLabel = new UI::Label(this, "0");
	_ShotsInCurrentSystemThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 240.0f));
	_ShotsInCurrentSystemThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_ShotsInCurrentSystemThisFrameLabel->SetAnchorLeft(false);
	_ShotsInCurrentSystemThisFrameLabel->SetAnchorRight(true);
	_ShotsInCurrentSystemThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ParticleSystemsThisFrameCaptionLabel(new UI::Label(this, "Particle systems this Frame:"));
	
	ParticleSystemsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 260.0f));
	ParticleSystemsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ParticleSystemsThisFrameCaptionLabel->SetAnchorRight(true);
	_ParticleSystemsThisFrameLabel = new UI::Label(this, "0");
	_ParticleSystemsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 260.0f));
	_ParticleSystemsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_ParticleSystemsThisFrameLabel->SetAnchorLeft(false);
	_ParticleSystemsThisFrameLabel->SetAnchorRight(true);
	_ParticleSystemsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * ParticlesThisFrameCaptionLabel(new UI::Label(this, "Particles this Frame:"));
	
	ParticlesThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 280.0f));
	ParticlesThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	ParticlesThisFrameCaptionLabel->SetAnchorRight(true);
	_ParticlesThisFrameLabel = new UI::Label(this, "0");
	_ParticlesThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 280.0f));
	_ParticlesThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_ParticlesThisFrameLabel->SetAnchorLeft(false);
	_ParticlesThisFrameLabel->SetAnchorRight(true);
	_ParticlesThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	
	UI::Label * FontSecondsThisFrameCaptionLabel(new UI::Label(this, "Font Seconds this Frame:"));
	
	FontSecondsThisFrameCaptionLabel->SetPosition(Vector2f(10.0f, 300.0f));
	FontSecondsThisFrameCaptionLabel->SetSize(Vector2f(GetSize()[0] - 100.0f, 20.0f));
	FontSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_FontSecondsThisFrameLabel = new UI::Label(this, "0");
	_FontSecondsThisFrameLabel->SetPosition(Vector2f(GetSize()[0] - 80.0f, 300.0f));
	_FontSecondsThisFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	_FontSecondsThisFrameLabel->SetAnchorLeft(false);
	_FontSecondsThisFrameLabel->SetAnchorRight(true);
	_FontSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
}

bool UI::TimingDialog::_OnKey(const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) && (KeyEventInformation.IsDown() == true))
	{
		Destroy();
		
		return true;
	}
	
	return false;
}

void UI::TimingDialog::_OnUpdating(float RealTimeSeconds, float GameTimeSeconds)
{
	_AISecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetAISecondsThisFrame() * 1000, 2) + " ms");
	_CommoditiesInCurrentSystemThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetCommoditiesInCurrentSystemThisFrame()));
	_DispatchedMessagesThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetDispatchedMessagesThisFrame()));
	_FontSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetFontSecondsThisFrame()));
	_FramesPerSecondLabel->SetText(to_string_cast(g_SystemStatistics->GetFramesPerSecond(), 2));
	_FrameToFrameSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetFrameToFrameSecondsThisFrame() * 1000, 2) + " ms");
	_GraphicsSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetGraphicsSecondsThisFrame() * 1000, 2) + " ms");
	_MessagingSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetMessagingSecondsThisFrame() * 1000, 2) + " ms");
	_ParticleSystemsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetParticleSystemsThisFrame()));
	_ParticlesThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetParticlesThisFrame()));
	_PhysicsSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetPhysicsSecondsThisFrame() * 1000, 2) + " ms");
	_ProcessingSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetProcessingSecondsThisFrame() * 1000, 2) + " ms");
	_ShipsInCurrentSystemThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetShipsInCurrentSystemThisFrame()));
	_ShotsInCurrentSystemThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetShotsInCurrentSystemThisFrame()));
}
