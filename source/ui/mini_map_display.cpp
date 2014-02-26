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

#include "../callbacks/callbacks.h"
#include "../color.h"
#include "../commodity.h"
#include "../globals.h"
#include "../graphics/callback_node.h"
#include "../graphics/camera.h"
#include "../graphics/engine.h"
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
#include "mini_map_display.h"

UI::MiniMapDisplay::MiniMapDisplay(UI::Widget * SupWidget) :
	UI::ViewDisplay(SupWidget)
{
	SetSize(Vector2f(100.0f, 100.0f));
	ConnectDestroyingCallback(Callback(this, &UI::MiniMapDisplay::_OnDestroying));
	ConnectSizeChangedCallback(Callback(this, &UI::MiniMapDisplay::_OnSizeChanged));
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
	Graphics::View * OldView(GetView());
	
	if(OldView != 0)
	{
		SetView(0);
		assert(OldView->GetScene() != 0);
		
		Graphics::Scene * Scene(OldView->GetScene());
		
		OldView->SetScene(0);
		delete Scene;
		
		assert(OldView->GetCamera() != 0);
		assert(OldView->GetCamera()->GetProjection() != 0);
		
		Graphics::Projection * Projection(OldView->GetCamera()->GetProjection());
		
		OldView->GetCamera()->SetProjection(0);
		delete Projection;
		
		Graphics::TextureRenderTarget * TextureRenderTarget(dynamic_cast< Graphics::TextureRenderTarget * >(OldView->GetRenderTarget()));
		
		assert(TextureRenderTarget != 0);
		OldView->SetRenderTarget(0);
		
		Graphics::Texture * Texture(TextureRenderTarget->GetTexture());
		
		TextureRenderTarget->SetTexture(0);
		delete TextureRenderTarget;
		g_GraphicsEngine->GetTextureManager()->Destroy(Texture->GetIdentifier());
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
	
	Graphics::View * View(new Graphics::View());
	
	View->SetClearColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
	assert(View->GetCamera() != 0);
	View->GetCamera()->SetProjection(PerspectiveProjection);
	View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateFromTranslationComponents(0.0f, 0.0f, 1500.0f));
	assert(g_GraphicsEngine != 0);
	g_GraphicsEngine->AddView(View);
	
	Graphics::Scene * Scene(new Graphics::Scene());
	
	Scene->SetDestroyCallback(std::bind(&UI::MiniMapDisplay::_OnDestroyInScene, this, std::placeholders::_1));
	View->SetScene(Scene);
	
	Graphics::Texture * Texture(g_GraphicsEngine->GetTextureManager()->Create("mini-map-display"));
	
	assert(Texture != 0);
	Texture->Create(GetSize()[0], GetSize()[1], 1);
	
	Graphics::TextureRenderTarget * RenderTarget(new Graphics::TextureRenderTarget());
	
	RenderTarget->SetTexture(Texture);
	View->SetRenderTarget(RenderTarget);
	
	Graphics::CallbackNode * RootNode(new Graphics::CallbackNode());
	
	RootNode->SetDrawCallback(Callback(this, &UI::MiniMapDisplay::_OnDraw));
	RootNode->SetClearColorBuffer(true);
	RootNode->SetClearDepthBuffer(true);
	RootNode->SetUseBlending(false);
	RootNode->SetUseLighting(false);
	RootNode->SetUseDepthTest(true);
	Scene->SetRootNode(RootNode);
	SetView(View);
}

void UI::MiniMapDisplay::_OnDestroying(void)
{
	_ClearView();
}

void UI::MiniMapDisplay::_OnDestroyInScene(Graphics::Node * Node)
{
	delete Node;
}

void UI::MiniMapDisplay::_OnDraw(void)
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

void UI::MiniMapDisplay::_OnSizeChanged(void)
{
	_ClearView();
	_SetupView();
}
