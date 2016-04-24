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

#include <string_cast/string_cast.h>

#include "../globals.h"
#include "../system_statistics.h"
#include "key_event.h"
#include "label.h"
#include "timing_dialog.h"

UI::TimingDialog::TimingDialog(UI::Widget * SupWidget) :
	UI::Window(SupWidget, "Timing Information")
{
	ConnectKeyCallback(std::bind(&UI::TimingDialog::_OnKey, this, std::placeholders::_1));
	ConnectUpdatingCallback(std::bind(&UI::TimingDialog::_OnUpdating, this, std::placeholders::_1, std::placeholders::_2));
	
	auto FramesPerSecondCaptionLabel{new UI::Label{this, "Frames per Second:"}};
	
	FramesPerSecondCaptionLabel->SetLeft(10.0f);
	FramesPerSecondCaptionLabel->SetTop(40.0f);
	FramesPerSecondCaptionLabel->SetWidth(GetWidth() - 100.0f);
	FramesPerSecondCaptionLabel->SetHeight(20.0f);
	FramesPerSecondCaptionLabel->SetAnchorRight(true);
	_FramesPerSecondLabel = new UI::Label{this, "0"};
	_FramesPerSecondLabel->SetLeft(GetWidth() - 80.0f);
	_FramesPerSecondLabel->SetTop(40.0f);
	_FramesPerSecondLabel->SetWidth(70.0f);
	_FramesPerSecondLabel->SetHeight(20.0f);
	_FramesPerSecondLabel->SetAnchorLeft(false);
	_FramesPerSecondLabel->SetAnchorRight(true);
	_FramesPerSecondLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto FrameToFrameSecondsThisFrameCaptionLabel{new UI::Label{this, "Seconds from Frame to Frame:"}};
	
	FrameToFrameSecondsThisFrameCaptionLabel->SetLeft(10.0f);
	FrameToFrameSecondsThisFrameCaptionLabel->SetTop(60.0f);
	FrameToFrameSecondsThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	FrameToFrameSecondsThisFrameCaptionLabel->SetHeight(20.0f);
	FrameToFrameSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_FrameToFrameSecondsThisFrameLabel = new UI::Label{this, "0"};
	_FrameToFrameSecondsThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_FrameToFrameSecondsThisFrameLabel->SetTop(60.0f);
	_FrameToFrameSecondsThisFrameLabel->SetWidth(70.0f);
	_FrameToFrameSecondsThisFrameLabel->SetHeight(20.0f);
	_FrameToFrameSecondsThisFrameLabel->SetAnchorLeft(false);
	_FrameToFrameSecondsThisFrameLabel->SetAnchorRight(true);
	_FrameToFrameSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto ProcessingSecondsThisFrameCaptionLabel{new UI::Label{this, "Processing Seconds this Frame:"}};
	
	ProcessingSecondsThisFrameCaptionLabel->SetLeft(10.0f);
	ProcessingSecondsThisFrameCaptionLabel->SetTop(80.0f);
	ProcessingSecondsThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	ProcessingSecondsThisFrameCaptionLabel->SetHeight(20.0f);
	ProcessingSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_ProcessingSecondsThisFrameLabel = new UI::Label{this, "0"};
	_ProcessingSecondsThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_ProcessingSecondsThisFrameLabel->SetTop(80.0f);
	_ProcessingSecondsThisFrameLabel->SetWidth(70.0f);
	_ProcessingSecondsThisFrameLabel->SetHeight(20.0f);
	_ProcessingSecondsThisFrameLabel->SetAnchorLeft(false);
	_ProcessingSecondsThisFrameLabel->SetAnchorRight(true);
	_ProcessingSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto AISecondsThisFrameCaptionLabel{new UI::Label{this, "AI Seconds this Frame:"}};
	
	AISecondsThisFrameCaptionLabel->SetLeft(10.0f);
	AISecondsThisFrameCaptionLabel->SetTop(100.0f);
	AISecondsThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	AISecondsThisFrameCaptionLabel->SetHeight(20.0f);
	AISecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_AISecondsThisFrameLabel = new UI::Label{this, "0"};
	_AISecondsThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_AISecondsThisFrameLabel->SetTop(100.0f);
	_AISecondsThisFrameLabel->SetWidth(70.0f);
	_AISecondsThisFrameLabel->SetHeight(20.0f);
	_AISecondsThisFrameLabel->SetAnchorLeft(false);
	_AISecondsThisFrameLabel->SetAnchorRight(true);
	_AISecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto PhysicsSecondsThisFrameCaptionLabel{new UI::Label{this, "Physics Seconds this Frame:"}};
	
	PhysicsSecondsThisFrameCaptionLabel->SetLeft(10.0f);
	PhysicsSecondsThisFrameCaptionLabel->SetTop(120.0f);
	PhysicsSecondsThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	PhysicsSecondsThisFrameCaptionLabel->SetHeight(20.0f);
	PhysicsSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_PhysicsSecondsThisFrameLabel = new UI::Label{this, "0"};
	_PhysicsSecondsThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_PhysicsSecondsThisFrameLabel->SetTop(120.0f);
	_PhysicsSecondsThisFrameLabel->SetWidth(70.0f);
	_PhysicsSecondsThisFrameLabel->SetHeight(20.0f);
	_PhysicsSecondsThisFrameLabel->SetAnchorLeft(false);
	_PhysicsSecondsThisFrameLabel->SetAnchorRight(true);
	_PhysicsSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto GraphicsSecondsThisFrameCaptionLabel{new UI::Label{this, "Graphics Seconds this Frame:"}};
	
	GraphicsSecondsThisFrameCaptionLabel->SetLeft(10.0f);
	GraphicsSecondsThisFrameCaptionLabel->SetTop(140.0f);
	GraphicsSecondsThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	GraphicsSecondsThisFrameCaptionLabel->SetHeight(20.0f);
	GraphicsSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_GraphicsSecondsThisFrameLabel = new UI::Label{this, "0"};
	_GraphicsSecondsThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_GraphicsSecondsThisFrameLabel->SetTop(140.0f);
	_GraphicsSecondsThisFrameLabel->SetWidth(70.0f);
	_GraphicsSecondsThisFrameLabel->SetHeight(20.0f);
	_GraphicsSecondsThisFrameLabel->SetAnchorLeft(false);
	_GraphicsSecondsThisFrameLabel->SetAnchorRight(true);
	_GraphicsSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto MessagingSecondsThisFrameCaptionLabel{new UI::Label{this, "Messaging Seconds this Frame:"}};
	
	MessagingSecondsThisFrameCaptionLabel->SetLeft(10.0f);
	MessagingSecondsThisFrameCaptionLabel->SetTop(160.0f);
	MessagingSecondsThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	MessagingSecondsThisFrameCaptionLabel->SetHeight(20.0f);
	MessagingSecondsThisFrameCaptionLabel->SetAnchorRight(true);
	_MessagingSecondsThisFrameLabel = new UI::Label{this, "0"};
	_MessagingSecondsThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_MessagingSecondsThisFrameLabel->SetTop(160.0f);
	_MessagingSecondsThisFrameLabel->SetWidth(70.0f);
	_MessagingSecondsThisFrameLabel->SetHeight(20.0f);
	_MessagingSecondsThisFrameLabel->SetAnchorLeft(false);
	_MessagingSecondsThisFrameLabel->SetAnchorRight(true);
	_MessagingSecondsThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto DispatchedMessagesThisFrameCaptionLabel{new UI::Label{this, "Dispatched messages this Frame:"}};
	
	DispatchedMessagesThisFrameCaptionLabel->SetLeft(10.0f);
	DispatchedMessagesThisFrameCaptionLabel->SetTop(180.0f);
	DispatchedMessagesThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	DispatchedMessagesThisFrameCaptionLabel->SetHeight(20.0f);
	DispatchedMessagesThisFrameCaptionLabel->SetAnchorRight(true);
	_DispatchedMessagesThisFrameLabel = new UI::Label{this, "0"};
	_DispatchedMessagesThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_DispatchedMessagesThisFrameLabel->SetTop(180.0f);
	_DispatchedMessagesThisFrameLabel->SetWidth(70.0f);
	_DispatchedMessagesThisFrameLabel->SetHeight(20.0f);
	_DispatchedMessagesThisFrameLabel->SetAnchorLeft(false);
	_DispatchedMessagesThisFrameLabel->SetAnchorRight(true);
	_DispatchedMessagesThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto CommoditiesInCurrentSystemThisFrameCaptionLabel{new UI::Label{this, "Commodities in current System this Frame:"}};
	
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetLeft(10.0f);
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetTop(200.0f);
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetHeight(20.0f);
	CommoditiesInCurrentSystemThisFrameCaptionLabel->SetAnchorRight(true);
	_CommoditiesInCurrentSystemThisFrameLabel = new UI::Label{this, "0"};
	_CommoditiesInCurrentSystemThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_CommoditiesInCurrentSystemThisFrameLabel->SetTop(200.0f);
	_CommoditiesInCurrentSystemThisFrameLabel->SetWidth(70.0f);
	_CommoditiesInCurrentSystemThisFrameLabel->SetHeight(20.0f);
	_CommoditiesInCurrentSystemThisFrameLabel->SetAnchorLeft(false);
	_CommoditiesInCurrentSystemThisFrameLabel->SetAnchorRight(true);
	_CommoditiesInCurrentSystemThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto ShipsInCurrentSystemThisFrameCaptionLabel{new UI::Label{this, "Ships in current System this Frame:"}};
	
	ShipsInCurrentSystemThisFrameCaptionLabel->SetLeft(10.0f);
	ShipsInCurrentSystemThisFrameCaptionLabel->SetTop(220.0f);
	ShipsInCurrentSystemThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	ShipsInCurrentSystemThisFrameCaptionLabel->SetHeight(20.0f);
	ShipsInCurrentSystemThisFrameCaptionLabel->SetAnchorRight(true);
	_ShipsInCurrentSystemThisFrameLabel = new UI::Label{this, "0"};
	_ShipsInCurrentSystemThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_ShipsInCurrentSystemThisFrameLabel->SetTop(220.0f);
	_ShipsInCurrentSystemThisFrameLabel->SetWidth(70.0f);
	_ShipsInCurrentSystemThisFrameLabel->SetHeight(20.0f);
	_ShipsInCurrentSystemThisFrameLabel->SetAnchorLeft(false);
	_ShipsInCurrentSystemThisFrameLabel->SetAnchorRight(true);
	_ShipsInCurrentSystemThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto ShotsInCurrentSystemThisFrameCaptionLabel{new UI::Label{this, "Shots in current System this Frame:"}};
	
	ShotsInCurrentSystemThisFrameCaptionLabel->SetLeft(10.0f);
	ShotsInCurrentSystemThisFrameCaptionLabel->SetTop(240.0f);
	ShotsInCurrentSystemThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	ShotsInCurrentSystemThisFrameCaptionLabel->SetHeight(20.0f);
	ShotsInCurrentSystemThisFrameCaptionLabel->SetAnchorRight(true);
	_ShotsInCurrentSystemThisFrameLabel = new UI::Label{this, "0"};
	_ShotsInCurrentSystemThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_ShotsInCurrentSystemThisFrameLabel->SetTop(240.0f);
	_ShotsInCurrentSystemThisFrameLabel->SetWidth(70.0f);
	_ShotsInCurrentSystemThisFrameLabel->SetHeight(20.0f);
	_ShotsInCurrentSystemThisFrameLabel->SetAnchorLeft(false);
	_ShotsInCurrentSystemThisFrameLabel->SetAnchorRight(true);
	_ShotsInCurrentSystemThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto ParticleSystemsUpdatedThisFrameCaptionLabel{new UI::Label{this, "Particle systems updated this Frame:"}};
	
	ParticleSystemsUpdatedThisFrameCaptionLabel->SetLeft(10.0f);
	ParticleSystemsUpdatedThisFrameCaptionLabel->SetTop(260.0f);
	ParticleSystemsUpdatedThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	ParticleSystemsUpdatedThisFrameCaptionLabel->SetHeight(20.0f);
	ParticleSystemsUpdatedThisFrameCaptionLabel->SetAnchorRight(true);
	_ParticleSystemsUpdatedThisFrameLabel = new UI::Label{this, "0"};
	_ParticleSystemsUpdatedThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_ParticleSystemsUpdatedThisFrameLabel->SetTop(260.0f);
	_ParticleSystemsUpdatedThisFrameLabel->SetWidth(70.0f);
	_ParticleSystemsUpdatedThisFrameLabel->SetHeight(20.0f);
	_ParticleSystemsUpdatedThisFrameLabel->SetAnchorLeft(false);
	_ParticleSystemsUpdatedThisFrameLabel->SetAnchorRight(true);
	_ParticleSystemsUpdatedThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto ParticleSystemsDrawnThisFrameCaptionLabel{new UI::Label{this, "Particle systems drawn this Frame:"}};
	
	ParticleSystemsDrawnThisFrameCaptionLabel->SetLeft(10.0f);
	ParticleSystemsDrawnThisFrameCaptionLabel->SetTop(280.0f);
	ParticleSystemsDrawnThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	ParticleSystemsDrawnThisFrameCaptionLabel->SetHeight(20.0f);
	ParticleSystemsDrawnThisFrameCaptionLabel->SetAnchorRight(true);
	_ParticleSystemsDrawnThisFrameLabel = new UI::Label{this, "0"};
	_ParticleSystemsDrawnThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_ParticleSystemsDrawnThisFrameLabel->SetTop(280.0f);
	_ParticleSystemsDrawnThisFrameLabel->SetWidth(70.0f);
	_ParticleSystemsDrawnThisFrameLabel->SetHeight(20.0f);
	_ParticleSystemsDrawnThisFrameLabel->SetAnchorLeft(false);
	_ParticleSystemsDrawnThisFrameLabel->SetAnchorRight(true);
	_ParticleSystemsDrawnThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto ParticlesUpdatedThisFrameCaptionLabel{new UI::Label{this, "Particles updated this Frame:"}};
	
	ParticlesUpdatedThisFrameCaptionLabel->SetLeft(10.0f);
	ParticlesUpdatedThisFrameCaptionLabel->SetTop(300.0f);
	ParticlesUpdatedThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	ParticlesUpdatedThisFrameCaptionLabel->SetHeight(20.0f);
	ParticlesUpdatedThisFrameCaptionLabel->SetAnchorRight(true);
	_ParticlesUpdatedThisFrameLabel = new UI::Label{this, "0"};
	_ParticlesUpdatedThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_ParticlesUpdatedThisFrameLabel->SetTop(300.0f);
	_ParticlesUpdatedThisFrameLabel->SetWidth(70.0f);
	_ParticlesUpdatedThisFrameLabel->SetHeight(20.0f);
	_ParticlesUpdatedThisFrameLabel->SetAnchorLeft(false);
	_ParticlesUpdatedThisFrameLabel->SetAnchorRight(true);
	_ParticlesUpdatedThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto ParticlesDrawnThisFrameCaptionLabel{new UI::Label{this, "Particles drawn this Frame:"}};
	
	ParticlesDrawnThisFrameCaptionLabel->SetLeft(10.0f);
	ParticlesDrawnThisFrameCaptionLabel->SetTop(320.0f);
	ParticlesDrawnThisFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	ParticlesDrawnThisFrameCaptionLabel->SetHeight(20.0f);
	ParticlesDrawnThisFrameCaptionLabel->SetAnchorRight(true);
	_ParticlesDrawnThisFrameLabel = new UI::Label{this, "0"};
	_ParticlesDrawnThisFrameLabel->SetLeft(GetWidth() - 80.0f);
	_ParticlesDrawnThisFrameLabel->SetTop(320.0f);
	_ParticlesDrawnThisFrameLabel->SetWidth(70.0f);
	_ParticlesDrawnThisFrameLabel->SetHeight(20.0f);
	_ParticlesDrawnThisFrameLabel->SetAnchorLeft(false);
	_ParticlesDrawnThisFrameLabel->SetAnchorRight(true);
	_ParticlesDrawnThisFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	
	auto FontSecondsLastFrameCaptionLabel{new UI::Label{this, "Font Seconds last Frame:"}};
	
	FontSecondsLastFrameCaptionLabel->SetLeft(10.0f);
	FontSecondsLastFrameCaptionLabel->SetTop(340.0f);
	FontSecondsLastFrameCaptionLabel->SetWidth(GetWidth() - 100.0f);
	FontSecondsLastFrameCaptionLabel->SetHeight(20.0f);
	FontSecondsLastFrameCaptionLabel->SetAnchorRight(true);
	_FontSecondsLastFrameLabel = new UI::Label{this, "0"};
	_FontSecondsLastFrameLabel->SetLeft(GetWidth() - 80.0f);
	_FontSecondsLastFrameLabel->SetTop(340.0f);
	_FontSecondsLastFrameLabel->SetWidth(70.0f);
	_FontSecondsLastFrameLabel->SetHeight(20.0f);
	_FontSecondsLastFrameLabel->SetAnchorLeft(false);
	_FontSecondsLastFrameLabel->SetAnchorRight(true);
	_FontSecondsLastFrameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
}

void UI::TimingDialog::_OnKey(UI::KeyEvent & KeyEvent)
{
	if((KeyEvent.GetKeyCode() == 9 /* ESCAPE */) && (KeyEvent.IsDown() == true))
	{
		Destroy();
	}
}

void UI::TimingDialog::_OnUpdating(float RealTimeSeconds, float GameTimeSeconds)
{
	_AISecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetAISecondsThisFrame() * 1000, 2) + " ms");
	_CommoditiesInCurrentSystemThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetCommoditiesInCurrentSystemThisFrame()));
	_DispatchedMessagesThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetDispatchedMessagesThisFrame()));
	_FontSecondsLastFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetFontSecondsLastFrame() * 1000, 2) + " ms");
	_FramesPerSecondLabel->SetText(to_string_cast(g_SystemStatistics->GetFramesPerSecond(), 2));
	_FrameToFrameSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetFrameToFrameSecondsThisFrame() * 1000, 2) + " ms");
	_GraphicsSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetGraphicsSecondsThisFrame() * 1000, 2) + " ms");
	_MessagingSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetMessagingSecondsThisFrame() * 1000, 2) + " ms");
	_ParticleSystemsDrawnThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetParticleSystemsDrawnThisFrame()));
	_ParticleSystemsUpdatedThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetParticleSystemsUpdatedThisFrame()));
	_ParticlesDrawnThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetParticlesDrawnThisFrame()));
	_ParticlesUpdatedThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetParticlesUpdatedThisFrame()));
	_PhysicsSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetPhysicsSecondsThisFrame() * 1000, 2) + " ms");
	_ProcessingSecondsThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetProcessingSecondsThisFrame() * 1000, 2) + " ms");
	_ShipsInCurrentSystemThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetShipsInCurrentSystemThisFrame()));
	_ShotsInCurrentSystemThisFrameLabel->SetText(to_string_cast(g_SystemStatistics->GetShotsInCurrentSystemThisFrame()));
}
