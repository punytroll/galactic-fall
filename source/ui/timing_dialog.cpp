/**
 * galactic-fall
 * Copyright (C) 2007-2018  Hagen Möbius
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

#include <expressions/operators.h>

#include <string_cast/string_cast.h>

#include "../globals.h"
#include "../system_statistics.h"
#include "key_event.h"
#include "label.h"
#include "timing_dialog.h"

using namespace Expressions::Operators;

UI::TimingDialog::TimingDialog(void) :
	_AISecondsThisFrameLabel(nullptr),
	_CommoditiesInCurrentSystemThisFrameLabel(nullptr),
	_DispatchedMessagesThisFrameLabel(nullptr),
	_FontSecondsLastFrameLabel(nullptr),
	_FramesPerSecondLabel(nullptr),
	_FrameToFrameSecondsThisFrameLabel(nullptr),
	_GraphicsSecondsThisFrameLabel(nullptr),
	_MessagingSecondsThisFrameLabel(nullptr),
	_ParticleSystemsDrawnThisFrameLabel(nullptr),
	_ParticleSystemsUpdatedThisFrameLabel(nullptr),
	_ParticlesDrawnThisFrameLabel(nullptr),
	_ParticlesUpdatedThisFrameLabel(nullptr),
	_PhysicsSecondsThisFrameLabel(nullptr),
	_ProcessingSecondsThisFrameLabel(nullptr),
	_ShipsInCurrentSystemThisFrameLabel(nullptr),
	_ShotsInCurrentSystemThisFrameLabel(nullptr)
{
	SetTitle("Timing Information");
	ConnectKeyCallback(std::bind(&UI::TimingDialog::_OnKey, this, std::placeholders::_1));
	ConnectUpdatingCallback(std::bind(&UI::TimingDialog::_OnUpdating, this, std::placeholders::_1, std::placeholders::_2));
	
	auto Top{40.0f};
	
	Top += _AddMeasure(Top, "Frames per Second:", _FramesPerSecondLabel);
	Top += _AddMeasure(Top, "Seconds from Frame to Frame:", _FrameToFrameSecondsThisFrameLabel);
	Top += _AddMeasure(Top, "Processing Seconds this Frame:", _ProcessingSecondsThisFrameLabel);
	Top += _AddMeasure(Top, "AI Seconds this Frame:", _AISecondsThisFrameLabel);
	Top += _AddMeasure(Top, "Physics Seconds this Frame:", _PhysicsSecondsThisFrameLabel);
	Top += _AddMeasure(Top, "Graphics Seconds this Frame:", _GraphicsSecondsThisFrameLabel);
	Top += _AddMeasure(Top, "Messaging Seconds this Frame:", _MessagingSecondsThisFrameLabel);
	Top += _AddMeasure(Top, "Dispatched messages this Frame:", _DispatchedMessagesThisFrameLabel);
	Top += _AddMeasure(Top, "Commodities in current System this Frame:", _CommoditiesInCurrentSystemThisFrameLabel);
	Top += _AddMeasure(Top, "Ships in current System this Frame:", _ShipsInCurrentSystemThisFrameLabel);
	Top += _AddMeasure(Top, "Shots in current System this Frame:", _ShotsInCurrentSystemThisFrameLabel);
	Top += _AddMeasure(Top, "Particle systems updated this Frame:", _ParticleSystemsUpdatedThisFrameLabel);
	Top += _AddMeasure(Top, "Particle systems drawn this Frame:", _ParticleSystemsDrawnThisFrameLabel);
	Top += _AddMeasure(Top, "Particles updated this Frame:", _ParticlesUpdatedThisFrameLabel);
	Top += _AddMeasure(Top, "Particles drawn this Frame:", _ParticlesDrawnThisFrameLabel);
	Top += _AddMeasure(Top, "Font Seconds last Frame:", _FontSecondsLastFrameLabel);
}

float UI::TimingDialog::_AddMeasure(float Top, const std::string & Label, UI::Label *& ValueLabel)
{
	assert(ValueLabel == nullptr);
	
	// create components
	auto CaptionLabel{new UI::Label{}};
	
	ValueLabel = new UI::Label{};
	// initialize components
	CaptionLabel->SetLeft(10.0_c);
	CaptionLabel->SetTop(constant(Top));
	CaptionLabel->SetWidth(width(this) - 100.0_c);
	CaptionLabel->SetHeight(20.0_c);
	CaptionLabel->SetText(Label);
	ValueLabel->SetLeft(width(this) - 80.0_c);
	ValueLabel->SetTop(constant(Top));
	ValueLabel->SetWidth(70.0_c);
	ValueLabel->SetHeight(20.0_c);
	ValueLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	// add components
	AddSubWidget(CaptionLabel);
	AddSubWidget(ValueLabel);
	
	return 20.0f;
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
