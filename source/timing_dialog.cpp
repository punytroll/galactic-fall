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

#include "label.h"
#include "string_cast.h"
#include "timing_dialog.h"

TimingDialog::TimingDialog(Widget * SupWidget) :
	WWindow(SupWidget, "Timing Information")
{
	SetPosition(Vector2f(300.0f, 300.0f));
	SetSize(Vector2f(250.0f, 300.0f));
	
	Label * FramesPerSecondCaptionLabel(new Label(this, "Frames per Second:"));
	
	FramesPerSecondCaptionLabel->SetPosition(Vector2f(10.0f, 40.0f));
	FramesPerSecondCaptionLabel->SetSize(Vector2f(150.0f, 20.0f));
	m_FramesPerSecondLabel = new Label(this, "0");
	m_FramesPerSecondLabel->SetPosition(Vector2f(170.0f, 40.0f));
	m_FramesPerSecondLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_FramesPerSecondLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * TotalSecondsPerFrameCaptionLabel(new Label(this, "Total Seconds per Frame:"));
	
	TotalSecondsPerFrameCaptionLabel->SetPosition(Vector2f(10.0f, 60.0f));
	TotalSecondsPerFrameCaptionLabel->SetSize(Vector2f(150.0f, 20.0f));
	m_TotalSecondsPerFrameLabel = new Label(this, "0");
	m_TotalSecondsPerFrameLabel->SetPosition(Vector2f(170.0f, 60.0f));
	m_TotalSecondsPerFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_TotalSecondsPerFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * AISecondsPerFrameCaptionLabel(new Label(this, "AI Seconds per Frame:"));
	
	AISecondsPerFrameCaptionLabel->SetPosition(Vector2f(10.0f, 80.0f));
	AISecondsPerFrameCaptionLabel->SetSize(Vector2f(150.0f, 20.0f));
	m_AISecondsPerFrameLabel = new Label(this, "0");
	m_AISecondsPerFrameLabel->SetPosition(Vector2f(170.0f, 80.0f));
	m_AISecondsPerFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_AISecondsPerFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * PhysicsSecondsPerFrameCaptionLabel(new Label(this, "Physics Seconds per Frame:"));
	
	PhysicsSecondsPerFrameCaptionLabel->SetPosition(Vector2f(10.0f, 100.0f));
	PhysicsSecondsPerFrameCaptionLabel->SetSize(Vector2f(150.0f, 20.0f));
	m_PhysicsSecondsPerFrameLabel = new Label(this, "0");
	m_PhysicsSecondsPerFrameLabel->SetPosition(Vector2f(170.0f, 100.0f));
	m_PhysicsSecondsPerFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_PhysicsSecondsPerFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	
	Label * GraphicsSecondsPerFrameCaptionLabel(new Label(this, "Graphics Seconds per Frame:"));
	
	GraphicsSecondsPerFrameCaptionLabel->SetPosition(Vector2f(10.0f, 120.0f));
	GraphicsSecondsPerFrameCaptionLabel->SetSize(Vector2f(150.0f, 20.0f));
	m_GraphicsSecondsPerFrameLabel = new Label(this, "0");
	m_GraphicsSecondsPerFrameLabel->SetPosition(Vector2f(170.0f, 120.0f));
	m_GraphicsSecondsPerFrameLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_GraphicsSecondsPerFrameLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
}

void TimingDialog::UpdateFramesPerSecond(float FramesPerSecond)
{
	m_FramesPerSecondLabel->SetString(to_string_cast(FramesPerSecond, 2));
}

void TimingDialog::UpdateAISecondsPerFrame(float AISecondsPerFrame)
{
	m_AISecondsPerFrameLabel->SetString(to_string_cast(AISecondsPerFrame * 1000, 2) + " ms");
}

void TimingDialog::UpdateGraphicsSecondsPerFrame(float GraphicsSecondsPerFrame)
{
	m_GraphicsSecondsPerFrameLabel->SetString(to_string_cast(GraphicsSecondsPerFrame * 1000, 2) + " ms");
}

void TimingDialog::UpdatePhysicsSecondsPerFrame(float PhysicsSecondsPerFrame)
{
	m_PhysicsSecondsPerFrameLabel->SetString(to_string_cast(PhysicsSecondsPerFrame * 1000, 2) + " ms");
}

void TimingDialog::UpdateTotalSecondsPerFrame(float TotalSecondsPerFrame)
{
	m_TotalSecondsPerFrameLabel->SetString(to_string_cast(TotalSecondsPerFrame * 1000, 2) + " ms");
}
