/**
 * galactic-fall
 * Copyright (C) 2006-2019  Hagen Möbius
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

#include <graphics/camera.h>
#include <graphics/color_rgbo.h>
#include <graphics/engine.h>
#include <graphics/light.h>
#include <graphics/node.h>
#include <graphics/perspective_projection.h>
#include <graphics/scene.h>
#include <graphics/texture.h>
#include <graphics/texture_render_target.h>
#include <graphics/view.h>

#include "../globals.h"
#include "../object.h"
#include "../object_aspect_physical.h"
#include "../object_aspect_position.h"
#include "../star.h"
#include "../system.h"
#include "../visualization.h"
#include "../visualizations.h"
#include "event.h"
#include "scanner_display.h"

UI::ScannerDisplay::ScannerDisplay(void) :
	_Target(nullptr)
{
	ConnectDestroyingCallback(std::bind(&UI::ScannerDisplay::_OnDestroying, this, std::placeholders::_1));
}

void UI::ScannerDisplay::SetTarget(Object * Target)
{
	assert((Target == nullptr) || (Target->GetAspectVisualization() != nullptr));
	if(Target != _Target)
	{
		if(_Target != nullptr)
		{
			_Clear();
			_Target->DisconnectDestroyingCallback(_TargetDestroyingConnection);
			_Target = nullptr;
		}
		if(Target != nullptr)
		{
			_Target = Target;
			_TargetDestroyingConnection = _Target->ConnectDestroyingCallback(std::bind(&UI::ScannerDisplay::_OnTargetDestroying, this));
			_Setup();
		}
	}
}

void UI::ScannerDisplay::_Clear(void)
{
	auto OldView(GetView());
	
	if(OldView != nullptr)
	{
		SetView(nullptr);
		assert(OldView->GetScene() != nullptr);
		
		auto Scene(OldView->GetScene());
		
		OldView->SetScene(nullptr);
		delete Scene;
		
		assert(OldView->GetCamera() != nullptr);
		assert(OldView->GetCamera()->GetProjection() != nullptr);
		
		auto Projection(OldView->GetCamera()->GetProjection());
		
		OldView->GetCamera()->SetProjection(nullptr);
		delete Projection;
		
		auto TextureRenderTarget(dynamic_cast< Graphics::TextureRenderTarget * >(OldView->GetRenderTarget()));
		
		assert(TextureRenderTarget != nullptr);
		OldView->SetRenderTarget(nullptr);
		
		auto Texture(TextureRenderTarget->GetTexture());
		
		TextureRenderTarget->SetTexture(nullptr);
		delete TextureRenderTarget;
		delete Texture;
		g_GraphicsEngine->RemoveView(OldView);
		delete OldView;
	}
}

void UI::ScannerDisplay::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_Clear();
	}
}

void UI::ScannerDisplay::_OnDestroyInScene(Graphics::Node * Node)
{
	InvalidateVisualizationReference(Node);
	delete Node;
}

void UI::ScannerDisplay::_OnTargetDestroying(void)
{
	_Clear();
	_Target = nullptr;
}

void UI::ScannerDisplay::_Setup(void)
{
	if((GetWidth() > 0.0f) && (GetHeight() > 0.0f))
	{
		assert(_Target != nullptr);
		assert(_Target->GetAspectPhysical() != nullptr);
		
		auto PerspectiveProjection{new Graphics::PerspectiveProjection{}};
		float RadialSize(_Target->GetAspectPhysical()->GetRadialSize());
		float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
		
		PerspectiveProjection->SetFieldOfViewY(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)) * 2.0f);
		PerspectiveProjection->SetAspect(GetWidth() / GetHeight());
		PerspectiveProjection->SetNearClippingPlane(1.0f);
		PerspectiveProjection->SetFarClippingPlane(1000.0f);
		
		auto View{new Graphics::View{}};
		
		g_GraphicsEngine->AddView(View);
		View->SetClearColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 0.0f));
		assert(View->GetCamera() != nullptr);
		View->GetCamera()->SetProjection(PerspectiveProjection);
		assert(_Target->GetAspectPosition() != nullptr);
		View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateTranslation(0.0f, 0.0f, 4.0f * RadialSize));
		
		auto Scene{new Graphics::Scene{}};
		
		Scene->SetDestroyCallback(std::bind(&UI::ScannerDisplay::_OnDestroyInScene, this, std::placeholders::_1));
		Scene->ActivateLight();
		assert(Scene->GetLight() != nullptr);
		
		auto TheSystem{dynamic_cast< System * >(_Target->GetContainer())};
		
		assert(TheSystem != nullptr);
		assert(TheSystem->GetStar() != nullptr);
		assert(TheSystem->GetStar()->GetAspectPosition() != nullptr);
		Scene->GetLight()->SetType(Graphics::Light::Type::Directional);
		Scene->GetLight()->SetDirection(Vector3f::CreateFromComponents(-TheSystem->GetStar()->GetAspectPosition()->GetPosition()[0], -TheSystem->GetStar()->GetAspectPosition()->GetPosition()[1], -100.0f));
		Scene->GetLight()->SetColor(TheSystem->GetStar()->GetColor());
		View->SetScene(Scene);
		
		auto Texture{new Graphics::Texture{}};
		
		Texture->Create(GetWidth(), GetHeight(), 1);
		
		auto RenderTarget{new Graphics::TextureRenderTarget{}};
		
		RenderTarget->SetTexture(Texture);
		View->SetRenderTarget(RenderTarget);
		
		auto RootNode{new Graphics::Node{}};
		
		RootNode->SetClearColorBuffer(true);
		RootNode->SetClearDepthBuffer(true);
		RootNode->SetUseDepthTest(true);
		Scene->SetRootNode(RootNode);
		
		auto Visualization{VisualizeObject(_Target, RootNode)};
		
		Visualization->SetUpdatePosition(false);
		SetView(View);
	}
}
