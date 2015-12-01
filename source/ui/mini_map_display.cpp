/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include <algebra/matrix4f.h>

#include "../commodity.h"
#include "../globals.h"
#include "../graphics/callback_node.h"
#include "../graphics/camera.h"
#include "../graphics/color_rgbo.h"
#include "../graphics/engine.h"
#include "../graphics/gl.h"
#include "../graphics/perspective_projection.h"
#include "../graphics/scene.h"
#include "../graphics/texture.h"
#include "../graphics/texture_manager.h"
#include "../graphics/texture_render_target.h"
#include "../graphics/view.h"
#include "../object_aspect_position.h"
#include "../planet.h"
#include "../ship.h"
#include "../system.h"
#include "event.h"
#include "mini_map_display.h"

UI::MiniMapDisplay::MiniMapDisplay(UI::Widget * SupWidget) :
	UI::ViewDisplay(SupWidget)
{
	SetSize(Vector2f(100.0f, 100.0f));
	ConnectDestroyingCallback(std::bind(&UI::MiniMapDisplay::_OnDestroying, this, std::placeholders::_1));
	ConnectSizeChangedCallback(std::bind(&UI::MiniMapDisplay::_OnSizeChanged, this, std::placeholders::_1));
	_SetupView();
}

UI::MiniMapDisplay::~MiniMapDisplay(void)
{
}

void UI::MiniMapDisplay::SetOwner(Reference< Ship > Owner)
{
	_Owner = Owner;
}

void UI::MiniMapDisplay::_ClearView(void)
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

void UI::MiniMapDisplay::_SetupView(void)
{
	Graphics::PerspectiveProjection * PerspectiveProjection(new Graphics::PerspectiveProjection());
	
	PerspectiveProjection->SetFieldOfViewY(0.8f);
	PerspectiveProjection->SetAspect(GetSize()[0] / GetSize()[1]);
	PerspectiveProjection->SetNearClippingPlane(1.0f);
	PerspectiveProjection->SetFarClippingPlane(10000.0f);
	
	auto View(new Graphics::View());
	
	View->SetClearColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 0.0f));
	assert(View->GetCamera() != nullptr);
	View->GetCamera()->SetProjection(PerspectiveProjection);
	View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateTranslation(0.0f, 0.0f, 1500.0f));
	assert(g_GraphicsEngine != nullptr);
	g_GraphicsEngine->AddView(View);
	
	auto Scene(new Graphics::Scene());
	
	Scene->SetDestroyCallback(std::bind(&UI::MiniMapDisplay::_OnDestroyInScene, this, std::placeholders::_1));
	View->SetScene(Scene);
	
	auto Texture(new Graphics::Texture());
	
	Texture->Create(GetSize()[0], GetSize()[1], 1);
	
	auto RenderTarget(new Graphics::TextureRenderTarget());
	
	RenderTarget->SetTexture(Texture);
	View->SetRenderTarget(RenderTarget);
	
	auto RootNode(new Graphics::CallbackNode());
	
	RootNode->SetDrawCallback(std::bind(&UI::MiniMapDisplay::_OnDraw, this, std::placeholders::_1));
	RootNode->SetClearColorBuffer(true);
	RootNode->SetClearDepthBuffer(true);
	RootNode->SetUseBlending(false);
	RootNode->SetUseDepthTest(true);
	Scene->SetRootNode(RootNode);
	SetView(View);
}

void UI::MiniMapDisplay::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_ClearView();
	}
}

void UI::MiniMapDisplay::_OnDestroyInScene(Graphics::Node * Node)
{
	delete Node;
}

void UI::MiniMapDisplay::_OnDraw(Graphics::RenderContext * RenderContext)
{
	if(_Owner.IsValid() == true)
	{
		assert(_Owner->GetAspectPosition() != nullptr);
		GLTranslatef(-_Owner->GetAspectPosition()->GetPosition()[0], -_Owner->GetAspectPosition()->GetPosition()[1], 0);
		
		auto TheSystem(_Owner->GetSystem());
		
		assert(TheSystem != nullptr);
		GLBegin(GL_POINTS);
		GLColor3f(0.8f, 0.8f, 0.8f);
		for(auto Planet : TheSystem->GetPlanets())
		{
			if(Planet == _Owner->GetTarget())
			{
				GLColor3f(0.2f, 1.0f, 0.0f);
			}
			GLVertex2f(Planet->GetAspectPosition()->GetPosition()[0], Planet->GetAspectPosition()->GetPosition()[1]);
			if(Planet == _Owner->GetTarget())
			{
				GLColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(auto Ship : TheSystem->GetShips())
		{
			if(Ship == _Owner->GetTarget())
			{
				GLColor3f(0.2f, 1.0f, 0.0f);
			}
			GLVertex2f(Ship->GetAspectPosition()->GetPosition()[0], Ship->GetAspectPosition()->GetPosition()[1]);
			if(Ship == _Owner->GetTarget())
			{
				GLColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(auto Commodity : TheSystem->GetCommodities())
		{
			if(Commodity == _Owner->GetTarget())
			{
				GLColor3f(0.2f, 1.0f, 0.0f);
			}
			GLVertex2f(Commodity->GetAspectPosition()->GetPosition()[0], Commodity->GetAspectPosition()->GetPosition()[1]);
			if(Commodity == _Owner->GetTarget())
			{
				GLColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		GLEnd();
	}
}

void UI::MiniMapDisplay::_OnSizeChanged(UI::Event & SizeChangedEvent)
{
	if(SizeChangedEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_ClearView();
		_SetupView();
	}
}
