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

#include "../character.h"
#include "../commodity.h"
#include "../globals.h"
#include "../graphics/callback_node.h"
#include "../graphics/camera.h"
#include "../graphics/color_rgbo.h"
#include "../graphics/drawing.h"
#include "../graphics/engine.h"
#include "../graphics/orthogonal_2d_projection.h"
#include "../graphics/render_context.h"
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
	UI::ViewDisplay(SupWidget),
	_Character(nullptr),
	_Scale(0.15f)
{
	ConnectDestroyingCallback(std::bind(&UI::MiniMapDisplay::_OnDestroying, this, std::placeholders::_1));
	ConnectHeightChangedCallback(std::bind(&UI::MiniMapDisplay::_OnSizeChanged, this, std::placeholders::_1));
	ConnectWidthChangedCallback(std::bind(&UI::MiniMapDisplay::_OnSizeChanged, this, std::placeholders::_1));
	_SetupView();
}

UI::MiniMapDisplay::~MiniMapDisplay(void)
{
	assert(_Character == nullptr);
	assert(_CharacterDestroyingConnection.IsValid() == false);
}

void UI::MiniMapDisplay::SetCharacter(Character * Character)
{
	if(_Character != nullptr)
	{
		assert(_CharacterDestroyingConnection.IsValid() == true);
		_CharacterDestroyingConnection.Disconnect();
		_Character = nullptr;
	}
	assert(_CharacterDestroyingConnection.IsValid() == false);
	if(Character != nullptr)
	{
		_Character = Character;
		_CharacterDestroyingConnection = _Character->ConnectDestroyingCallback(std::bind(&UI::MiniMapDisplay::_OnCharacterDestroying, this));
	}
	_ClearView();
	_SetupView();
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
	if((GetWidth() > 0.0f) && (GetHeight() > 0.0f))
	{
		auto OrthogonalProjection{new Graphics::Orthogonal2DProjection{}};
		
		// the orthogonal pojection is designed to place (0.0, 0.0) in the middle of the widget, scale appropriately and make this a game coordinate view (y axis is up)
		OrthogonalProjection->SetLeft(-GetWidth() / 2.0f / _Scale);
		OrthogonalProjection->SetTop(GetHeight() / 2.0f / _Scale);
		OrthogonalProjection->SetRight(GetWidth() / 2.0f / _Scale);
		OrthogonalProjection->SetBottom(-GetHeight() / 2.0f / _Scale);
		
		auto View{new Graphics::View{}};
		
		View->SetClearColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 0.0f));
		assert(View->GetCamera() != nullptr);
		View->GetCamera()->SetProjection(OrthogonalProjection);
		if(_Character != nullptr)
		{
			assert(_Character->GetShip() != nullptr);
			assert(_Character->GetShip()->GetAspectPosition() != nullptr);
			View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateTranslation(_Character->GetShip()->GetAspectPosition()->GetPosition()[0], _Character->GetShip()->GetAspectPosition()->GetPosition()[1], 0.0f));
		}
		else
		{
			View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateTranslation(0.0f, 0.0f, 0.0f));
		}
		assert(g_GraphicsEngine != nullptr);
		g_GraphicsEngine->AddView(View);
		
		auto Scene{new Graphics::Scene{}};
		
		Scene->SetDestroyCallback(std::bind(&UI::MiniMapDisplay::_OnDestroyInScene, this, std::placeholders::_1));
		View->SetScene(Scene);
		
		auto Texture{new Graphics::Texture{}};
		
		Texture->Create(GetWidth(), GetHeight(), 1);
		
		auto RenderTarget{new Graphics::TextureRenderTarget{}};
		
		RenderTarget->SetTexture(Texture);
		View->SetRenderTarget(RenderTarget);
		
		auto RootNode{new Graphics::CallbackNode{}};
		
		RootNode->SetDrawCallback(std::bind(&UI::MiniMapDisplay::_OnDraw, this, std::placeholders::_1));
		RootNode->SetClearColorBuffer(true);
		RootNode->SetClearDepthBuffer(true);
		RootNode->SetUseBlending(false);
		RootNode->SetUseDepthTest(true);
		Scene->SetRootNode(RootNode);
		SetView(View);
	}
}

void UI::MiniMapDisplay::_OnCharacterDestroying(void)
{
	assert(_Character != nullptr);
	_Character = nullptr;
	assert(_CharacterDestroyingConnection.IsValid() == true);
	_CharacterDestroyingConnection.Disconnect();
	assert(_CharacterDestroyingConnection.IsValid() == false);
}

void UI::MiniMapDisplay::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_ClearView();
		if(_Character != nullptr)
		{
			assert(_CharacterDestroyingConnection.IsValid() == true);
			_CharacterDestroyingConnection.Disconnect();
			_Character = nullptr;
		}
		assert(_CharacterDestroyingConnection.IsValid() == false);
	}
}

void UI::MiniMapDisplay::_OnDestroyInScene(Graphics::Node * Node)
{
	delete Node;
}

void UI::MiniMapDisplay::_OnDraw(Graphics::RenderContext * RenderContext)
{
	assert(GetView() != nullptr);
	assert(GetView()->GetCamera() != nullptr);
	if(_Character != nullptr)
	{
		auto CharacterShip{_Character->GetShip()};
		
		assert(CharacterShip != nullptr);
		assert(CharacterShip->GetAspectPosition() != nullptr);
		GetView()->GetCamera()->SetSpacialMatrix(Matrix4f::CreateTranslation(_Character->GetShip()->GetAspectPosition()->GetPosition()[0], _Character->GetShip()->GetAspectPosition()->GetPosition()[1], 0.0f));
		
		auto System{_Character->GetSystem()};
		
		assert(System != nullptr);
		
		std::vector< Vector2f > Positions;
		std::vector< Graphics::ColorRGBO > Colors;
		
		for(auto Planet : System->GetPlanets())
		{
			Positions.push_back(Vector2f(Planet->GetAspectPosition()->GetPosition()[0], Planet->GetAspectPosition()->GetPosition()[1]));
			if(Planet == CharacterShip->GetTarget())
			{
				Colors.push_back(Graphics::ColorRGBO(0.2f, 1.0f, 0.0f, 1.0f));
			}
			else
			{
				Colors.push_back(Graphics::ColorRGBO(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}
		for(auto Ship : System->GetShips())
		{
			Positions.push_back(Vector2f(Ship->GetAspectPosition()->GetPosition()[0], Ship->GetAspectPosition()->GetPosition()[1]));
			if(Ship == CharacterShip->GetTarget())
			{
				Colors.push_back(Graphics::ColorRGBO(0.2f, 1.0f, 0.0f, 1.0f));
			}
			else
			{
				Colors.push_back(Graphics::ColorRGBO(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}
		for(auto Commodity : System->GetCommodities())
		{
			Positions.push_back(Vector2f(Commodity->GetAspectPosition()->GetPosition()[0], Commodity->GetAspectPosition()->GetPosition()[1]));
			if(Commodity == CharacterShip->GetTarget())
			{
				Colors.push_back(Graphics::ColorRGBO(0.2f, 1.0f, 0.0f, 1.0f));
			}
			else
			{
				Colors.push_back(Graphics::ColorRGBO(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}
		RenderContext->SetProgramIdentifier("flat");
		Graphics::Drawing::DrawPoints(RenderContext, Positions, Colors);
		RenderContext->UnsetProgramIdentifier();
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
